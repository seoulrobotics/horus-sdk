/// @file
///
/// The `OneOf` type.

#ifndef HORUS_TYPES_ONE_OF_H_
#define HORUS_TYPES_ONE_OF_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "horus/attributes.h"
#include "horus/internal/type_traits.h"
#include "horus/internal/unsafe_union.h"
#include "horus/strings/fallback.h"
#include "horus/strings/string_view.h"
#include "horus/type_traits/conjunction.h"
#include "horus/types/in_place.h"

namespace horus {
namespace horus_internal {

/// Tag type for `OneOf`s with 1 type.
enum class Tag1 : std::uint8_t {
  k0,
};
/// Tag type for `OneOf`s with 2 types.
enum class Tag2 : std::uint8_t {
  k0,
  k1,
};
/// Tag type for `OneOf`s with 3 types.
enum class Tag3 : std::uint8_t {
  k0,
  k1,
  k2,
};
/// Tag type for `OneOf`s with 4 types.
enum class Tag4 : std::uint8_t {
  k0,
  k1,
  k2,
  k3,
};
/// Tag type for `OneOf`s with 5 types.
enum class Tag5 : std::uint8_t {
  k0,
  k1,
  k2,
  k3,
  k4,
};

/// Defines `type = TagN` with `N = Tags`.
template <std::size_t Tags>
struct TagTypeFor;
template <>
struct TagTypeFor<1> {
  using type = Tag1;
};
template <>
struct TagTypeFor<2> {
  using type = Tag2;
};
template <>
struct TagTypeFor<3> {
  using type = Tag3;
};
template <>
struct TagTypeFor<4> {
  using type = Tag4;
};
template <>
struct TagTypeFor<5> {
  using type = Tag5;
};

/// The return type of `F(T&)`.
template <class F, class T>
struct ResultType {
  using type = decltype(std::declval<F>()(std::declval<T&>()));

  /// Whether the call is `noexcept`.
  static constexpr bool kIsNoExcept{noexcept(std::declval<F>()(std::declval<T&>()))};
};
template <class F>
struct ResultType<F, void> {
  using type = void;

  /// `true`, since we never invoke `F` if the type is void.
  static constexpr bool kIsNoExcept{true};
};
template <class F>
struct ResultType<F, const void> {
  using type = void;

  /// `true`, since we never invoke `F` if the type is void.
  static constexpr bool kIsNoExcept{true};
};

/// Implementation of `CommonResultTypeT`.
template <class F, class T, class... Rest>
struct CommonResultType : ResultType<F, T> {};

/// `std::common_type_t` applies `std::decay_t` to its types, which is not acceptable for us.
template <class F, class... Ts>
using CommonResultTypeT = typename CommonResultType<F, Ts...>::type;

}  // namespace horus_internal

/// Exception thrown by checked `OneOf` members.
class InvalidUnionAccess final : public std::logic_error {
 public:
  /// Creates an exception with default error message.
  InvalidUnionAccess() : std::logic_error{"attempted to access wrong union member"} {}
};

// MARK: OneOf<Ts...>

/// A tagged union.
template <class... Ts>
class OneOf;

/// Defines `value` to be true if `T` is `OneOf<Ts...>`.
template <class T>
struct IsOneOf : std::false_type {};
template <class... Ts>
struct IsOneOf<OneOf<Ts...>> : std::true_type {};

template <class... Ts>
class OneOf final {
  /// Returns whether all types in `Ts` satisfy `Check<T>::value`; `void` will be replaced with
  /// `std::nullptr_t`.
  template <template <class T> class Check>
  static constexpr bool TypesSatisfy() noexcept {
    return Conjunction<
        Check<std::conditional_t<std::is_void<Ts>::value, std::nullptr_t, Ts>>...>::value;
  }

 public:
  /// The type of the tag.
  using TagType = typename horus_internal::TagTypeFor<sizeof...(Ts)>::type;

  /// The type at index `I`.
  template <std::size_t I>
  using TypeAt = horus_internal::TypeAtT<I, Ts...>;

  /// The index of type `T`. If `T` is not in `Ts...` or is in `Ts...` more than once, a build error
  /// will occur.
  template <class T>
  static constexpr std::size_t kIndexOf{
      horus_internal::UniquePackIndexOf<horus_internal::kNotFound, 0, T, Ts...>::value};

  /// Returns the `Tag` which corresponds to the type at index `I`.
  template <std::size_t I>
  static constexpr TagType TagAt() noexcept {
    static_assert(I < sizeof...(Ts), "");
    return static_cast<TagType>(I);
  }

  /// Returns the `Tag` which corresponds to `T`.
  template <class T>
  static constexpr TagType TagFor() noexcept {
    return TagAt<kIndexOf<T>>();
  }

  /// The `Tag` which corresponds to `T`.
  template <std::size_t I>
  static constexpr TagType kTagAt{TagAt<I>()};

  /// The `Tag` which corresponds to `T`.
  template <class T>
  static constexpr TagType kTagFor{TagFor<T>()};

  // MARK: Constructors

  /// Constructs a new union value at index `I` in-place.
  template <std::size_t I, class... Args,
            std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  explicit OneOf(InPlaceIndexT<I> /* in_place */,
                 Args&&... args) noexcept(std::is_nothrow_constructible<TypeAt<I>, Args...>::value)
      : tag_{TagAt<I>()} {
    new (union_.template As<TypeAt<I>>()) TypeAt<I>(std::forward<Args>(args)...);
  }

  /// Constructs a new union value at index `I` in-place.
  template <std::size_t I, std::enable_if_t<std::is_void<TypeAt<I>>::value>* = nullptr>
  explicit OneOf(InPlaceIndexT<I> /* in_place */) noexcept : tag_{TagAt<I>()} {}

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
  template <class T, std::enable_if_t<!IsOneOf<std::decay_t<T>>::value>* = nullptr>
  OneOf(T&& value) noexcept : tag_{TagFor<T>()} {  // NOLINT(*-explicit-*)
    // NOLINTNEXTLINE(*-implicit-pointer-conversion)
    new (union_.template As<T>()) T(std::forward<T>(value));
  }

  // MARK: Special members.

  /// Move constructor.
  OneOf(OneOf&& other) noexcept : tag_{other.tag_} {
    static_assert(TypesSatisfy<std::is_nothrow_move_constructible>(),
                  "Ts... must be noexcept-move-constructible");

    other.InvokeWith([this](auto& value) noexcept {
      using T = std::decay_t<decltype(value)>;

      // NOLINTNEXTLINE(*-implicit-pointer-conversion)
      new (union_.template As<T>()) T(std::move(value));
    });
  }

  /// Move assignment operator.
  OneOf& operator=(OneOf&& other) noexcept {
    static_assert(TypesSatisfy<std::is_nothrow_move_assignable>(),
                  "Ts... must be noexcept-move-assignable");
    // is_nothrow_move_constructible is checked in move constructor used below.

    if (this != &other) {
      if (tag_ == other.tag_) {
        InvokeWith([this, &other](auto& value) noexcept {
          using T = std::decay_t<decltype(value)>;

          *union_.template As<T>() = std::move(*other.union_.template As<T>());
        });
      } else {
        this->~OneOf();
        new (this) OneOf(std::move(other));
      }
    }
    return *this;
  }

  /// Copy constructor.
  OneOf(const OneOf& other) noexcept(TypesSatisfy<std::is_nothrow_copy_constructible>())
      : tag_{other.tag_} {
    other.InvokeWithConst([this](const auto& value) {
      using T = std::decay_t<decltype(value)>;

      // NOLINTNEXTLINE(*-implicit-pointer-conversion)
      new (union_.template As<T>()) T(value);
    });
  }

  /// No copy assignment is provided as failures would leave the object in an invalid state.
  OneOf& operator=(const OneOf&) = delete;

  /// Destructor.
  ~OneOf() noexcept {
    static_assert(TypesSatisfy<std::is_nothrow_destructible>(),
                  "Ts... must be noexcept-destructible");

    InvokeWith([](auto& value) noexcept {
      using T = std::decay_t<decltype(value)>;

      value.~T();
    });
  }

  // MARK: Tag(), Is(), EnsureIs()

  /// Returns the current tag.
  constexpr TagType Tag() const noexcept { return tag_; }

  /// Returns the type name of the current type.
  StringView CurrentTypeName() const noexcept {
    StringView result{"void"};
    InvokeWithConst(
        [&result](const auto& value) noexcept { result = typeid(decltype(value)).name(); });
    return result;
  }

  /// Returns whether the union stores the type at index `I`.
  template <std::size_t I>
  constexpr bool Is() const& noexcept {
    return tag_ == TagAt<I>();
  }

  /// Returns whether the union stores `T`.
  template <class T>
  constexpr bool Is() const& noexcept {
    return tag_ == TagFor<T>();
  }

  /// Ensures that the stored value is the type at index `I`. If false, throws an exception.
  ///
  /// @throws InvalidUnionAccess
  template <std::size_t I>
  void EnsureIs() const& noexcept(false) {
    if (!Is<I>()) {
      throw InvalidUnionAccess{};
    }
  }

  /// Ensures that the stored value is `T`. If false, throws an exception.
  ///
  /// @throws InvalidUnionAccess
  template <class T>
  void EnsureIs() const& noexcept(false) {
    if (!Is<T>()) {
      throw InvalidUnionAccess{};
    }
  }

  // MARK: As()

  /// Returns a reference to the stored value of type at index `I`, throwing an exception if it is
  /// of a different type.
  ///
  /// @throws InvalidUnionAccess
  template <std::size_t I, std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  TypeAt<I>& As() & noexcept(false) HORUS_LIFETIME_BOUND {
    EnsureIs<I>();
    return *union_.template As<TypeAt<I>>();
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidUnionAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T& As() & noexcept(false) HORUS_LIFETIME_BOUND {
    return As<kIndexOf<T>>();
  }

  /// Returns a reference to the stored value of type at index `I`, throwing an exception if it is
  /// of a different type.
  ///
  /// @throws InvalidUnionAccess
  template <std::size_t I, std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  const TypeAt<I>& As() const& noexcept(false) HORUS_LIFETIME_BOUND {
    EnsureIs<I>();
    return *union_.template As<TypeAt<I>>();
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidUnionAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  const T& As() const& noexcept(false) HORUS_LIFETIME_BOUND {
    return As<kIndexOf<T>>();
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidUnionAccess
  template <std::size_t I, std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  TypeAt<I>&& As() && noexcept(false) HORUS_LIFETIME_BOUND {
    return std::move(As<I>());
  }

  /// Returns a reference to the stored value of type `T`, throwing an exception if it is of a
  /// different type.
  ///
  /// @throws InvalidUnionAccess
  template <class T, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T&& As() && noexcept(false) HORUS_LIFETIME_BOUND {
    return std::move(As<T>());
  }

  /// Same as `EnsureIs<I>()`.
  ///
  /// @throws InvalidUnionAccess
  template <std::size_t I, std::enable_if_t<std::is_void<TypeAt<I>>::value>* = nullptr>
  void As() noexcept(false) {
    EnsureIs<I>();
  }

  /// Same as `EnsureIs<T>()`.
  ///
  /// @throws InvalidUnionAccess
  template <class T, std::enable_if_t<std::is_void<T>::value>* = nullptr>
  void As() noexcept(false) {
    EnsureIs<T>();
  }

  // MARK: TryAs()

  /// Returns a pointer to the stored value of type at index `I`, returning nullptr if it is of a
  /// different type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the tag matches.
  template <std::size_t I>
  TypeAt<I>* TryAs() & noexcept HORUS_LIFETIME_BOUND {
    return Is<I>() ? union_.template As<TypeAt<I>>() : nullptr;
  }

  /// Returns a pointer to the stored value of type `T`, returning nullptr if it is of a different
  /// type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the type matches.
  template <class T>
  T* TryAs() & noexcept HORUS_LIFETIME_BOUND {
    return TryAs<kIndexOf<T>>();
  }

  /// Returns a pointer to the stored value of type at index `I`, returning nullptr if it is of a
  /// different type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the tag matches.
  template <std::size_t I>
  const TypeAt<I>* TryAs() const& noexcept HORUS_LIFETIME_BOUND {
    return Is<I>() ? union_.template As<TypeAt<I>>() : nullptr;
  }

  /// Returns a pointer to the stored value of type `T`, returning nullptr if it is of a different
  /// type.
  ///
  /// @note If the inner type is `void`, a non-null pointer to an arbitrary value will be returned
  /// if the type matches.
  template <class T>
  const T* TryAs() const& noexcept HORUS_LIFETIME_BOUND {
    return TryAs<kIndexOf<T>>();
  }

  // MARK: Emplace()

  /// Replaces the stored value with a new value of type `T` constructed with `args`.
  template <std::size_t I, class... Args,
            std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  TypeAt<I>& Emplace(Args&&... args) & noexcept HORUS_LIFETIME_BOUND {
    using T = TypeAt<I>;

    static_assert(
        std::is_nothrow_constructible<T, Args...>::value,
        "T must be noexcept-constructible to make sure the OneOf remains in a valid state");

    this->~OneOf();
    tag_ = TagAt<I>();
    // NOLINTNEXTLINE(*-implicit-pointer-conversion)
    return *new (union_.template As<T>()) T(std::forward<Args>(args)...);
  }

  /// Replaces the stored value with a new value of type `T` constructed with `args`.
  template <class T, class... Args, std::enable_if_t<!std::is_void<T>::value>* = nullptr>
  T& Emplace(Args&&... args) & noexcept HORUS_LIFETIME_BOUND {
    return Emplace<kIndexOf<T>>(std::forward<Args>(args)...);
  }

  /// Replaces the stored value with a new value of type `T` constructed with `args`.
  template <std::size_t I, std::enable_if_t<std::is_void<TypeAt<I>>::value>* = nullptr>
  void Emplace() & noexcept {
    this->~OneOf();
    tag_ = TagAt<I>();
  }

  /// Replaces the stored value with a new value of type `T` constructed with `args`.
  template <class T, std::enable_if_t<std::is_void<T>::value>* = nullptr>
  void Emplace() & noexcept {
    Emplace<kIndexOf<T>>();
  }

  // MARK: InvokeWith()

  /// Invokes `F` with a reference to the inner value.
  template <class F>
  horus_internal::CommonResultTypeT<F, Ts...> InvokeWith(F&& invocable) noexcept(
      ConjunctionOf({horus_internal::ResultType<F, Ts>::kIsNoExcept...})) {
    using R = horus_internal::CommonResultTypeT<F, Ts...>;

    return InvokeWithInner<0, R>(*this, std::forward<F>(invocable));
  }

  /// Invokes `F` with a reference to the inner value.
  ///
  /// @note In theory this could be an overload of `InvokeWith()` above, but for some reason clang
  /// prefers this overload to the one above even though it is `const`.
  template <class F>
  horus_internal::CommonResultTypeT<F, const Ts...> InvokeWithConst(F&& invocable) const
      noexcept(ConjunctionOf({horus_internal::ResultType<F, const Ts>::kIsNoExcept...})) {
    using R = horus_internal::CommonResultTypeT<F, const Ts...>;

    return InvokeWithInner<0, R>(*this, std::forward<F>(invocable));
  }

 private:
  /// Recursive function which calls `invocable()` on the current type.
  template <std::size_t I, class R, class Self, class F,
            std::enable_if_t<(I < sizeof...(Ts))>* = nullptr,
            std::enable_if_t<!std::is_void<TypeAt<I>>::value>* = nullptr>
  static R InvokeWithInner(Self& self, F&& invocable) {
    if (self.template Is<I>()) {
      return std::forward<F>(invocable)(*self.union_.template As<TypeAt<I>>());
    }
    return InvokeWithInner<I + 1, R>(self, std::forward<F>(invocable));
  }

  /// Recursive function which calls `invocable()` on the current type.
  template <std::size_t I, class R, class Self, class F,
            std::enable_if_t<(I < sizeof...(Ts))>* = nullptr,
            std::enable_if_t<std::is_void<TypeAt<I>>::value>* = nullptr>
  static R InvokeWithInner(Self& self, F&& invocable) {
    if (!self.template Is<I>()) {
      return InvokeWithInner<I + 1, R>(self, std::forward<F>(invocable));
    }
  }

  /// Base case of `InvokeWithInner()`.
  template <std::size_t I, class R, class Self, class F,
            std::enable_if_t<(I == sizeof...(Ts))>* = nullptr>
  static R InvokeWithInner(Self& /* self */, F&& invocable) noexcept {
    static_cast<void>(F{std::forward<F>(invocable)});
    assert(false && "caller should have handled I, as I == sizeof...(Ts) is invalid");
    std::abort();
  }

  /// The tag of the stored value.
  TagType tag_;
  /// The raw union.
  horus_internal::UnsafeUnion<Ts...> union_;
};

/// Appends `OneOf<T, ...>: <value>` to `sink`.
template <class Sink, class... Ts>
void HorusStringify(Sink& sink, const OneOf<Ts...>& one_of) {
  bool printed{false};
  one_of.InvokeWithConst([&printed, &sink](const auto& value) {
    printed = true;
    sink.Append("OneOf<");
    sink.Append(typeid(value).name());
    sink.Append(", ...>: ");
    HorusStringify(sink, WithFallback(value, "<no string representation>"));
  });
  if (!printed) {
    sink.Append("OneOf<void, ...>");
  }
}

}  // namespace horus

#endif  // HORUS_TYPES_ONE_OF_H_
