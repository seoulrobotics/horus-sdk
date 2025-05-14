/// @file
///
/// The `ErasedPointer` type.

#ifndef HORUS_POINTER_ERASED_H_
#define HORUS_POINTER_ERASED_H_

#include <typeinfo>

#include "horus/attributes.h"

// NOLINTNEXTLINE(*-macro-usage): used in directives
#define HORUS_ERASED_POINTER_CHECK !NDEBUG

#if HORUS_ERASED_POINTER_CHECK
#include "horus/optimization.h"
#endif

namespace horus {

/// A type-erased pointer which replaces `void*`.
///
/// It can be constructed from any pointer type and converted back to the original pointer type. In
/// debug builds, it ensures that these types are the same.
class ErasedPointer final {
 public:
  /// Constructs a `ErasedPointer` wrapping the given value.
  template <class T>
  constexpr explicit ErasedPointer(T* pointer HORUS_LIFETIME_BOUND) noexcept
      : pointer_{pointer}
#if HORUS_ERASED_POINTER_CHECK
        ,
        pointer_info_{&kPointerInfoFor<T>}
#endif
  {
  }

  /// Constructs a null `ErasedPointer`.
  constexpr explicit ErasedPointer(decltype(nullptr)) noexcept : pointer_{nullptr} {}

  /// Returns the stored pointer as a `T const*`. If assertions are enabled, this will throw an
  /// `std::logic_error` if the stored pointer is not of type `T`.
  template <class T>
  T const* UnsafeAs() const& {
#if HORUS_ERASED_POINTER_CHECK
    if (HORUS_PREDICT_FALSE(pointer_ != nullptr && pointer_info_->type_info != typeid(T))) {
      ThrowInvalidCastError(pointer_info_->type_info, typeid(T));
    }
#endif
    // SAFETY: the caller knows they're doing something unsafe (by calling `UnsafeAs`), and in
    // assertion builds we checked that the cast is valid.
    return reinterpret_cast<T const*>(pointer_);  // NOLINT(*-reinterpret-cast)
  }

  /// Returns the stored pointer as a `T*`. If assertions are enabled, this will throw an
  /// `std::logic_error` if the stored pointer is not of type `T` or is `const` and `T` isn't.
  template <class T>
  T* UnsafeAs() & {
#if HORUS_ERASED_POINTER_CHECK
    if (HORUS_PREDICT_FALSE(pointer_ != nullptr && pointer_info_->is_const &&
                            !IsConst(static_cast<T*>(nullptr)))) {
      ThrowInvalidConstCastError(typeid(T));
    }
#endif
    // SAFETY: the caller knows they're doing something unsafe (by calling `UnsafeAs`), and in
    // assertion builds we checked that the cast is valid.
    return const_cast<T*>(  // NOLINT(*-const-cast)
        static_cast<ErasedPointer const&>(*this).UnsafeAs<T>());
  }

  /// Returns whether the pointer is null.
  friend constexpr bool operator==(ErasedPointer pointer, decltype(nullptr)) noexcept {
    return pointer.pointer_ == nullptr;
  }
  /// Returns whether the pointer is non-null.
  friend constexpr bool operator!=(ErasedPointer pointer, decltype(nullptr)) noexcept {
    return pointer.pointer_ != nullptr;
  }

  /// Clears the underlying pointer.
  constexpr ErasedPointer& operator=(decltype(nullptr)) & noexcept {
    pointer_ = nullptr;
    return *this;
  }

 private:
  /// Throws an `std::logic_error` following an invalid cast from `from` to `to`.
  [[noreturn]] static void ThrowInvalidCastError(std::type_info const& from,
                                                 std::type_info const& to);
  /// Throws an `std::logic_error` following an invalid cast from `const` to non-`const`.
  [[noreturn]] static void ThrowInvalidConstCastError(std::type_info const& type);

  /// The type-erased pointer. May be null.
  void const* pointer_;

#if HORUS_ERASED_POINTER_CHECK
  /// Information about a stored pointer.
  struct PointerInfo {
    /// The target type information.
    std::type_info const& type_info;
    /// Whether the pointer is to a `const` value.
    bool const is_const;
  };

  template <class T>
  static constexpr bool IsConst(T* /*ptr*/) noexcept {
    return false;
  }
  template <class T>
  static constexpr bool IsConst(T const* /*ptr*/) noexcept {
    return true;
  }

  template <class T>
  static constexpr PointerInfo kPointerInfoFor{typeid(T), IsConst(static_cast<T*>(nullptr))};

  /// Information about `pointer_`. May only be null if `pointer_` is null.
  PointerInfo const* pointer_info_{nullptr};
#endif
};

#if HORUS_ERASED_POINTER_CHECK
template <class T>
// NOLINTNEXTLINE(*-redundant-declaration): needed to link the static member
ErasedPointer::PointerInfo const ErasedPointer::kPointerInfoFor;
#endif

}  // namespace horus

#endif  // HORUS_POINTER_ERASED_H_
