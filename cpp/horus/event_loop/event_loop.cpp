#include "horus/event_loop/event_loop.h"

#include <uv.h>

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "horus/event_loop/uv.h"
#include "horus/event_loop/waker.h"
#include "horus/future/core_owner.h"
#include "horus/future/poll.h"
#include "horus/pointer/unsafe_cast.h"

namespace horus {
namespace horus_internal {

EventLoop* EventLoopFromUv(uv_loop_t* loop) noexcept {
  return UnsafePointerCast<EventLoop>(loop->data);
}

EventLoop::State::State(EventLoop& loop) noexcept : loop_{}, ticket_handle_{} {
  loop_.Get().data = &loop;
  UvAssert(uv_loop_init(&loop_.Get()));
  UvAssert(uv_async_init(&loop_.Get(), &ticket_handle_.Get(), [](uv_async_t* async) noexcept {
    State& self{*UnsafePointerCast<State>(async->data)};
    TicketQueue tickets;
    {
      const std::unique_lock<std::mutex> lock{self.mutex_};
      std::swap(tickets, self.tickets_);
    }
    if (tickets.first != nullptr) {
      // `tickets.first` should always be non-null since this callback was scheduled, but we may
      // give the ability to cancel tickets in the future so to avoid problems then we'll handle
      // this case here.
      std::move(*tickets.first).RunChainAndDestroy(self.Loop());
    }
  }));
  // Unref the ticket handle, otherwise it will prevent `Run()` from exiting. Note that the handle
  // still needs to be closed when shutting down.
  uv_unref(&UvToHandle(ticket_handle_));
  ticket_handle_.Get().data = this;
}

bool EventLoop::State::IncTasks() noexcept {
  std::thread::id const loop_thread{loop_thread_};
  if (std::this_thread::get_id() == loop_thread) {
    return false;
  }
  std::unique_lock<std::mutex> lock{mutex_};
  task_capacity_cv_.wait(lock, [this]() noexcept -> bool { return task_capacity_ > 0; });
  --task_capacity_;
  return true;
}

void EventLoop::State::DecTasks() noexcept {
  const std::unique_lock<std::mutex> lock{mutex_};
  if (++task_capacity_ == 1) {
    task_capacity_cv_.notify_one();
  }
}

void EventLoop::State::Run() noexcept {
  std::thread::id const prev_thread{
      loop_thread_.exchange(std::this_thread::get_id(), std::memory_order_acquire)};
  assert(prev_thread == std::thread::id{});
  static_cast<void>(prev_thread);
  // We allow `uv_run()` to return non-zero values since that simply indicates that
  // `uv_stop()` was called. We don't expect `uv_run()` to be non-zero either, because
  // `uv_run()` will return 0 when `uv_stop()` is called if there are no pending events.
  static_cast<void>(uv_run(&loop_.Get(), UV_RUN_DEFAULT));
  loop_thread_.store({}, std::memory_order_release);
}

void EventLoop::State::Shutdown() noexcept {
  uv_close(&UvToHandle(ticket_handle_), nullptr);
  const std::unique_lock<std::mutex> lock{mutex_};
  if (tickets_.first != nullptr) {
    std::move(*tickets_.first).DestroyChain();
  }
}

EventLoop::~EventLoop() noexcept {
  InternalState()->Shutdown();
  std::int32_t status{uv_loop_close(&loop_->UvLoop())};
  if (status == UV_EBUSY) {
    static_cast<void>(uv_run(&loop_->UvLoop(), UV_RUN_NOWAIT));
    status = uv_loop_close(&loop_->UvLoop());
  }
  UvAssert(status);
}

void EventLoop::Task::Spawn(EventLoop& loop) noexcept {
  UvAssert(
      uv_async_init(&loop.loop_->UvLoop(), waker_owner_.Handle(), [](uv_async_t* async) noexcept {
        Task& self{*UnsafePointerCast<Task>(async->data)};
        assert(self.waker_owner_.Handle() == async);
        EventLoop& inner_loop{*EventLoopFromUv(async->loop)};
        CoreFutureOwner& future_owner{self.future_owner_};

        PollContext context{inner_loop, future_owner, self.waker_owner_.Waker()};
        if (!future_owner.DidStartDestruction()) {
          if (!self.PollIsReady(context)) {
            // Future did not complete, try again later when woken up.
            return;
          }
          // Future just completed or failed; start its destruction.
          static_cast<void>(future_owner.PollDestroy(self.waker_owner_.Waker()));
        }
        if (!future_owner.DidCompleteDestruction()) {
          // Destruction did not complete, try again later when woken up.
          return;
        }

        // Future completed and its resources were destroyed; we can completely stop now.
        uv_close(&UvToHandle(*async), [](uv_handle_t* close_handle) noexcept {
          assert(close_handle->type == UV_ASYNC);
          Task& close_self{*UnsafePointerCast<Task>(close_handle->data)};
          assert(&UvToHandle(*close_self.waker_owner_.Handle()) == close_handle);

          close_self.NotifyClosed();
        });
      }));

  waker_owner_.Wake();
}

bool EventLoop::AwaitTask::Await(EventLoop& loop) noexcept(false) {
  State::InvokeInLoop(loop.InternalState(),
                      [this](EventLoop& inner_loop) noexcept { Spawn(inner_loop); });

  std::unique_lock<std::mutex> lock{mutex_};
  end_cv_.wait(lock, [this]() noexcept -> bool { return FutureCompleted(); });
  return true;
}

void EventLoop::State::Ticket::Register(const std::shared_ptr<State>& state) noexcept {
  state_ = state;
  {
    const std::unique_lock<std::mutex> lock{state->mutex_};
    if (state->tickets_.last == nullptr) {
      // We're the first (and last) ticket.
      state->tickets_.first = this;
      state->tickets_.last = this;
    } else {
      state->tickets_.last->next_ = this;
    }
    state->tickets_.last = this;
  }
  // `uv_async_send()` coalesces multiple calls to `uv_async_send()` into one.
  UvAssert(uv_async_send(&state->ticket_handle_.Get()));
}

void EventLoop::State::Ticket::RunChainAndDestroy(EventLoop& loop) && noexcept {
  Ticket* next{this};
  while (next != nullptr) {
    Ticket* const next_next{next->next_};
    std::move(*next).RunAndDestroy(&loop);
    next = next_next;
  }
}

void EventLoop::State::Ticket::DestroyChain() && noexcept {
  Ticket* next{this};
  while (next != nullptr) {
    Ticket* const next_next{next->next_};
    std::move(*next).RunAndDestroy(nullptr);
    next = next_next;
  }
}

}  // namespace horus_internal
}  // namespace horus
