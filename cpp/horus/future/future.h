/// @file
///
/// The `Future<T>` class and related symbols.

#ifndef HORUS_FUTURE_FUTURE_H_
#define HORUS_FUTURE_FUTURE_H_

#include <stdexcept>
#include <type_traits>

#include "horus/future/poll.h"

namespace horus {

// MARK: Future<T>

/// Defines a value which will resolve to either a value of type `T` or to an exception in the
/// future.
///
/// This abstract class is inspired by the
/// [Future](https://doc.rust-lang.org/stable/std/future/trait.Future.html) trait in Rust.
template <class T>
class Future {
 public:
  /// The `UnsafePoll()` context type.
  using Context = PollContext;
  /// The resulting type.
  using Result = T;

  /// Futures cannot be copied.
  Future(const Future&) = delete;
  /// Futures cannot be copied.
  Future& operator=(const Future&) = delete;

  /// Some futures may be moved.
  Future(Future&&) noexcept = default;
  /// Some futures may be moved.
  Future& operator=(Future&&) noexcept = default;

  /// Futures may be stored in a `std::unique_ptr<Future<T>>`, so their destructor is not
  /// necessarily known.
  virtual ~Future() = default;

  /// Polls the future; if it has completed, it will return a `PollResult` storing `T`. Otherwise,
  /// it will return `Pending`.
  ///
  /// The future may request to be polled again in the future by calling `context.Waker().Wake()`.
  ///
  /// `UnsafePoll()` may throw, which indicates that the future cannot complete (that is, if
  /// `UnsafePoll()` throws, it should not be called again).
  ///
  /// This function is explicitly "unsafe" to call as two invariants must be enforced by the caller
  /// before calling this function:
  ///
  /// 1. This function should always be called from the same thread, with access to the same
  ///    `Waker`, running in the same `EventLoop`.
  ///
  /// 2. Once polled, the `Future` may not be moved until it is destroyed.
  ///
  /// The event loop enforces these invariants when executing futures.
  virtual PollResult<T> UnsafePoll(PollContext& context) = 0;

 protected:
  /// Returns `future.UnsafePoll(context)`.
  ///
  /// This function is not annotated as "unsafe" as it can only be called by futures; it is expected
  /// that futures will call this function within `UnsafePoll()`, and if the invariants necessary
  /// for the caller `UnsafePoll()` are guaranteed in the caller, then they are also likely
  /// guaranteed for this function.
  ///
  /// This function may be used instead of `future.UnsafePoll(context)` when working with concrete
  /// future types not known by the compiler to be `Future`s. Notably, the compiler will provide
  /// better error messages since it can see that the first argument is expected to be a
  /// `Future<U>`.
  template <class U>
  static PollResult<U> PollFuture(Future<U>& future, PollContext& context) {
    return future.UnsafePoll(context);
  }

  /// Default constructor for derived classes.
  Future() noexcept = default;
};

namespace horus_internal {

/// Evaluates to `void`.
template <class>
using IsFutureVoidT = void;
/// Implementation of `IsFuture<F>`.
template <class F, class Enable = void>
struct IsFuture : std::false_type {};
template <class F>
struct IsFuture<F, IsFutureVoidT<typename F::Result>>
    : std::is_base_of<Future<typename F::Result>, F> {};

}  // namespace horus_internal

/// Defines `value` to be true if `F` is a `Future`.
template <class F>
using IsFuture = horus_internal::IsFuture<F>;

/// Evaluates to `T` given `F = Future<T>`.
template <class F>
using FutureResult = std::enable_if_t<IsFuture<F>::value, typename F::Result>;

/// Error potentially thrown by `Future::UnsafePoll()` when a future is polled after returning a
/// non-`Pending` value.
class PolledAfterCompletionError final : public std::logic_error {
 public:
  /// Constructs an error with default error message.
  PolledAfterCompletionError() : std::logic_error{"future polled after completing"} {}
};

/// Returns `pipe_operator(future)`.
///
/// This can be used to reduce nesting of future-building expressions. For instance,
///
///   `Map(Then(Then(Future, Fn1), Fn2), Fn3)`
///
/// can be better expressed as:
///
///   `Future | Then(Fn1) | Then(Fn2) | Map(Fn3)`
template <class Future, class Op, std::enable_if_t<IsFuture<Future>::value>* = nullptr>
constexpr auto operator|(Future&& future, Op&& pipe_operator) noexcept(
    noexcept(std::declval<Op>()(std::declval<Future>())))
    -> decltype(std::declval<Op>()(std::declval<Future>())) {
  return std::forward<Op>(pipe_operator)(std::forward<Future>(future));
}

}  // namespace horus

#endif  // HORUS_FUTURE_FUTURE_H_
