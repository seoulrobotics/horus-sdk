/// @file
///
/// The `OneOf` type.

#ifndef HORUS_TYPES_ONE_OF_H_
#define HORUS_TYPES_ONE_OF_H_

#include <cassert>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "horus/attributes.h"
#include "horus/optimization.h"
#include "horus/strings/fallback.h"
#include "horus/strings/string_view.h"
#include "horus/type_traits/conjunction.h"
#include "horus/type_traits/pack.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of_tag.h"
#include "horus/types/union.h"

// NOLINTBEGIN(*-macro-usage): cannot be functions

/// Switches over a `OneOf<Ts...>`.
///
/// This macros must be followed by a block of `HORUS_ONEOF_CASE`s and, if needed, a
/// `HORUS_ONEOF_DEFAULT`.
///
/// Usage:
///
///   OneOf<std::int32_t, std::string> one_of{42};
///
///   HORUS_ONEOF_SWITCH(one_of) {
///     HORUS_ONEOF_CASE(value, std::int32_t) {
///       // `value` is `std::int32_t&`.
///     }
///     HORUS_ONEOF_CASE(string, std::string) {
///       // `string` is `std::string&`.
///     }
///   }
///
/// If all cases are handled, a `HORUS_ONEOF_DEFAULT` is not needed. However, GCC does not know
/// that `OneOf` tags are always valid enum values, so it will consider that a `HORUS_ONEOF_SWITCH`
/// without a default case is not exhaustive, which can lead to diagnostics like "non-void function
/// does not return a value in all control paths". There is currently no good way to suppress this
/// warning.
///
/// @note This macro is implemented using a `switch`, so `break`ing within one of the cases will not
/// `break` from a loop, but instead exit the `switch` block.
#define HORUS_ONEOF_SWITCH(ONE_OF)      \
  auto& _horus_switch_oneof = (ONE_OF); \
  switch (::horus::one_of_internal::TagOf(_horus_switch_oneof))

/// Declares a case in a `HORUS_ONEOF_SWITCH` without binding it to a variable.
///
/// `...` is the type being matched (it uses an ellipsis in case that type has a comma in it).
#define HORUS_ONEOF_CASE_DISCARD(...)                                      \
  break;                                                                   \
  case ::horus::one_of_internal::TagFor<std::remove_const_t<__VA_ARGS__>>( \
      static_cast<std::add_pointer_t<decltype(_horus_switch_oneof)>>(nullptr)):

/// Declares a case in a `HORUS_ONEOF_SWITCH`.
///
/// `VARIABLE` is the name of the variable which will be bound to the inner value of the `OneOf`,
/// `...` is the type being matched (it uses an ellipsis in case that type has a comma in it).
///
/// In C++14 and earlier, compilers will not be able to determine that the following block is
/// always executed, so code like the following will fail to compile due to "not returning a value
/// in all control paths":
///
///   std::int32_t ToInt(OneOf<std::int32_t, bool> const& one_of) {
///     HORUS_ONEOF_SWITCH(one_of) {
///       HORUS_ONEOF_CASE(value, std::int32_t) { return value; }
///       HORUS_ONEOF_CASE(value, bool) { return value ? 1 : 0; }
///     }
///   }
///
/// `HORUS_ONEOF_RETURN_NOT_HANDLED` can be used to throw an exception if the default case is not
/// handled in such cases.
#if __cplusplus >= 201703L
#define HORUS_ONEOF_CASE(VARIABLE, ...)                                                   \
  HORUS_ONEOF_CASE_DISCARD(__VA_ARGS__)                                                   \
  if (::horus::one_of_internal::PropagateConst<__VA_ARGS__,                               \
                                               decltype(_horus_switch_oneof)>& VARIABLE = \
          ::horus::one_of_internal::UnsafeAs<std::remove_const_t<__VA_ARGS__>>(           \
              _horus_switch_oneof);                                                       \
      true)
#else
#define HORUS_ONEOF_CASE(VARIABLE, ...)                                            \
  HORUS_ONEOF_CASE_DISCARD(__VA_ARGS__)                                            \
  for (::horus::one_of_internal::PropagateConst<                                   \
           __VA_ARGS__,                                                            \
           decltype(_horus_switch_oneof)>& VARIABLE =                              \
           ::horus::one_of_internal::UnsafeAs<std::remove_const_t<__VA_ARGS__>>(   \
               _horus_switch_oneof),                                               \
                                           *VARIABLE##_horus_loop_ptr = &VARIABLE; \
       VARIABLE##_horus_loop_ptr != nullptr;                                       \
       VARIABLE##_horus_loop_ptr = nullptr)
#endif

/// Declares a default case in a `HORUS_ONEOF_SWITCH`.
///
/// Default cases are optional, and, if specified, must always appear at the end of a
/// `HORUS_ONEOF_SWITCH` block.
#define HORUS_ONEOF_DEFAULT \
  break;                    \
  default:

/// Declares a default case in a `HORUS_ONEOF_SWITCH` which throws a `std::logic_error`.
#define HORUS_ONEOF_DEFAULT_NOT_HANDLED     \
  break;                                    \
  default:                                  \
    static_cast<void>(_horus_switch_oneof); \
    ::horus::one_of_internal::OneOfCaseNotHandled(__FILE__, __LINE__, _horus_switch_oneof.Tag())
// ^ We discard `_horus_switch_oneof` above to make sure this macro is used within a
// `HORUS_ONEOF_SWITCH`.

/// Throws a `std::logic_error` in C++14, and does nothing in C++17.
///
/// Due to C++14 not having the `if (<decl>; <expr>)` syntax, `HORUS_ONEOF_CASE()` is implemented
/// using a `for (;;)` loop, which prevents C++ compilers from detecting that a
/// `HORUS_ONEOF_SWITCH()` is exhaustive. This macro can be used to throw an exception following
/// such a switch if the switch is expected to always return. In C++17, this macro does nothing as
/// the compiler can correctly determine if the switch is non-exhaustive.
///
/// We only enable this for Clang since GCC (tested with 13.2) still emits -Werror=return-type.
#if __cplusplus >= 201703L && defined(__clang__)
#define HORUS_ONEOF_RETURN_NOT_HANDLED
#else
#define HORUS_ONEOF_RETURN_NOT_HANDLED           \
  static_cast<void>(_horus_switch_oneof);        \
  ::horus::one_of_internal::OneOfCaseNotHandled( \
      __FILE__, __LINE__, static_cast<std::uint16_t>(_horus_switch_oneof.Tag()))
// ^ We discard `_horus_switch_oneof` above to make sure this macro is used after a
// `HORUS_ONEOF_SWITCH`.
#endif

// NOLINTEND(*-macro-usage)

namespace horus {
namespace one_of_internal {

/// Struct used to invoke some invocable `F` with a reference to the inner value of a `OneOf`.
template <class Union, class F>
struct Dispatcher {
  Union& underlying_union;
  F& invocable;

  /// Dispatches `F(T&)` with the inner value of `OneOf`. This is called by `TagHelpers::Dispatch`
  /// with a null pointer with the type of the current tag.
  template <class T>
  constexpr decltype(auto) operator()(T* /*ptr*/) const {
    return std::forward<F>(invocable)(*underlying_union.template As<T>());
  }
};

}  // namespace one_of_internal

/// Exception thrown by checked `OneOf` members.
class InvalidOneOfAccess final : public std::logic_error {
 public:
  /// Creates an exception with default error message.
  InvalidOneOfAccess() : std::logic_error{"attempted to access wrong union member"} {}
};

/// An empty struct stored instead of `void` in `OneOf`s.
using union_internal::Empty;

// MARK: OneOf<Ts...>

/// A tagged union.
template <class... Ts>
class OneOf;  // IWYU pragma: keep

/// Defines `value` to be true if `T` is `OneOf<Ts...>`.
template <class T>
struct IsOneOf : std::false_type {};
template <class... Ts>
struct IsOneOf<OneOf<Ts...>> : std::true_type {};

template <class... Ts>
class OneOf final {
  /// Returns whether all types in `Ts` satisfy `Check<T>::value`; `void` will be replaced with
  /// `Empty`.
  template <template <class T> class Check>
  static constexpr bool TypesSatisfy() noexcept {
    return Conjunction<Check<union_internal::VoidToEmpty<Ts>>...>::value;
  }

  /// A class which defines static helpers to manipulate `Ts...` and its tag.
  using TagHelpers = ApplyUnique<one_of_internal::TagHelpers, union_internal::VoidToEmpty<Ts>...>;

 public:
  /// The type of the tag.
  using TagType = typename TagHelpers::Tag;

  /// Returns the `Tag` which corresponds to `T`.
  template <class T>
  static constexpr TagType TagFor() noexcept {
    return TagHelpers::TagFor(static_cast<union_internal::VoidToEmpty<T>*>(nullptr));
  }

  // MARK: Constructors

  /// Constructs a new union value of type `T` in-place.
  template <class T, class... Args, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  explicit OneOf(InPlaceTypeT<T> /* in_place */,
                 Args&&... args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
      : tag_{TagFor<T>()} {
    // NOLINTNEXTLINE(*-implicit-pointer-conversion)
    new (union_.template As<T>()) T(std::forward<Args>(args)...);
  }

  /// Constructs a new union value of type `T` in-place.
  template <class T, std::enable_if_t<std::is_void<T>::value>* = nullptr>
  explicit OneOf(InPlaceTypeT<T> /* in_place */) noexcept : tag_{TagFor<T>()} {}

  /// Constructs a new union value with `value`.
  template <class T,
            class VT = std::decay_t<T>,
            std::enable_if_t<!IsOneOf<VT>::value>* = nullptr,
            decltype(TagHelpers::TagFor(static_cast<VT*>(nullptr)))* = nullptr>
  OneOf(T&& value) noexcept(std::is_nothrow_constructible<VT, T>::value)  // NOLINT(*-explicit-*)
      : tag_{TagFor<VT>()} {
    // NOLINTNEXTLINE(*-implicit-pointer-conversion)
    new (union_.template As<VT>()) VT(std::forward<T>(value));
  }

  // MARK: Special members.

  /// Move constructor.
  OneOf(OneOf&& other) noexcept : tag_{other.tag_} {
    static_assert(TypesSatisfy<std::is_nothrow_move_constructible>(),
                  "Ts... must be noexcept-move-constructible");

    other.Visit([this](auto& value) noexcept {
      using T = std::decay_t<decltype(value)>;

      // NOLINTNEXTLINE(*-implicit-pointer-conversion)
      new (union_.template As<T>()) T(std::move(value));
    });
  }

  /// Move assignment operator.
  OneOf& operator=(OneOf&& other) & noexcept {
    static_assert(TypesSatisfy<std::is_nothrow_move_assignable>(),
                  "Ts... must be noexcept-move-assignable");
    // is_nothrow_move_constructible is checked in move constructor used below.

    if (this != &other) {
      if (tag_ == other.tag_) {
        other.Visit([this](auto& value) noexcept {
          using T = std::decay_t<decltype(value)>;

          *union_.template As<T>() = std::move(value);
        });
      } else {
        this->~OneOf();
        new (this) OneOf(std::move(other));
      }
    }
    return *this;
  }

  /// Copy constructor.
  OneOf(OneOf const& other) noexcept(TypesSatisfy<std::is_nothrow_copy_constructible>())
      : tag_{other.tag_} {
    other.VisitConst([this](auto const& value) {
      using T = std::decay_t<decltype(value)>;

      // NOLINTNEXTLINE(*-implicit-pointer-conversion)
      new (union_.template As<T>()) T(value);
    });
  }

  /// No copy assignment is provided as failures would leave the object in an invalid state.
  OneOf& operator=(OneOf const&) noexcept = delete;

  /// Destructor.
  ~OneOf() noexcept {
    static_assert(TypesSatisfy<std::is_nothrow_destructible>(),
                  "Ts... must be noexcept-destructible");

    Visit([](auto& value) noexcept {
      using T = std::decay_t<decltype(value)>;

      value.~T();
    });
  }

  // MARK: Tag(), Is(), EnsureIs()

  /// Returns the current tag.
  constexpr TagType Tag() const noexcept { return tag_; }

  /// Returns whether the union stores `T`.
  template <class T>
  constexpr bool Is() const& noexcept {
    return tag_ == TagFor<T>();
  }

  /// Ensures that the stored value is `T`. If false, throws an exception.
  ///
  /// @throws InvalidOneOfAccess
  template <class T>
  void EnsureIs() const& noexcept(false) {
    if (!Is<T>()) {
      throw InvalidOneOfAccess{};
    }
  }

  // MARK: As()

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidOneOfAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T& As() & noexcept(false) HORUS_LIFETIME_BOUND {
    EnsureIs<T>();
    return UnsafeAs<T>();
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidOneOfAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T const& As() const& noexcept(false) HORUS_LIFETIME_BOUND {
    EnsureIs<T>();
    return UnsafeAs<T>();
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidOneOfAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T&& As() && noexcept(false) HORUS_LIFETIME_BOUND {
    return std::move(As<T>());
  }

  /// Same as `EnsureIs<T>()`.
  ///
  /// This function returns `void` for `T = void` in order to allow forwarding from functions such
  /// as:
  ///
  ///   T Foo() { return one_of_.As<T>(); }
  ///
  /// @throws InvalidOneOfAccess
  template <class T, std::enable_if_t<std::is_void<T>::value>* = nullptr>
  void As() const& noexcept(false) HORUS_LIFETIME_BOUND {
    EnsureIs<T>();
  }

  // MARK: TryAs()

  /// Returns a pointer to the stored value of type `T`, returning nullptr if it is of a different
  /// type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the type matches.
  template <class T>
  union_internal::VoidToEmpty<T>* TryAs() & noexcept HORUS_LIFETIME_BOUND {
    return Is<T>() ? &UnsafeAs<T>() : nullptr;
  }

  /// Returns a pointer to the stored value of type `T`, returning nullptr if it is of a different
  /// type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the type matches.
  template <class T>
  union_internal::VoidToEmpty<T> const* TryAs() const& noexcept HORUS_LIFETIME_BOUND {
    return Is<T>() ? &UnsafeAs<T>() : nullptr;
  }

  // MARK: Emplace()

  /// Replaces the stored value with a new value of type `T` constructed with `args`.
  template <class T, class... Args, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T& Emplace(Args&&... args) & noexcept HORUS_LIFETIME_BOUND {
    static_assert(
        std::is_nothrow_constructible<T, Args...>::value,
        "T must be noexcept-constructible to make sure the OneOf remains in a valid state");

    this->~OneOf();
    tag_ = TagFor<T>();

    // NOLINTNEXTLINE(*-implicit-pointer-conversion)
    return *new (union_.template As<T>()) T(std::forward<Args>(args)...);
  }

  /// Replaces the stored value with a `void`.
  template <class T, std::enable_if_t<std::is_void<T>::value>* = nullptr>
  void Emplace() & noexcept {
    this->~OneOf();
    tag_ = TagFor<T>();
  }

  // MARK: Visit()

  /// Invokes `F` with a reference to the inner value, thus "visiting" the inner state of the
  /// `OneOf`. If the current value is `void`, `invocable()` will be invoked with `Empty`.
  ///
  /// This is similar to `std::visit(F&&, std::variant&)`, but only works with a single `OneOf`
  /// argument, and `F` is guaranteed to be called exactly once.
  template <class F>
  one_of_internal::CommonResultType<F, union_internal::VoidToEmpty<Ts>&...> Visit(
      F&& invocable) & {  // NOLINT(*-missing-std-forward): forwarded by Dispatcher
    return TagHelpers::Dispatch(
        one_of_internal::Dispatcher<typename TagHelpers::Union, F>{union_, invocable}, tag_);
  }

  /// Invokes `F` with a reference to the inner value, thus "visiting" the inner state of the
  /// `OneOf`. If the current value is `void`, `invocable()` will be invoked with `Empty`.
  ///
  /// This is similar to `std::visit(F&&, std::variant const&)`, but only works with a single
  /// `OneOf` argument, and `F` is guaranteed to be called exactly once.
  ///
  /// @note This cannot be an overload of `Visit()` above because the compiler will instantiate
  /// `F(Ts const&)` for all `Ts...` during overload resolution, which will lead to errors if `F`
  /// takes an argument `auto&`, as `auto` may resolve to a `const` reference which will not be
  /// valid within `F`. Small example: https://godbolt.org/z/q5hd7sb7Y. Replacing `auto&` with a
  /// concrete type leads the instantiation of `F` to fail (SFINAE), which works, but is too
  /// restrictive.
  template <class F>
  one_of_internal::CommonResultType<F, union_internal::VoidToEmpty<Ts> const&...> VisitConst(
      F&& invocable) const& {  // NOLINT(*-missing-std-forward): forwarded by Dispatcher
    return TagHelpers::Dispatch(
        one_of_internal::Dispatcher<typename TagHelpers::Union const, F>{union_, invocable}, tag_);
  }

  // MARK: UnsafeAs()

  /// Returns a reference to the stored value of type `T` without checking the tag.
  template <class T>
  union_internal::VoidToEmpty<T>& UnsafeAs() & noexcept HORUS_LIFETIME_BOUND {
    assert(Is<T>());
    return *union_.template As<union_internal::VoidToEmpty<T>>();
  }

  /// Returns a reference to the stored value of type `T` without checking the tag.
  template <class T>
  union_internal::VoidToEmpty<T> const& UnsafeAs() const& noexcept HORUS_LIFETIME_BOUND {
    assert(Is<T>());
    return *union_.template As<union_internal::VoidToEmpty<T>>();
  }

  /// Returns a reference to the stored value of type `T` without checking the tag.
  template <class T>
  union_internal::VoidToEmpty<T>&& UnsafeAs() && noexcept HORUS_LIFETIME_BOUND {
    assert(Is<T>());
    return std::move(*union_.template As<union_internal::VoidToEmpty<T>>());
  }

 private:
  /// The tag of the stored value.
  TagType tag_;
  /// The raw union.
  typename TagHelpers::Union union_;
};

/// Appends `OneOf<T, ...>: <value>` to `sink`.
template <class Sink, class... Ts>
void HorusStringify(Sink& sink, OneOf<Ts...> const& one_of) {
  one_of.InvokeWithConst([&sink](auto const& value) {
    sink.Append("OneOf<");
    sink.Append(typeid(value).name());
    sink.Append(", ...>: ");
    HorusStringify(sink, WithFallback(value, "<no string representation>"));
  });
}

/// Returns the tag of `one_of`.
///
/// This is an alternative to `O::TagFor<T>()` which does not require the `::template` syntax when
/// `O` is templated.
template <class O, class T>
constexpr typename O::TagType OneOfTagFor() noexcept {
  return O::template TagFor<T>();
}

/// Returns whether the given `one_of` holds a value of type `T`.
///
/// This is an alternative to `one_of.Is<T>()` which does not require the `.template` syntax when
/// `Ts...` are not all determined at compile-time.
template <class T, class... Ts>
constexpr bool OneOfIs(OneOf<Ts...> const& one_of) noexcept {
  return one_of.template Is<T>();
}

namespace one_of_internal {

/// Returns the tag of `one_of`.
template <class... Ts>
constexpr typename OneOf<Ts...>::TagType TagOf(OneOf<Ts...> const& one_of) noexcept {
  return one_of.Tag();
}

/// Returns the tag of `T` in `Ts...`.
template <class T, class... Ts>
constexpr typename OneOf<Ts...>::TagType TagFor(OneOf<Ts...> const* /*one_of*/) noexcept {
  return OneOf<Ts...>::template TagFor<T>();
}

/// Returns a reference to the value stored in `one_of` as `T const&`.
template <class T, class... Ts>
constexpr T const& UnsafeAs(OneOf<Ts...> const& one_of HORUS_LIFETIME_BOUND) noexcept {
  return one_of.template UnsafeAs<T>();
}

/// Returns a reference to the value stored in `one_of` as `T&`.
template <class T, class... Ts>
constexpr T& UnsafeAs(OneOf<Ts...>& one_of HORUS_LIFETIME_BOUND) noexcept {
  return one_of.template UnsafeAs<T>();
}

/// Throws a "OneOf case not handled" `std::logic_error`.
[[noreturn]] HORUS_COLD void OneOfCaseNotHandled(StringView file,
                                                 std::uint32_t line,
                                                 std::uint16_t tag);

template <class T, class From>
struct PropagateConstImpl {
  using Type = T;
};
template <class T, class From>
struct PropagateConstImpl<T, From const&> {
  using Type = T const;
};

template <class T, class From>
using PropagateConst = typename PropagateConstImpl<T, From>::Type;

}  // namespace one_of_internal
}  // namespace horus

#endif  // HORUS_TYPES_ONE_OF_H_
