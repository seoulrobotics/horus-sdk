/// @file
///
/// The `Pipe()` function.

#ifndef HORUS_FUTURE_PIPE_H_
#define HORUS_FUTURE_PIPE_H_

#include <utility>

namespace horus {
namespace horus_internal {

/// Specialization-based implementation of `Pipe()`.
template <class Future, class... Ops>
struct PipeBuilder;

/// Specialization of `PipeBuilder` when no operator is left.
template <class Future>
struct PipeBuilder<Future> {
  /// Return type of `Pipe()`.
  using type = Future;

  /// Returns `future`.
  static constexpr Future&& Pipe(Future&& future) noexcept { return std::move(future); }
};

/// Recursive specialization of `PipeBuilder` with an operator `Op0`.
template <class Future, class Op0, class... OpN>
struct PipeBuilder<Future, Op0, OpN...> {
  /// `PipeBuilder` which takes the future produced by applying `Op0` to `Future`.
  using InnerBuilder = PipeBuilder<decltype(std::declval<Op0>()(std::declval<Future>())), OpN...>;

  /// Return type of `Pipe()`.
  using type = typename InnerBuilder::type;

  /// Returns `InnerBuilder::Pipe(op_0(future), op_n...)`.
  static constexpr type Pipe(Future&& future, Op0&& op_0, OpN&&... op_n) noexcept(noexcept(
      InnerBuilder::Pipe(std::move(op_0)(std::move(future)), std::forward<OpN>(op_n)...))) {
    return InnerBuilder::Pipe(std::move(op_0)(std::move(future)), std::forward<OpN>(op_n)...);
  }
};

}  // namespace horus_internal

/// The future resulting by `Pipe()`.
template <class Future, class... Ops>
using PipeResult = typename horus_internal::PipeBuilder<Future, Ops...>::type;

/// Returns `op_n(op_n-1(...op_0(future)))`.
///
/// This can be used to reduce nesting of future-building expressions. For instance,
///
///   `Map(Then(Then(Future, Fn1), Fn2), Fn3)`
///
/// can be better expressed as:
///
///   `Pipe(Future, Then(Fn1), Then(Fn2), Map(Fn3))`
template <class Future, class... Ops>
constexpr PipeResult<Future, Ops...> Pipe(Future&& future, Ops&&... ops) noexcept(
    noexcept(horus_internal::PipeBuilder<Future, Ops...>::Pipe(std::forward<Future>(future),
                                                               std::forward<Ops>(ops)...))) {
  return horus_internal::PipeBuilder<Future, Ops...>::Pipe(std::forward<Future>(future),
                                                           std::forward<Ops>(ops)...);
}

}  // namespace horus

#endif  // HORUS_FUTURE_PIPE_H_
