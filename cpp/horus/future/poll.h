/// @file
///
/// Structures related to polling in `Future`s.

#ifndef HORUS_FUTURE_POLL_H_
#define HORUS_FUTURE_POLL_H_

#include <type_traits>
#include <utility>

#include "horus/attributes.h"
#include "horus/event_loop/event_loop.h"
#include "horus/event_loop/waker.h"
#include "horus/future/core_owner.h"
#include "horus/future/info.h"
#include "horus/pointer/unsafe_cast.h"
#include "horus/strings/string_view.h"
#include "horus/types/one_of.h"

namespace horus {

// MARK: PollContext

/// A context object given to `Future::UnsafePoll()`.
///
/// A context provides access to the different owners of the future, all of which have a different
/// lifetimes:
///
/// - The `EventLoop`, which drives the overall future to completion and has the greatest lifetime.
///
/// - The `FutureWaker`, which is used to wake up an entire future tree from its root. A future tree
///   always has a single waker corresponding to a call to `EventLoop::RunFuture()` or
///   `EventLoop::AwaitFuture()`.
///
/// - The `CoreFutureOwner`, which owns a future and all its resources, which it can destroy
///   asynchronously. A future tree can have subtrees with different owners using `FutureOwner<F>`.
class PollContext final {
 public:
  /// An object used to trace the execution of a future.
  class Trace;

  /// Initializes a new `PollContext`.
  PollContext(horus_internal::EventLoop& loop HORUS_LIFETIME_BOUND,
              horus_internal::CoreFutureOwner& owner HORUS_LIFETIME_BOUND,
              horus_internal::FutureWaker&& waker) noexcept
      : loop_{loop},
        future_owner_{owner},
        waker_{std::move(waker)},
        trace_{nullptr},
        trace_receiver_{nullptr} {}

  /// Initializes a new `PollContext` which traces future execution using the provided function,
  /// which must outlive the context.
  ///
  /// When a future is entered, `trace(const char*, const FutureInfo*)` is called with the name and
  /// information of the future. When it is left, it is called again with `nullptr` arguments.
  template <class F>
  PollContext(horus_internal::EventLoop& loop HORUS_LIFETIME_BOUND,
              horus_internal::CoreFutureOwner& owner HORUS_LIFETIME_BOUND,
              horus_internal::FutureWaker&& waker, F& trace HORUS_LIFETIME_BOUND) noexcept
      : loop_{loop},
        future_owner_{owner},
        waker_{std::move(waker)},
        trace_{[](void* receiver, const char* type, const FutureInfo* info) {
          (*UnsafePointerCast<F>(receiver))(type, info);
        }},
        trace_receiver_{&trace} {}

  /// A `PollContext` can neither be copied nor moved.
  PollContext(const PollContext&) = delete;
  /// A `PollContext` can neither be copied nor moved.
  PollContext& operator=(const PollContext&) = delete;
  /// A `PollContext` can neither be copied nor moved.
  PollContext(PollContext&&) = delete;
  /// A `PollContext` can neither be copied nor moved.
  PollContext& operator=(PollContext&&) = delete;

  /// Destroys the `PollContext`, closing all the handles it manages.
  ~PollContext() noexcept = default;

  /// Returns a reference to the `EventLoop` polling this future.
  horus_internal::EventLoop& Loop() noexcept { return loop_; }

  /// Returns a reference to the owner of the current future.
  horus_internal::CoreFutureOwner& FutureOwner() noexcept { return future_owner_; }

  /// Returns a copy of the `FutureWaker` for this context. This copy may outlive this
  /// `PollContext`, but not the future being executed. It may also be used as a callback given to
  /// the `Handle()`.
  horus_internal::FutureWaker Waker() const { return waker_; }

 private:
  /// @see Loop()
  horus_internal::EventLoop& loop_;
  /// @see Handles()
  horus_internal::CoreFutureOwner& future_owner_;
  /// @see Waker()
  horus_internal::FutureWaker waker_;

  /// Function called to trace future execution in this context. May be null.
  void (*trace_)(void* receiver, const char* type, const FutureInfo* info);
  /// Receiver given to `trace_`. May be null.
  void* trace_receiver_;
};

class PollContext::Trace final {
 public:
  /// Creates a RAII future trace.
  Trace(PollContext& context, const char* type,
        const FutureInfo& info HORUS_LIFETIME_BOUND) noexcept;

  /// Cannot move or copy.
  Trace(const Trace&) = delete;
  /// Cannot move or copy.
  Trace& operator=(const Trace&) = delete;
  /// Cannot move or copy.
  Trace(Trace&&) = delete;
  /// Cannot move or copy.
  Trace& operator=(Trace&&) = delete;

  /// Destroys the RAII future trace.
  ~Trace() noexcept;

  /// The trace which was active when this trace was created, if any.
  constexpr Trace* Parent() const noexcept { return parent_; }
  /// The type of the future being traced.
  constexpr StringView FutureType() const noexcept { return type_; }
  /// Information about the future being traced.
  constexpr const FutureInfo& FutureInformation() const noexcept { return info_; }

 private:
  /// The parent of the trace, if any.
  Trace* parent_;
  /// The context of the trace.
  PollContext& context_;

  /// @see FutureType(0)
  const char* type_;
  /// @see FutureInformation()
  const FutureInfo& info_;
};

/// Prints the stack trace of the current future to stderr, if any.
extern void PrintFutureStackTrace();

// MARK: PollResult

/// Marker type used in `PollResult<T>` to indicate a `Future` is not yet ready.
///
/// Ideally this would be defined in `future.h`, but to break the dependency cycle this is defined
/// here instead.
struct Pending {};

/// The result of polling a `Future`.
template <class T>
using PollResult = OneOf<T, Pending>;

/// Returns a ready `PollResult`.
template <class T, class... Args>
inline PollResult<T> ReadyResult(Args&&... args) noexcept(
    std::is_nothrow_constructible<T, Args...>::value) {
  return PollResult<T>{InPlaceType<T>, std::forward<Args>(args)...};
}

/// Returns a `Pending` `PollResult`.
template <class T>
inline PollResult<T> PendingResult() noexcept {
  return PollResult<T>{InPlaceType<Pending>};
}

}  // namespace horus

#endif  // HORUS_FUTURE_POLL_H_
