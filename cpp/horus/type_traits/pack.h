/// @file
///
/// Type traits for working with parameter packs.

#ifndef HORUS_TYPE_TRAITS_PACK_H_
#define HORUS_TYPE_TRAITS_PACK_H_

namespace horus {
namespace type_traits_internal {

/// Defines `Type` to be `IfPresent` if `Needle` is in `Haystack...`, and `IfAbsent` otherwise.
template <class IfPresent, class IfAbsent, class Needle, class... Haystack>
struct FindThenImpl;
template <class IfPresent, class IfAbsent, class Needle>
struct FindThenImpl<IfPresent, IfAbsent, Needle> {  // Base case: `Needle` not found.
  using Type = IfAbsent;
};
template <class IfPresent, class IfAbsent, class Needle, class... Haystack>
struct FindThenImpl<IfPresent, IfAbsent, Needle, Needle, Haystack...> {  // Base case: `Needle`
                                                                         // found.
  using Type = IfPresent;
};
template <class IfPresent, class IfAbsent, class Needle, class NotNeedle, class... Haystack>
struct FindThenImpl<IfPresent, IfAbsent, Needle, NotNeedle, Haystack...>  // Recursive case.
    : FindThenImpl<IfPresent, IfAbsent, Needle, Haystack...> {};

template <class IfPresent, class IfAbsent, class Needle, class... Haystack>
using FindThen = typename FindThenImpl<IfPresent, IfAbsent, Needle, Haystack...>::Type;

/// Marker type used by `ApplyUniqueImpl` which "stores" the deduplicates types in a parameter pack.
template <class...>
struct Dedup {};

/// Implementation of `ApplyUnique`: deduplicates `Ts...` into `Dedup`, and once `Ts` is empty we
/// evaluate `A<Us...>`.
template <template <class...> class A, class Dedup, class... Ts>
struct ApplyUniqueImpl;
template <template <class...> class A, class... Us>
struct ApplyUniqueImpl<A, Dedup<Us...>> {  // Base case: evaluate `A<Us...>`.
  using Type = A<Us...>;
};
template <template <class...> class A, class... Us, class T, class... Ts>
struct ApplyUniqueImpl<A, Dedup<Us...>, T, Ts...>  // Recursive case.
    : ApplyUniqueImpl<A,
                      FindThen</*IfPresent=*/Dedup<Us...>, /*IfAbsent=*/Dedup<Us..., T>, T, Us...>,
                      Ts...> {};

}  // namespace type_traits_internal

/// Evaluates to `A<Us...>` with `Us...` being the unique (deduplicated) types in `Ts...`.
template <template <class...> class A, class... Ts>
using ApplyUnique =
    typename type_traits_internal::ApplyUniqueImpl<A, type_traits_internal::Dedup<>, Ts...>::Type;

}  // namespace horus

#endif  // HORUS_TYPE_TRAITS_PACK_H_
