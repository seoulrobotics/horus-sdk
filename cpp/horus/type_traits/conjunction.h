/// @file
///
/// Type-level conjunction helpers.

#ifndef HORUS_TYPE_TRAITS_CONJUNCTION_H_
#define HORUS_TYPE_TRAITS_CONJUNCTION_H_

#include <initializer_list>
#include <type_traits>

namespace horus {

/// See https://en.cppreference.com/w/cpp/types/conjunction
template <class... Ts>
struct Conjunction : std::true_type {};
template <class T>
struct Conjunction<T> : T {};
template <class T, class... Ts>
struct Conjunction<T, Ts...> : std::conditional_t<T::value, Conjunction<Ts...>, T> {};

/// Returns true if all the given `booleans` are true. If `booleans` is empty, returns true.
constexpr bool ConjunctionOf(std::initializer_list<bool> booleans) noexcept {
  for (bool const value : booleans) {  // NOLINT(*-use-anyofallof): not constexpr
    if (!value) {
      return false;
    }
  }
  return true;
}

}  // namespace horus

#endif  // HORUS_TYPE_TRAITS_CONJUNCTION_H_
