/// @file
///
/// Helpers for manipulating tuples.

#ifndef HORUS_INTERNAL_TUPLE_H_
#define HORUS_INTERNAL_TUPLE_H_

#include <cstddef>
#include <type_traits>
#include <utility>

namespace horus {
namespace horus_internal {

/// Base recursive case of `ForEachIndexOfImpl()`
template <std::size_t Index>
constexpr void ForEachIndexOfImpl() noexcept {}

/// Calls `invocable(std::integral_constant<std::size_t, I>)` for all invocables.
template <std::size_t Index, class Invocable, class... Rest>
void ForEachIndexOfImpl(Invocable&& invocable, Rest&&... rest) noexcept(
    noexcept(std::forward<Invocable>(invocable)(
        std::integral_constant<std::size_t, Index>{})) && noexcept(ForEachIndexOfImpl<Index +
                                                                                      1>(
        std::forward<Rest>(rest)...))) {
  std::forward<Invocable>(invocable)(std::integral_constant<std::size_t, Index>{});
  ForEachIndexOfImpl<Index + 1>(std::forward<Rest>(rest)...);
}

/// Base recursive case of `ForEachIndexImpl()`
template <class Invocable>
void ForEachIndexImpl(Invocable&& invocable, std::index_sequence<> /* indices */) noexcept {
  const Invocable forwarded{std::forward<Invocable>(invocable)};
}

/// Invokes `invocable(std::integral_constant<std::size_t, I>{})` for each invocable in
/// `invocables`.
template <class... Invocables>
void ForEachIndexOf(Invocables&&... invocables) noexcept(
    noexcept(ForEachIndexOfImpl<0>(std::forward<Invocables>(invocables)...))) {
  ForEachIndexOfImpl<0>(std::forward<Invocables>(invocables)...);
}

template <class T, std::size_t Ignored>
using IgnoreSecond = T;

/// Calls `invocable(std::integral_constant<std::size_t, I>)` for all `Indices`.
template <class Invocable, std::size_t... Indices>
void ForEachIndexToImpl(
    Invocable&& invocable,
    std::index_sequence<
        Indices...> /* indices */) noexcept(noexcept(ForEachIndexOf(std::
                                                                        forward<IgnoreSecond<
                                                                            Invocable, Indices>>(
                                                                            invocable)...))) {
  ForEachIndexOf(std::forward<IgnoreSecond<Invocable, Indices>>(invocable)...);
}

/// Invokes `invocable(std::integral_constant<std::size_t, I>{})` with `0 <= I < N`.
template <std::size_t N, class Invocable>
void ForEachIndexTo(Invocable&& invocable) noexcept(noexcept(
    ForEachIndexToImpl(std::forward<Invocable>(invocable), std::make_index_sequence<N>{}))) {
  ForEachIndexToImpl(std::forward<Invocable>(invocable), std::make_index_sequence<N>{});
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_TUPLE_H_
