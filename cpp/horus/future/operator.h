/// @file
///
/// The `FutureOperator` class.

#ifndef HORUS_FUTURE_OPERATOR_H_
#define HORUS_FUTURE_OPERATOR_H_

#include <cstddef>
#include <tuple>
#include <utility>

#include "horus/future/info.h"

namespace horus {

/// An "operator object" which, given an input `Future`, yields an output `Future`.
template <template <class...> class OutputFuture, class Args, class... Ts>
class FutureOperator;

template <template <class...> class OutputFuture, class... Args, class... Ts>
class FutureOperator<OutputFuture, std::tuple<Args...>, Ts...> final {
 public:
  /// Constructs an operator which will, given any `InputFuture`, construct and return an
  /// `OutputFuture<InputFuture, Ts...>`
  constexpr explicit FutureOperator(Args&&... args, const FutureInfo& info) noexcept
      : args_{std::move(args)...}, info_{info} {}

  /// Returns an `OutputFuture<Future, Ts...>` constructed with the given future and
  /// previously-specified arguments.
  template <class Future>
  constexpr auto operator()(Future&& future) noexcept {
    return Apply(std::forward<Future>(future), std::make_index_sequence<sizeof...(Ts)>{});
  }

 private:
  /// Implementation of `operator()()` which uses `std::index_sequence` to extract all tuple values.
  template <class Future, std::size_t... Indices>
  constexpr auto Apply(Future&& future, std::index_sequence<Indices...> /* indices */) noexcept {
    return OutputFuture<Future, Ts...>(std::forward<Future>(future),
                                       std::forward<Args>(std::get<Indices>(args_))..., info_);
  }

  /// The arguments to give to the `OutputFuture` constructor.
  std::tuple<Args...> args_;
  /// The location where the operator was created.
  FutureInfo info_;
};

/// Specialization of `FutureOperator` used when `Args` is `Ts`.
template <template <class...> class OutputFuture, class... Ts>
class FutureOperator<OutputFuture, void, Ts...> final {
 public:
  /// Constructs an operator which will, given any `InputFuture`, construct and return an
  /// `OutputFuture<InputFuture, Ts...>`
  constexpr explicit FutureOperator(Ts&&... args, const FutureInfo& info) noexcept
      : inner_{std::move(args)..., info} {}

  /// Returns an `OutputFuture<Future, Ts...>` constructed with the given future and
  /// previously-specified arguments.
  template <class Future>
  constexpr auto operator()(Future&& future) noexcept {
    return inner_(std::forward<Future>(future));
  }

 private:
  /// Inner operator.
  FutureOperator<OutputFuture, std::tuple<Ts...>, Ts...> inner_;
};

}  // namespace horus

#endif  // HORUS_FUTURE_OPERATOR_H_
