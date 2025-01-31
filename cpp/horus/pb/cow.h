/// @file
///
/// The `Cow<T>` class.

#ifndef HORUS_PB_COW_H_
#define HORUS_PB_COW_H_

#include <type_traits>
#include <utility>

#include "horus/internal/attributes.h"
#include "horus/types/one_of.h"

namespace horus {

/// Either `T` or `const T&`.
///
/// Note that copying a `Cow` internally storing a copy of `T` will create a new copy of `T`, rather
/// than borrowing the original.
template <class T>
class Cow final {
 public:
  /// Constructs a `Cow` which owns its value.
  Cow(T&& value) noexcept : value_{InPlaceType<T>, std::move(value)} {}  // NOLINT(*-explicit-*)
  /// Constructs a `Cow` which borrows a value.
  Cow(const T& value HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept  // NOLINT(*-explicit-*)
      : value_{InPlaceType<const T*>, &value} {}

  /// Returns a reference to the inner value.
  const T& Ref() const noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    const T* const owned{value_.template TryAs<T>()};
    return owned != nullptr ? *owned : **value_.template TryAs<const T*>();
  }

  /// Returns a mutable reference to the inner value. If we're only holding a const reference to it,
  /// it will be copied into a full object that we own first.
  T& Mutable() noexcept(std::is_nothrow_copy_constructible<T>::value)
      HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    T* const owned{value_.template TryAs<T>()};
    if (owned != nullptr) {
      return *owned;
    }
    return value_.template Emplace<T>(**value_.template TryAs<const T*>());
  }

  /// Returns a value `T` copy-constructed from `Ref()` if the value is borrowed, or
  /// move-constructed from `Mutable()` if the value is owned.
  T CopyOrMove() && noexcept(std::is_nothrow_copy_constructible<T>::value) {
    T* const owned{value_.template TryAs<T>()};
    if (owned != nullptr) {
      return std::move(*owned);
    }
    return T{**value_.template TryAs<const T*>()};
  }

  /// Returns the underlying `OneOf`.
  OneOf<T, const T*> Own() && noexcept { return std::move(value_); }

 private:
  /// The (non-null) underlying value.
  OneOf<T, const T*> value_;
};

}  // namespace horus

#endif  // HORUS_PB_COW_H_
