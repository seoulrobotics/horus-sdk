/// @file
///
/// The `AnyFuture<T>` type.

#ifndef HORUS_FUTURE_ANY_H_
#define HORUS_FUTURE_ANY_H_

#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>

#include "horus/future/future.h"
#include "horus/internal/pointer_cast.h"

namespace horus {

template <class T>
class AnyFuture;

namespace horus_internal {

/// Returns whether a future of type `Future` can be stored inline in an `AnyFuture`.
template <class Future>
static constexpr bool CanStoreFutureInline() noexcept;

/// `std::enable_if_t<(whether the AnyFuture allocating constructor should be enabled)>`. Must be
/// defined as a type alias as inlining the definition of the type in the constructor makes it
/// impossible to use an out-of-line definition (which is necessary since `Future` may refer to
/// `AnyFuture`).
template <class T, class Future>
using EnableAnyFutureAllocatingConstructor =
    std::enable_if_t<!std::is_same<Future, AnyFuture<T>>::value &&
                     std::is_base_of<horus::Future<T>, Future>::value &&
                     !CanStoreFutureInline<Future>()>;

/// `std::enable_if_t<(whether the AnyFuture allocating constructor should be enabled)>`. Must be
/// defined as a type alias as inlining the definition of the type in the constructor makes it
/// impossible to use an out-of-line definition (which is necessary since `Future` may refer to
/// `AnyFuture`).
template <class T, class Future>
using EnableAnyFutureInlineConstructor =
    std::enable_if_t<!std::is_same<Future, AnyFuture<T>>::value &&
                     std::is_base_of<horus::Future<T>, Future>::value &&
                     CanStoreFutureInline<Future>()>;

/// Maximum number of words stored in an `AnyFuture` inline data.
static constexpr std::size_t kAnyFutureMaxDataWords{7};

}  // namespace horus_internal

/// A `Future<T>` whose specific implementation is unknown.
///
/// This is essentially a `std::unique_ptr<Future<T>>` which can store small futures inline.
template <class T>
class AnyFuture final : public Future<T> {
 public:
  /// Creates an erased `AnyFuture` which takes ownership of `future`.
  ///
  /// `future` must not be null.
  AnyFuture(std::unique_ptr<Future<T>> future) noexcept {  // NOLINT(*-explicit-*)
    static_assert(sizeof(Future<T>*) == sizeof(void*), "");
    data_[0] = future.release();
  }

  /// Creates an erased `AnyFuture` which wraps `future`.
  ///
  /// @throws std::bad_alloc If the future cannot be allocated.
  template <class Future,
            horus_internal::EnableAnyFutureAllocatingConstructor<T, Future>* = nullptr>
  AnyFuture(Future&& future) noexcept(false);  // NOLINT(*-explicit-*)

  /// Creates an erased `AnyFuture` which wraps `future`.
  template <class Future, horus_internal::EnableAnyFutureInlineConstructor<T, Future>* = nullptr>
  AnyFuture(Future&& future) noexcept;  // NOLINT(*-explicit-*)

  /// Futures cannot be copied.
  AnyFuture(const AnyFuture&) = delete;
  /// Futures cannot be copied.
  AnyFuture& operator=(const AnyFuture&) = delete;

  /// Move-constructs a future.
  AnyFuture(AnyFuture&& other) noexcept {
    if (other.vtable_ == nullptr) {
      data_ = other.data_;
      other.data_ = {};
    } else {
      vtable_ = other.vtable_;
      (vtable_->move_construct)(ConcreteData(), other.ConcreteData());
    }
  }

  /// Move-assigns a future.
  AnyFuture& operator=(AnyFuture&& other) noexcept {
    if (this != &other) {
      ~AnyFuture();
      new (this) AnyFuture{std::move(other)};
    }
    return *this;
  }

  /// Destroys the future.
  ~AnyFuture() noexcept final {
    if (vtable_ == nullptr) {
      std::unique_ptr<Future<T>>(AsVirtual()).reset();
    } else {
      (vtable_->destroy)(ConcreteData());
    }
  }

  /// Implementation of `UnsafePoll()` which relies on the type-erased future.
  PollResult<T> UnsafePoll(PollContext& context) final {
    if (vtable_ == nullptr) {
      return AsVirtual()->UnsafePoll(context);
    }
    return (vtable_->poll)(ConcreteData(), context);
  }

 private:
  /// Table of function pointers used to refer to the future when it is stored inline.
  struct VTable {
    /// Calls `Future::UnsafePoll()`.
    PollResult<T> (*poll)(void* data, PollContext& context);
    /// Calls `Future::~Future()`.
    void (*destroy)(void* data);
    /// Calls `Future::Future(Future&&)`.
    void (*move_construct)(void* data, void* other);
  };

  /// Function whose pointer is set in `VTable::poll`.
  template <class Future>
  static PollResult<T> VTablePoll(void* data, PollContext& context) {
    return AsConcrete<Future>(data)->UnsafePoll(context);
  }
  /// Function whose pointer is set in `VTable::destroy`.
  template <class Future>
  static void VTableDestroy(void* data) {
    AsConcrete<Future>(data)->~Future();
  }
  /// Function whose pointer is set in `VTable::move_construct`.
  template <class Future>
  static void VTableMoveConstruct(void* data, void* other) {
    new (AsConcrete<Future>(data)) Future{std::move(*AsConcrete<Future>(other))};
  }

  /// Returns the stored data as a pointer to the virtual `Future<T>` class.
  Future<T>* AsVirtual() noexcept { return horus_internal::UnsafePointerCast<Future<T>>(data_[0]); }

  /// Returns the stored data as a pointer to a concrete `Future` class.
  ///
  /// `data` is the pointer given to `VTable` functions.
  template <class Future>
  static Future* AsConcrete(void* data) noexcept {
    static_assert(horus_internal::CanStoreFutureInline<Future>(),
                  "AsConcrete() should only be called with classes stored inline");
    return horus_internal::UnsafePointerCast<Future>(data);
  }

  /// Returns the pointer which refers to the data of the concrete future, as used when calling
  /// `VTable` functions.
  void* ConcreteData() noexcept {
    // NOLINTNEXTLINE(*-pointer-conversion): https://github.com/llvm/llvm-project/issues/93959
    return static_cast<void*>(data_.data());
  }

  /// Data representing the future. If it is stored inline (`vtable_` is non-null), this is the raw
  /// bytes of the future. If it is stored on the heap (`vtable_` is null), this is a fat pointer to
  /// `Future<T>`.
  std::array<void*, horus_internal::kAnyFutureMaxDataWords> data_{};
  /// Table of pointer for the stored future. May be null, in which case the future is stored on the
  /// heap and accessed using its `Future<T>` vtable.
  const VTable* vtable_{nullptr};
};

// The constructors below use `CanStoreInline()` in `Enable*Constructor`. Due to cycles in
// declaration order, they must be defined outside of the class constructor.

namespace horus_internal {

template <class Future>
constexpr bool CanStoreFutureInline() noexcept {
  // NOLINTNEXTLINE(*-redundant-expression): `<=` is not redundant; `Future` is a template argument
  return sizeof(Future) <= sizeof(std::array<void*, kAnyFutureMaxDataWords>) &&
         alignof(Future) <= alignof(void*);
}

}  // namespace horus_internal

template <class T>
template <class Future, horus_internal::EnableAnyFutureAllocatingConstructor<T, Future>*>
AnyFuture<T>::AnyFuture(Future&& future) noexcept(false)  // NOLINT(*-forwarding-*)
    : AnyFuture(std::make_unique<Future>(std::forward<Future>(future))) {}

template <class T>
template <class Future, horus_internal::EnableAnyFutureInlineConstructor<T, Future>*>
AnyFuture<T>::AnyFuture(Future&& future) noexcept {  // NOLINT(*-forwarding-*)
  static constexpr VTable kVTable{
      &AnyFuture::VTablePoll<Future>,
      &AnyFuture::VTableDestroy<Future>,
      &AnyFuture::VTableMoveConstruct<Future>,
  };

  vtable_ = &kVTable;
  new (ConcreteData()) Future{std::forward<Future>(future)};
}

}  // namespace horus

#endif  // HORUS_FUTURE_ANY_H_
