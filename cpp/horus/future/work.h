/// @file
///
/// The `WorkFuture` future.

#ifndef HORUS_FUTURE_WORK_H_
#define HORUS_FUTURE_WORK_H_

#include <atomic>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/event_loop/uv.h"
#include "horus/event_loop/waker.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"
#include "horus/internal/enum.h"
#include "horus/pointer/unsafe_cast.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"

namespace horus {

// MARK: DeferWork()

/// Future returned by `DeferWork()`.
template <class F>
class WorkFuture final
    : public Future<decltype(std::declval<F&>()(std::declval<std::atomic<bool>&>()))> {
 public:
  /// @copydoc Future::Result
  using typename WorkFuture::Future::Result;

  /// Constructs the future.
  explicit WorkFuture(F&& work, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : work_{InPlaceType<F>, std::move(work)}, info_{info} {}

  /// Futures cannot be copied.
  WorkFuture(const WorkFuture&) = delete;
  /// Futures cannot be copied.
  WorkFuture& operator=(const WorkFuture&) = delete;
  /// Move-constructs the future.
  WorkFuture(WorkFuture&&) noexcept = default;
  /// Move-assigns the future.
  WorkFuture& operator=(WorkFuture&&) noexcept = default;

  /// Destroys the future, cancelling the work.
  ~WorkFuture() noexcept final;

  /// Polls the future until its work has completed in the worker thread.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// Structure used to notify the work that it has been cancelled.
  struct CancelFlag {
    /// Whether the work was cancelled.
    std::atomic<bool> cancelled{false};
  };

  /// Type of `state_`.
  using State = OneOf<F, CancelFlag*, Result, std::exception_ptr>;

  /// Type of `work_`.
  struct Work {
    /// Constructs the `Work` object.
    explicit Work(F&& work) noexcept : state{InPlaceType<F>, std::move(work)} {
      horus_internal::UvSetData(handle, state.template TryAs<F>());
    }

    /// Handle to the work.
    ///
    /// The data is a pointer to an arbitrary value. When `data` is nullptr, the work was cancelled.
    horus_internal::Uv<uv_work_t> handle;

    /// The mutex used to guard `state_` and `cancelled_`.
    std::mutex mutex;

    /// The state of the work:
    ///
    /// - `F`: scheduled, but not yet started.
    /// - `CancelFlag`: started, but not yet completed.
    /// - `Result`: completed, but not yet polled.
    /// - `std::exception_ptr`: failed, but not yet polled.
    State state;

    /// Waker to notify the end of the work.
    horus_internal::FutureWaker waker;
  };

  /// The work to perform. If `F`, the future hasn't been polled yet. If `Work*`, this is never
  /// `nullptr`.
  OneOf<F, Work*> work_;

  /// Future information.
  FutureInfo info_;
};

/// Returns a future which completes when `work()` completes, with `work()` executing in a worker
/// thread.
///
/// @warning The event loop should not be accessed during the work, as it executes on a different
/// thread. The result of futures spawned on the event loop should not be awaited either, as it
/// might starve the event loop thread pool. If access to the event loop is necessary, consider
/// using `Async()`, though it is more expensive.
template <class F>
inline WorkFuture<F> DeferWork(F&& work, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return WorkFuture<F>{std::forward<F>(work), info};
}

// MARK: Function definitions

template <class F>
WorkFuture<F>::~WorkFuture() noexcept {
  if (!work_.template Is<Work*>()) {
    return;  // Work did not start.
  }
  Work* const maybe_work{*work_.template TryAs<Work*>()};
  if (maybe_work == nullptr) {
    // Work completed.
    return;
  }
  bool free_work{false};
  {
    Work& work{*maybe_work};
    const std::unique_lock<std::mutex> lock{work.mutex};
    switch (work.state.Tag()) {
      case State::template kTagFor<F>: {
        // Work was scheduled, but not yet started.
        //
        // Remove `F` to tell the worker that the work was cancelled.
        work.state.template Emplace<CancelFlag*>(nullptr);

        // The worker was enqueued and will access the `handle`, so we can't free `work` yet.
        free_work = false;
        break;
      }
      case State::template kTagFor<CancelFlag*>: {
        // Work has started, but not yet completed.
        //
        // Update the `CancelFlag` to tell the worker that it should abort.
        work.state.template As<CancelFlag*>()->cancelled.store(true);

        // The worker is running and will access the `handle`, so we can't free `work` yet.
        free_work = false;
        break;
      }
      case State::template kTagFor<Result>:
      case State::template kTagFor<std::exception_ptr>: {
        // Work completed. The worker is closing or closed.
        if (horus_internal::UvGetData(work.handle) == nullptr) {
          // Work was closed.
          free_work = true;
        } else {
          // Work is closing, but `after_work_cb` hasn't been called yet.
          free_work = false;
          horus_internal::UvSetData(work.handle, nullptr);
        }
        break;
      }
      default:
        break;
    }
  }
  if (free_work) {
    std::unique_ptr<Work>(*work_.template TryAs<Work*>()).reset();
  }
}

template <class F>
auto WorkFuture<F>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Work", info_};

  if (work_.template Is<F>()) {
    // Note that `move()` will not have any effect on `F` if `std::make_unique` throws due to an
    // allocation error.
    std::unique_ptr<Work> work_ptr{std::make_unique<Work>(std::move(work_.template As<F>()))};
    static_cast<void>(work_.template Emplace<Work*>(work_ptr.get()));
    work_ptr->waker = context.Waker();

    uv_work_cb const work_cb{[](uv_work_t* work_handle) noexcept {
      // If this function is called, then the `work` still exists and it will not be
      // destroyed until closed (which requires this function to return).
      Work& work{*UnsafePointerCast<Work>(work_handle)};

      CancelFlag cancel_flag;  // Allow cancellation while running.
      std::unique_lock<std::mutex> lock{work.mutex};
      if (!work.state.template Is<F>()) {
        // Work was cancelled.
        return;
      }

      // Acquire work.
      F work_invocable{std::move(work.state).template As<F>()};
      work.state.template Emplace<CancelFlag*>(&cancel_flag);
      lock.unlock();

      // Do work.
      try {
        // NOLINTNEXTLINE(*-const-correctness): false positive, we move it later
        Result result{work_invocable(cancel_flag.cancelled)};
        if (cancel_flag.cancelled.load()) {
          // Work was cancelled.
          return;
        }
        lock.lock();
        work.state.template Emplace<Result>(std::move(result));
      } catch (...) {
        if (cancel_flag.cancelled.load()) {
          // Work was cancelled.
          return;
        }
        lock.lock();
        work.state.template Emplace<std::exception_ptr>(std::current_exception());
      }
    }};
    uv_after_work_cb const after_work_cb{[](uv_work_t* work_handle, std::int32_t status) noexcept {
      Work* maybe_work{UnsafePointerCast<Work>(work_handle)};
      if (status == horus_internal::UvErrors::kCanceled) {
        // Work never started.
        std::unique_ptr<Work>(maybe_work).reset();
        return;
      }
      if (horus_internal::UvGetData(*work_handle) == nullptr) {
        // Future was destroyed and tasked us of deleting the shared state.
        std::unique_ptr<Work>(maybe_work).reset();
        return;
      }

      Work& work{*UnsafePointerCast<Work>(work_handle)};
      if (work.state.template Is<Result>() || work.state.template Is<std::exception_ptr>()) {
        // Work completed without a cancellation, so we can wake up the event loop.
        work.waker.Wake();
      }
      horus_internal::UvSetData(*work_handle,
                                nullptr);  // Tell Future destructor to deallocate work.
    }};

    // Futures should not be polled after failure, so we don't need to try again outside of the
    // first poll if the statement below fails.
    horus_internal::UvThrowIfError(uv_queue_work(horus_internal::EventLoopToUv(&context.Loop()),
                                                 &work_ptr.release()->handle.Get(), work_cb,
                                                 after_work_cb));

    return PendingResult<Result>();
  }

  // Work was already scheduled.
  Work* const maybe_work{work_.template As<Work*>()};
  if (maybe_work == nullptr) {
    throw PolledAfterCompletionError{};
  }

  Work& work{*maybe_work};
  const std::unique_lock<std::mutex> lock{work.mutex};
  switch (work.state.Tag()) {
    case State::template kTagFor<F>:
      // Work was scheduled, but not yet started.
    case State::template kTagFor<CancelFlag*>: {
      // Work has started, but not yet completed.
      return PendingResult<Result>();
    }
    case State::template kTagFor<Result>: {
      // Work has completed with value.
      return std::move(work.state).template As<Result>();
    }
    case State::template kTagFor<std::exception_ptr>: {
      // Work has completed with failure.
      std::rethrow_exception(work.state.template As<std::exception_ptr>());
      return PendingResult<Result>();  // Unreachable.
    }
    default:
      throw horus_internal::UnhandledEnumValueError{work.state.Tag()};
  }
}

}  // namespace horus

#endif  // HORUS_FUTURE_WORK_H_
