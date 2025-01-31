/// @file
///
/// The Horus `EventLoop` class.

#ifndef HORUS_EVENT_LOOP_EVENT_LOOP_H_
#define HORUS_EVENT_LOOP_EVENT_LOOP_H_

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <new>
#include <system_error>
#include <thread>
#include <type_traits>
#include <utility>

#include "horus/event_loop/uv.h"
#include "horus/event_loop/waker.h"
#include "horus/future/core_owner.h"
#include "horus/internal/pointer_cast.h"
#include "horus/internal/source_location.h"
#include "horus/strings/logging.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"

namespace horus {

class PollContext;

namespace horus_internal {

/// A [libuv](https://libuv.org)-based event loop which can execute `Future`s.
class EventLoop final {
 public:
  /// An error emitted by the event loop.
  using Error = std::system_error;

  /// The actual state of the event loop.
  class State final {
   public:
    /// A ticket used to schedule some code to run on the event loop.
    class Ticket;
    /// A `Ticket` which runs a future of type `F`.
    template <class F>
    class TicketFor;

    /// A list of tickets.
    struct TicketQueue {
      Ticket* first{nullptr};
      Ticket* last{nullptr};
    };

    /// (Internal use only) Constructs the `EventLoop` state.
    explicit State(EventLoop& loop) noexcept;

    /// Returns a reference to the owning `EventLoop`.
    EventLoop& Loop() noexcept { return *UnsafePointerCast<EventLoop>(UvGetData(loop_.Get())); }

    /// Returns a reference to the underlying libuv loop.
    uv_loop_t& UvLoop() noexcept { return loop_.Get(); }

    /// Increases the number of tasks, blocking if the limit was reached. If this returns `true`,
    /// `DecTasks()` must later be called.
    bool IncTasks() noexcept;

    /// Decreases the number of tasks.
    void DecTasks() noexcept;

    /// Runs the event loop until there is nothing left to do.
    void Run() noexcept;

    /// Shutdowns the loop as the `EventLoop` is being destroyed.
    void Shutdown() noexcept;

    /// Schedules for `invocable(EventLoop&)` to be invoked by the event loop soon. If called on the
    /// event loop, immediately executes `invocable()`.
    ///
    /// @throws std::bad_alloc If the invocation state cannot be allocated.
    template <class F>
    static void InvokeInLoop(const std::shared_ptr<State>& state, F&& invocable) noexcept(false);

   private:
    /// The initial value of `task_capacity_`.
    static constexpr std::uint32_t kTaskCapacity{1'024};

    /// The underlying libuv loop.
    Uv<uv_loop_t> loop_{};
    /// The thread on which the event loop is currently running. `id()` if it is not running.
    std::atomic<std::thread::id> loop_thread_{};

    /// Mutex which guards `active_tasks_`.
    std::mutex mutex_;
    /// The number of tasks which are available to be enqueued now, decreased with `IncTasks()` and
    /// increased with `DecTasks()`.
    std::uint32_t task_capacity_{kTaskCapacity};
    /// Condition variable used to notify `IncTasks()` that capacity is now available.
    std::condition_variable task_capacity_cv_;

    /// Async handle used to schedule
    Uv<uv_async_t> ticket_handle_;
    /// A queue of tickets requesting execution of some function on the event loop.
    TicketQueue tickets_;
  };

  /// A reference to the loop which can be used to invoke functions on it from another thread.
  class Invoker;

  /// Creates the event loop.
  ///
  /// @throws std::bad_alloc If the initial resources for the loop cannot be allocated.
  /// @throws std::system_error If the loop cannot be created.
  explicit EventLoop() noexcept(false) = default;

  /// This object cannot be copied.
  EventLoop(const EventLoop&) = delete;
  /// This object cannot be copied.
  EventLoop& operator=(const EventLoop&) = delete;
  /// This object cannot be moved.
  EventLoop(EventLoop&&) = delete;
  /// This object cannot be moved.
  EventLoop& operator=(EventLoop&&) = delete;

  /// Waits until the loop has no more outstanding work, then closes it and destroys all associated
  /// resources.
  ///
  /// @see https://docs.libuv.org/en/v1.x/loop.html#c.uv_loop_close
  ~EventLoop() noexcept;

  /// Executes the given `future` in the current thread, then runs the event loop until completion
  /// of the future, returning its result.
  ///
  /// Throws if we reach a point where the event loop has no more events to process but the future
  /// hasn't completed.
  template <class F>
  typename F::Result RunFuture(
      F&& future, SourceLocation source_location = CurrentSourceLocation()) noexcept(false);

  /// Schedules for `future` to execute in the event loop at some point in the future.
  ///
  /// This function will _not_ run the event loop; it should either be already running in another
  /// thread, or explicitly run after calling `SpawnFuture()`.
  ///
  /// Results and exceptions thrown by the future will be discarded. It is the responsibility of the
  /// caller to set up the future to do any necessary post-processing (using e.g. `Try()` and
  /// `Map()`).
  template <class F>
  void SpawnFuture(F&& future,
                   SourceLocation source_location = CurrentSourceLocation()) noexcept(false);

  /// Enqueues the execution of the given `future` in the event loop thread (assumed to be running
  /// in another thread), then blocks the current thread until its completion. If an exception is
  /// encountered, it will be rethrown.
  ///
  /// Use `CancelIn()` or `CancelAt()` if you'd like to limit the execution time of the future.
  template <class F>
  typename F::Result AwaitFuture(
      F&& future, SourceLocation source_location = CurrentSourceLocation()) noexcept(false);

  /// Returns a shared pointer to the inner `State`.
  const std::shared_ptr<State>& InternalState() const noexcept { return loop_; }

 private:
  /// A task to execute on the event loop, used by `{Run,Spawn}Future()`.
  class Task;
  /// A task to execute on the event loop, used by `AwaitFuture()`.
  class AwaitTask;

  /// `Task` implementation for `RunFuture()`.
  template <class F>
  class RunFutureTask;
  /// `Task` implementation for `SpawnFuture()`.
  template <class F>
  class SpawnFutureTask;
  /// `Task` implementation for `AwaitFuture()`.
  template <class F>
  class AwaitFutureTask;

  /// The libuv event loop.
  std::shared_ptr<State> loop_{std::make_shared<State>(*this)};
};

/// Returns a pointer to the `EventLoop` object bound to a `uv_loop_t`.
EventLoop* EventLoopFromUv(uv_loop_t* loop) noexcept;

/// Returns a pointer to the `uv_loop_t` object bound to an `EventLoop`.
inline static uv_loop_t* EventLoopToUv(EventLoop* loop) noexcept {
  return &loop->InternalState()->UvLoop();
}

// MARK: Ticket, InvokeInLoop()

class EventLoop::State::Ticket {
 public:
  /// Runs the function which was scheduled to run on the event loop, destroys the ticket, and then
  /// do so for subsequent items of the chain.
  void RunChainAndDestroy(EventLoop& loop) && noexcept;

  /// Destroys the ticket and any next ticket (recursively). The state mutex must be held.
  void DestroyChain() && noexcept;

  /// Tickets are immovable.
  Ticket(const Ticket&) = delete;
  /// Tickets are immovable.
  Ticket& operator=(const Ticket&) = delete;
  /// Tickets are immovable.
  Ticket(Ticket&&) = delete;
  /// Tickets are immovable.
  Ticket& operator=(Ticket&&) = delete;

  /// Destroys the ticket; if `Run()` hasn't executed yet, it will no longer be scheduled for
  /// execution.
  virtual ~Ticket() noexcept = default;

 protected:
  /// Constructs an empty ticket. `Register()` must be called soon after initializing the ticket.
  Ticket() noexcept = default;

  /// Runs the scheduled code. This function should also free all state held by `this`, including by
  /// doing a `delete this` if relevant. If `loop` is nullptr, the ticket is being destroyed early
  /// and should only destroy without running.
  virtual void RunAndDestroy(EventLoop* loop) && noexcept = 0;

  /// Registers the ticket; `Run()` will be called at some point in the future unless `Abort()` or
  /// `~Ticket()` is called first. This should only be called *once* in the constructor.
  ///
  /// The reason why this isn't done in the constructor is that, when the `Ticket` is constructed,
  /// the derived class may not have finished initializing, but we want it to be fully initialized
  /// before registering the ticket to avoid race conditions.
  void Register(const std::shared_ptr<State>& state) noexcept;

 private:
  /// Implementation of `Abort()` which does not call `Destroy()` (so that it may safely be called
  /// by the destructor).
  void AbortInternal() noexcept;

  /// A pointer to the state which owns this ticket.
  std::weak_ptr<State> state_{};
  /// The previous ticket, used when aborting.
  Ticket* prev_{nullptr};
  /// The next ticket.
  Ticket* next_{nullptr};
};

template <class F>
class EventLoop::State::TicketFor final : public Ticket {
 public:
  static_assert(noexcept(std::declval<F>()(std::declval<EventLoop&>())), "");

  /// Creates a ticket which will execute in the background and free itself in the future.
  ///
  /// @throw std::bad_alloc
  static void CreateAndRelease(const std::shared_ptr<State>& state, F&& invocable) noexcept(false) {
    TicketFor* const ticket{new TicketFor{std::move(invocable)}};
    ticket->Register(state);
  }

 protected:
  /// @copydoc Ticket::RunAndDestroy()
  void RunAndDestroy(EventLoop* loop) && noexcept final {
    if (loop != nullptr) {
      std::move(invocable_)(*loop);
      loop->InternalState()->DecTasks();
    }
    delete this;
  }

 private:
  /// Constructs the ticket.
  explicit TicketFor(F&& invocable) noexcept : Ticket{}, invocable_(std::move(invocable)) {}

  /// The underlying function to invoke.
  F invocable_;
};

template <class F>
// static
void EventLoop::State::InvokeInLoop(const std::shared_ptr<State>& state,
                                    F&& invocable) noexcept(false) {
  if (!state->IncTasks()) {
    // We're already on the event loop.
    std::forward<F>(invocable)(state->Loop());
    return;
  }
  try {
    TicketFor<F>::CreateAndRelease(state, std::forward<F>(invocable));
  } catch (const std::bad_alloc&) {
    state->DecTasks();
    throw;
  }
}

// MARK: RunFuture(), SpawnFuture()

class EventLoop::Task {
 public:
  /// Constructs a task given the source location of its caller.
  explicit Task(SourceLocation source_location) : source_location_{source_location} {
    UvSetData(*waker_owner_.Handle(), this);
  }

  /// Non-copyable.
  Task(const Task&) = delete;
  /// Non-copyable.
  Task& operator=(const Task&) = delete;
  /// Non-moveable.
  Task(Task&&) = delete;
  /// Non-moveable.
  Task& operator=(Task&&) = delete;

  /// Polls the future represented by the task for completion, then returns whether it is ready
  /// (i.e. has completed).
  virtual bool PollIsReady(PollContext& context) noexcept = 0;

  /// Notifies the caller that the task has completed and all its handles were closed. Whatever
  /// state allocated by the task may be freed here -- it will not be used again.
  virtual void NotifyClosed() noexcept = 0;

  /// Returns a pointer to the `uv_async_t` used to schedule execution of the task in the event
  /// loop.
  uv_async_t* Handle() noexcept { return waker_owner_.Handle(); }

  /// Returns the `SourceLocation` of the caller.
  constexpr SourceLocation SourceLoc() const noexcept { return source_location_; }

  /// Runs the underlying future in the event loop, awaiting its completion. The caller is
  /// responsible for checking that the future did (or did not) throw.
  void Run(EventLoop& loop) noexcept {
    Spawn(loop);
    loop.InternalState()->Run();
  }

  /// Spawns the underlying future into the event loop. This function must be called from the
  /// event loop.
  void Spawn(EventLoop& loop) noexcept;

 protected:
  /// Protected destructor to make sure only derived classes may be destroyed.
  ~Task() noexcept = default;

 private:
  /// The object used to wake the task. Must be the first object in the struct.
  FutureWaker::Owner waker_owner_;
  /// The owner of the future to execute.
  CoreFutureOwner future_owner_;

  /// Source location of the call which led to the future being executed.
  SourceLocation source_location_;
};

template <class F>
class EventLoop::RunFutureTask final : public Task {
  /// Result of `F`.
  using T = typename F::Result;
  /// Result of `F::UnsafePoll()`.
  using PollResult = decltype(std::declval<F&>().UnsafePoll(std::declval<PollContext&>()));
  /// Alias of `Pending` (using `PollResult` to avoid a header include cycle).
  using Pending = typename PollResult::template TypeAt<1>;

 public:
  /// Constructs the task.
  RunFutureTask(F&& future, SourceLocation source_location)
      : Task{source_location}, future_{InPlaceType<F>, std::move(future)} {}

  /// @copydoc Task::PollIsReady
  bool PollIsReady(PollContext& context) noexcept final {
    try {
      PollResult poll_result{future_.template As<F>().UnsafePoll(context)};
      if (poll_result.template Is<Pending>()) {
        return false;
      }
      result_ = std::move(poll_result);
    } catch (...) {
      exception_ = std::current_exception();
    }
    return true;
  }

  /// @copydoc Task::NotifyClosed
  void NotifyClosed() noexcept final {
    // Set `future` to `void` to indicate we're done (and to destroy all its data).
    future_.template Emplace<void>();
  }

  /// Returns the result of the task, throwing if it encountered an error.
  T Result() && {
    // Throw encountered exception, if any.
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    assert(future_.template Is<void>() &&
           "Future did not complete despite driving event loop to completion");
    return std::move(result_).template As<T>();
  }

 private:
  /// The underlying future to drive to completion. If `void`, the future completed.
  OneOf<F, void> future_;
  /// The result of the future. May still be `Pending` when `future_` is `void` if `exception_` is
  /// non-null.
  PollResult result_{InPlaceType<Pending>};
  /// A pointer to an exception encountered while executing the future. May be null.
  std::exception_ptr exception_;
};

template <class F>
typename F::Result EventLoop::RunFuture(F&& future,
                                        SourceLocation source_location) noexcept(false) {
  static_assert(std::is_same<F, std::decay_t<F>>::value,
                "RunFuture() must take ownership of `future`");

  RunFutureTask<F> task{std::forward<F>(future), source_location};
  task.Run(*this);
  return std::move(task).Result();
}

template <class F>
class EventLoop::SpawnFutureTask final : public Task {
 public:
  /// Constructs the task.
  SpawnFutureTask(F&& future, SourceLocation source_location)
      : Task{source_location}, future_{std::move(future)} {}

  /// @copydoc Task::PollIsReady
  bool PollIsReady(PollContext& context) noexcept final {
    try {
      return future_.UnsafePoll(context).template Is<void>();
    } catch (const std::exception& e) {
      // This API has no way to bubble up errors to the caller, unfortunately.
      // Ideally callers would use `Try()` to catch and handle errors, but this
      // seemingly did not happen here. Simply print the error message instead.
      Log("unhandled future exception at ", SourceLoc(), ": ", e.what());
      return true;  // "Ready" because the future failed.
    }
  }

  /// @copydoc Task::NotifyClosed
  void NotifyClosed() noexcept final {
    // Free `State` we allocated below (and released in `InvokeInLoop()`).
    std::unique_ptr<SpawnFutureTask>(this).reset();
  }

 private:
  /// The underlying future to drive to completion.
  F future_;
};

template <class F>
void EventLoop::SpawnFuture(F&& future, SourceLocation source_location) noexcept(false) {
  static_assert(std::is_same<F, std::decay_t<F>>::value,
                "SpawnFuture() must take ownership of `future`");

  std::unique_ptr<SpawnFutureTask<F>> task{
      std::make_unique<SpawnFutureTask<F>>(std::forward<F>(future), source_location)};

  State::InvokeInLoop(loop_, [task_ptr{std::move(task)}](EventLoop& loop) mutable noexcept {
    task_ptr.release()->Spawn(loop);
  });
}

// MARK: AwaitFuture()

class EventLoop::AwaitTask : public Task {
 public:
  /// @copydoc Task::Task
  using Task::Task;

  /// Non-copyable.
  AwaitTask(const AwaitTask&) = delete;
  /// Non-copyable.
  AwaitTask& operator=(const AwaitTask&) = delete;
  /// Non-moveable.
  AwaitTask(AwaitTask&&) = delete;
  /// Non-moveable.
  AwaitTask& operator=(AwaitTask&&) = delete;

  /// Returns whether the future completed.
  virtual bool FutureCompleted() noexcept = 0;

  /// @copydoc Task::NotifyClosed()
  void NotifyClosed() noexcept override { end_cv_.notify_one(); }

  /// Awaits the underlying future from the calling thread (which must be different from the
  /// event loop thread), awaiting its completion. The caller is responsible for checking that the
  /// future did (or did not) throw.
  bool Await(EventLoop& loop) noexcept(false);

 protected:
  /// Destroys the task.
  ~AwaitTask() noexcept = default;

  /// Returns a reference to the mutex used to synchronize operations between the caller and the
  /// event loop.
  constexpr std::mutex& Mutex() & noexcept { return mutex_; }

 private:
  /// @see Mutex()
  std::mutex mutex_;
  /// Condition variable used to notify the caller that the future has completed.
  std::condition_variable end_cv_;
};

template <class F>
class EventLoop::AwaitFutureTask final : public AwaitTask {
  /// Result of `F`.
  using T = typename F::Result;
  /// Result of `F::UnsafePoll()`.
  using PollResult = decltype(std::declval<F&>().UnsafePoll(std::declval<PollContext&>()));
  /// Alias of `Pending` (using `PollResult` to avoid a header include cycle).
  using Pending = typename PollResult::template TypeAt<1>;

 public:
  /// Constructs the task.
  AwaitFutureTask(F&& future, SourceLocation source_location)
      : AwaitTask{source_location}, future_{InPlaceType<F>, std::move(future)} {}

  /// @copydoc Task::PollIsReady
  bool PollIsReady(PollContext& context) noexcept final {
    try {
      PollResult poll_result{future_.template As<F>().UnsafePoll(context)};
      if (poll_result.template Is<Pending>()) {
        return false;
      }
      const std::unique_lock<std::mutex> lock{Mutex()};
      result_ = std::move(poll_result);
    } catch (...) {
      const std::unique_lock<std::mutex> lock{Mutex()};
      exception_ = std::current_exception();
    }
    return true;
  }

  /// @copydoc Task::NotifyClosed
  void NotifyClosed() noexcept final {
    // Set `future` to `void` to indicate we're done (and to destroy all its data).
    const std::unique_lock<std::mutex> lock{Mutex()};
    future_.template Emplace<void>();
    AwaitTask::NotifyClosed();
  }

  /// @copydoc AwaitTask::FutureCompleted
  bool FutureCompleted() noexcept final { return future_.template Is<void>(); }

  /// Returns the result of the task, throwing if it encountered an error.
  T Result() && {
    const std::unique_lock<std::mutex> lock{Mutex()};
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return std::move(result_).template As<T>();
  }

 private:
  /// The underlying future to drive to completion. If `void`, the future completed.
  OneOf<F, void> future_;
  /// The result of the future. May still be `Pending` when `future_` is `void` if `exception_` is
  /// non-null.
  PollResult result_{InPlaceType<Pending>};
  /// A pointer to an exception encountered while executing the future. May be null.
  std::exception_ptr exception_;
};

template <class F>
typename F::Result EventLoop::AwaitFuture(F&& future,
                                          SourceLocation source_location) noexcept(false) {
  static_assert(std::is_same<F, std::decay_t<F>>::value,
                "AwaitFuture() must take ownership of `future`");

  AwaitFutureTask<F> task{std::forward<F>(future), source_location};
  task.Await(*this);
  return std::move(task).Result();
}

// MARK: Invoker

class EventLoop::Invoker final {
 public:
  /// Creates an `Invoker` which can be used to later invoke functions in the context of the
  /// `EventLoop`.
  explicit Invoker(EventLoop& loop) noexcept : loop_ptr_{loop.InternalState()} {}

  /// Schedules for `invocable()` to be invoked by the event loop soon. If the event loop is no
  /// longer available, discards `invocable` and returns false. If called on the event loop,
  /// immediately executes `invocable()`.
  ///
  /// @throws std::bad_alloc If the invocation state cannot be allocated.
  template <class F, std::enable_if_t<std::is_void<decltype(std::declval<F&>()(
                         std::declval<EventLoop&>()))>::value>* = nullptr>
  bool TryInvoke(F&& invocable) const noexcept(false) {
    const std::shared_ptr<State> loop_ptr{loop_ptr_.lock()};
    if (loop_ptr == nullptr) {
      return false;
    }
    State::InvokeInLoop(loop_ptr, std::forward<F>(invocable));
    return true;
  }

  /// Same as `TryInvoke()`, but only moves `invocable` if it can be invoked.
  ///
  /// @throws std::bad_alloc If the invocation state cannot be allocated.
  template <class F, std::enable_if_t<std::is_void<decltype(std::declval<F&>()(
                         std::declval<EventLoop&>()))>::value>* = nullptr>
  bool TryInvoke(F& invocable) const noexcept(false) {
    const std::shared_ptr<State> loop_ptr{loop_ptr_.lock()};
    if (loop_ptr == nullptr) {
      return false;
    }
    State::InvokeInLoop(loop_ptr, std::move(invocable));
    return true;
  }

 private:
  /// A weak reference to the event loop. This may be expired, but if `lock()` returns a shared
  /// pointer, then the loop it points to is guaranteed to be non-null.
  std::weak_ptr<State> loop_ptr_;
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_EVENT_LOOP_EVENT_LOOP_H_
