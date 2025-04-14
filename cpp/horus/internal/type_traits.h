/// @file
///
/// Type traits used in Horus.

#ifndef HORUS_INTERNAL_TYPE_TRAITS_H_
#define HORUS_INTERNAL_TYPE_TRAITS_H_

#include <type_traits>
#include <utility>

namespace horus {
namespace horus_internal {

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
