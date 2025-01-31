/// @file
///
/// Type traits used in Horus.

#ifndef HORUS_INTERNAL_TYPE_TRAITS_H_
#define HORUS_INTERNAL_TYPE_TRAITS_H_

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace horus {
namespace horus_internal {

/// See https://en.cppreference.com/w/cpp/types/conjunction
template <class... Ts>
struct Conjunction : std::true_type {};
template <class T>
struct Conjunction<T> : T {};
template <class T, class... Ts>
struct Conjunction<T, Ts...> : std::conditional_t<T::value, Conjunction<Ts...>, T> {};

/// Returns true if all the given `booleans` are true. If `booleans` is empty, returns true.
constexpr bool ConjunctionOf(std::initializer_list<bool> booleans) noexcept {
  for (const bool value : booleans) {  // NOLINT(*-use-anyofallof): not constexpr
    if (!value) {
      return false;
    }
  }
  return true;
}

template <std::size_t I, class... Ts>
struct TypeAt;
template <class T, class... Rest>
struct TypeAt<0, T, Rest...> {
  using type = T;
};
template <std::size_t I, class T, class... Rest>
struct TypeAt<I, T, Rest...> : TypeAt<I - 1, Rest...> {};

template <std::size_t I, class... Ts>
using TypeAtT = typename TypeAt<I, Ts...>::type;

/// Returns the index of `T` in `Ts`, causing a compile-time error if it cannot be found.
template <std::size_t I, class T, class... Ts>
struct PackIndexOf;
template <std::size_t I, class T, class... Rest>
struct PackIndexOf<I, T, T, Rest...> : std::integral_constant<std::size_t, I> {};
template <std::size_t I, class T, class U, class... Rest>
struct PackIndexOf<I, T, U, Rest...> : PackIndexOf<I + 1, T, Rest...> {};

/// Value used by `UniquePackIndexOf` to represent an unknown index.
static constexpr std::size_t kNotFound{-1UL};

/// Returns the index of `T` in `Ts`, causing a compile-time error if it cannot be found exactly
/// once.
template <std::size_t kFound, std::size_t I, class T, class... Ts>
struct UniquePackIndexOf;

template <std::size_t I, class T>
struct UniquePackIndexOf<kNotFound, I, T> {
  static_assert(!std::is_same<T, T>::value, "type T is not in parameter pack Ts...");
};
template <std::size_t kFound, std::size_t I, class T, class... Rest>
struct UniquePackIndexOf<kFound, I, T, T, Rest...> {
  static_assert(!std::is_same<T, T>::value, "type T found more than once in parameter pack Ts...");
};
template <std::size_t kFound, std::size_t I, class T>
struct UniquePackIndexOf<kFound, I, T> : std::integral_constant<std::size_t, kFound> {};

template <std::size_t I, class T, class... Rest>
struct UniquePackIndexOf<kNotFound, I, T, T, Rest...> : UniquePackIndexOf<I, I + 1, T, Rest...> {};
template <std::size_t kFound, std::size_t I, class T, class U, class... Rest>
struct UniquePackIndexOf<kFound, I, T, U, Rest...> : UniquePackIndexOf<kFound, I + 1, T, Rest...> {
};

static_assert(std::is_same<TypeAtT<0, bool, float>, bool>::value, "");
static_assert(std::is_same<TypeAtT<1, bool, float>, float>::value, "");

/// Defines `Type = T`, ignoring @p Ignored....
///
/// Used in two scenarios:
///
///  1. To prevent C++ from attempting to infer @p T.
///  2. To make @p T dependent on @p Ignored....
template <class T, class... Ignored>
struct TypeIdentity {
  /// Evaluates to @p T.
  using Type = T;
};

/// Evaluates to @p T.
///
/// @see TypeIdentity
template <class T>
using TypeIdentityT = typename TypeIdentity<T>::Type;

/// Evaluates to @p T.
///
/// @see TypeIdentity
template <class T, class... Ignored>
using ConstantTypeT = typename TypeIdentity<T, Ignored...>::Type;

/// Evaluates to void.
///
/// @see TypeIdentity
template <class... Ts>
using VoidT = typename TypeIdentity<void, Ts...>::Type;

/// Implementation of IsDetected and other utilities.
///
/// The default specialization of Detector defines ValueT as std::false_type
/// and Type as @p Default, indicating a failure to instantiate `Op<Args...>`.
///
/// @see https://en.cppreference.com/w/cpp/experimental/is_detected
template <class Default, class Enable, template <class...> class Op, class... Args>
struct Detector {
  using ValueT = std::false_type;
  using Type = Default;
};

/// Successful specialization of Detector indicating that `Op<Args...>` was
/// successfully instantiated, therefore defining ValueT as std::true_type and Type as Op<Args...>.
template <class Default, template <class...> class Op, class... Args>
struct Detector<Default, VoidT<Op<Args...>>, Op, Args...> {
  using ValueT = std::true_type;
  using Type = Op<Args...>;
};

/// Marker type used by IsDetected to indicate that no type matched in the default case.
///
/// @see https://en.cppreference.com/w/cpp/experimental/nonesuch
struct Nonesuch {
  ~Nonesuch() = delete;
  Nonesuch(const Nonesuch&) = delete;
  void operator=(const Nonesuch&) = delete;
  Nonesuch(Nonesuch&&) = delete;
  Nonesuch& operator=(Nonesuch&&) = delete;
};

/// Evaluates to std::true_type if `Op<Args...>` is a valid template instantiation, and to
/// std::false_type otherwise.
///
/// @see https://en.cppreference.com/w/cpp/experimental/is_detected
template <template <class...> class Op, class... Args>
using IsDetected = typename Detector<Nonesuch, void, Op, Args...>::ValueT;

/// Defines `Type = Op<Args...>` and `ValueT = std::true_type` if `Op<Args...>` is a valid template
/// instantiation, and `Type = nonesuch` and `ValueT = std::false_type` otherwise.
///
/// @see https://en.cppreference.com/w/cpp/experimental/is_detected
template <template <class...> class Op, class... Args>
using Detected = Detector<Nonesuch, void, Op, Args...>;

/// Evaluates to `Op<Args...>` if it is a valid template instantiation, and Nonesuch otherwise.
///
/// @see https://en.cppreference.com/w/cpp/experimental/is_detected
template <template <class...> class Op, class... Args>
using DetectedT = typename Detector<Nonesuch, void, Op, Args...>::Type;

/// Evaluates to std::true_type if `Op<Args...>` is a valid template instantiation and is exactly
/// the same as @p Expected, and std::false_type otherwise.
///
/// @see https://en.cppreference.com/w/cpp/experimental/is_detected
template <class Expected, template <class...> class Op, class... Args>
using IsDetectedExact = std::is_same<Expected, DetectedT<Op, Args...>>;

/// Evaluates to the result of `F(Args...)`.
template <class F, class... Args>
using InvokeResultT = decltype(std::declval<F>()(std::declval<Args>()...));

/// Defines `value` to be true if `F(Args...)` is a valid function call.
template <class F, class... Args>
using IsInvocable = IsDetected<InvokeResultT, F, Args...>;

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_TYPE_TRAITS_H_
