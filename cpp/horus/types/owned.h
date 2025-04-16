/// @file
///
/// The `Owned` class.

#ifndef HORUS_TYPES_OWNED_H_
#define HORUS_TYPES_OWNED_H_

#include <cassert>
#include <cstdint>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>

#include "horus/internal/attributes.h"
#include "horus/source_location.h"
#include "horus/types/in_place.h"

namespace horus {

/// The compile-time "mode" of a `Owned<T>`.
enum class OwnedMode : std::uint8_t {
  /// `Owned<T>` will use reference counting to ensure use-after-free's never occur.
  kAllocateOneThread,
  /// `Owned<T>` will use atomic reference counting to ensure use-after-free's never occur.
  kAllocate,

  /// If a `Borrowed<T>` reference outlives its `Owned<T>` owner or accesses it from another thread,
  /// a runtime assertion will abort the process.
  kAssertOneThread,
  /// If a `Borrowed<T>` reference outlives its `Owned<T>` owner, a runtime assertion will abort
  /// the process.
  kAssert,

  /// `Borrowed<T>` will not check the safety of the access of their variable.
  kAssume,

  /// Automatically choose the single-thread mode; when debug assertions are enabled, this is
  /// `kAssertOneThread`. When they are disabled, this is `kAssume`.
  kAutomaticOneThread = false ? kAssume : kAssertOneThread,
  /// Automatically choose the mode; when debug assertions are enabled, this is `kAssert`. When they
  /// are disabled, this is `kAssume`.
  kAutomatic = false ? kAssume : kAssert,
};

namespace horus_internal {

/// A structure used to check that access to an object is valid.
class LifetimeChecker final {
 public:
  /// Constructs the checker.
  explicit LifetimeChecker(bool one_thread) noexcept(false)
      : state{std::make_shared<SharedState>()} {
    if (one_thread) {
      state->thread_id = std::this_thread::get_id();
    }
  }

  /// Invalidates the lifetime; future calls to `Check()` will abort the process.
  void Invalidate(SourceLocation source_location) noexcept {
    assert(state->thread_id == std::thread::id{} || state->thread_id == std::this_thread::get_id());
    state->invalidated_location = source_location;
  }

  /// Checks that `Invalidate()` was never called.
  void Check() const noexcept {
    assert(state->invalidated_location.File().empty());
    assert(state->thread_id == std::thread::id{} || state->thread_id == std::this_thread::get_id());
  }

 private:
  /// Type of `state`.
  struct SharedState {
    /// The location at which `Invalidate()` was called. If `File().empty()`, `Invalidate()` was not
    /// called.
    SourceLocation invalidated_location{"", 0};
    /// The thread to which the object belongs. If `std::thread::id()`, there is no need to check
    /// this.
    std::thread::id thread_id{};
  };

  /// The actual state of the checker.
  std::shared_ptr<SharedState> state;
};

/// A no-op object with the same interface as `LifetimeChecker`.
class NoopLifetimeChecker final {
 public:
  /// Constructs the object.
  constexpr explicit NoopLifetimeChecker(bool one_thread) noexcept {
    static_cast<void>(one_thread);
  }

  /// No-op.
  constexpr void Invalidate(SourceLocation source_location) noexcept {
    static_cast<void>(this);
    static_cast<void>(source_location);
  }
  /// No-op.
  constexpr void Check() const noexcept { static_cast<void>(this); }
};

/// The underlying implementation of `Owned<T, Mode>`.
///
/// This is separate from `Owned<T, Mode>` to expose a minimal interface using specializations,
/// so that `Owned<T, Mode>` may expose a more user-friendly, easier-to-read interface.
template <class T, OwnedMode Mode>
class OwnedImpl final {
  /// Whether assertions are enabled.
  static constexpr bool kAssert{Mode == OwnedMode::kAssert};

  /// Type of `lifetime_`.
  using Lifetime = std::conditional_t<kAssert, LifetimeChecker, NoopLifetimeChecker>;

 public:
  /// Implementation of `Borrowed`.
  class Borrowed final {
   public:
    /// Constructs a borrowed reference.
    constexpr explicit Borrowed(const OwnedImpl& owner) noexcept
        : borrowed_{&owner.value_}, lifetime_{owner.lifetime_} {}

    /// Returns a reference to the borrowed value.
    constexpr T& Get() const noexcept {
      lifetime_.Check();
      return *borrowed_;
    }

   private:
    /// A (non-null) pointer to the value.
    const T* borrowed_;
    /// A copy of the owner `lifetime_`.
    HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS Lifetime lifetime_;
  };

  /// Constructs the owner in-place.
  template <class... Args>
  constexpr explicit OwnedImpl(InPlace /* in_place */, Args&&... args) noexcept(false)
      : value_(std::forward<Args>(args)...) {}

  /// Returns a reference to the value.
  constexpr T& Get() noexcept { return value_; }
  /// Returns a reference to the value.
  constexpr const T& Get() const noexcept { return value_; }

  /// Indicates that the owned value is no longer valid and should not be accessed.
  void Invalidate(SourceLocation source_location) noexcept {
    lifetime_.Invalidate(source_location);
  }

 private:
  /// The owned value.
  T value_;
  /// An object used to check invariants when `kAssert` is true.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS Lifetime lifetime_{/*one_thread=*/Mode ==
                                                           OwnedMode::kAssertOneThread};
};

template <class T>
class OwnedImpl<T, OwnedMode::kAllocate> final {
 public:
  /// Implementation of `Borrowed`.
  class Borrowed final {
   public:
    /// Constructs a borrowed reference.
    explicit Borrowed(const OwnedImpl& owner) noexcept : borrowed_{owner.value_} {}

    /// Returns a reference to the borrowed value.
    constexpr T& Get() const noexcept { return *borrowed_; }

   private:
    /// A (non-null) pointer to the borrowed value.
    std::shared_ptr<T> borrowed_;
  };

  /// Constructs the owner in-place.
  template <class... Args>
  constexpr explicit OwnedImpl(InPlace /* in_place */, Args&&... args) noexcept(false)
      : value_{std::make_shared<T>(std::forward<Args>(args)...)} {}

  /// Returns a reference to the value.
  T& Get() noexcept { return *value_; }
  /// Returns a reference to the value.
  const T& Get() const noexcept { return *value_; }

  /// Indicates that the owned value is no longer valid and should not be accessed.
  void Invalidate(SourceLocation source_location) noexcept {
    static_cast<void>(this);
    static_cast<void>(source_location);
  }

 private:
  /// A pointer to the "owned" value (only null after being moved).
  std::shared_ptr<T> value_;
};

}  // namespace horus_internal

template <class T, OwnedMode Mode = OwnedMode::kAutomatic>
class Borrowed;

/// A wrapper around an owned object `T` which can be shared with other objects.
///
/// This is conceptually `T`, where references are explicitly created with `Share()`. `T` is _still_
/// owned by the `Owned`.
///
/// This class can be used to keep a reference to an object `T` without any concrete proof that this
/// reference will outlive `T`. When debug assertions are enabled, `Owned<T>` will ensure that it
/// outlives all shared references to it. If it is particularly hard to ensure that a `Owned<T>`
/// always outlives references to it, `OwnedMode::kAllocate` can be used to ensure all references
/// stay valid at the cost of performance.
template <class T, OwnedMode Mode = OwnedMode::kAutomatic>
class HORUS_SDK_ATTRIBUTE_TRIVIAL_ABI Owned final {
 public:
  /// Constructs an `Owned<T>` wrapper around an object of type `T` constructed with `args`.
  ///
  /// @throws std::exception Any exception thrown by `T(Args...)`.
  /// @throws std::bad_alloc If an allocation is required to check invariants.
  template <class... Args>
  explicit Owned(Args&&... args) noexcept(false) : impl_{kInPlace, std::forward<Args>(args)...} {}

  /// Not copyable.
  Owned(const Owned&) = delete;
  /// Not copyable.
  Owned& operator=(const Owned&) = delete;

  /// Move-constructs an `Owned<T>` object. `other` must not be borrowed.
  Owned(Owned&& other, SourceLocation source_location = CurrentSourceLocation()) noexcept
      : impl_{kInPlace, std::move(other.Get())} {
    other.impl_.Invalidate(source_location);
  }

  /// Not move-assignable.
  Owned& operator=(Owned&&) noexcept = delete;

  /// Destroys the `Owned<T>` object. Future accesses to the value from `Borrowed` instances will be
  /// invalid.
  ~Owned() noexcept { impl_.Invalidate(CurrentSourceLocation()); }

  /// Returns a reference to the owned value.
  constexpr T& Get() noexcept { return impl_.Get(); }
  /// Returns a reference to the owned value.
  constexpr const T& Get() const noexcept { return impl_.Get(); }
  /// Returns a reference to the owned value.
  constexpr T& operator*() noexcept { return Get(); }
  /// Returns a reference to the owned value.
  constexpr const T& operator*() const noexcept { return Get(); }
  /// Returns a pointer to the owned value.
  constexpr T* operator->() noexcept { return &Get(); }
  /// Returns a pointer to the owned value.
  constexpr const T* operator->() const noexcept { return &Get(); }

  /// Returns a `Borrowed` reference to the owned value.
  constexpr Borrowed<T, Mode> Borrow() noexcept;
  /// Returns a const `Borrowed` reference to the owned value.
  constexpr Borrowed<const T, Mode> Borrow() const noexcept;

 private:
  /// Type of `impl_`.
  using Impl = horus_internal::OwnedImpl<T, Mode>;

  /// The implementation of the object.
  Impl impl_;
};

/// A non-owning reference to an `Owned<T>` object.
template <class T, OwnedMode Mode>
class HORUS_SDK_ATTRIBUTE_TRIVIAL_ABI Borrowed final {
 public:
  /// The type of the owner of this object; for `const` `T`s this is `const Owner<T, Mode>`,
  /// otherwise it is `Owner<T, Mode>`.
  using Owner = std::conditional_t<std::is_const<T>::value,
                                   const Owned<std::remove_const_t<T>, Mode>, Owned<T, Mode>>;

  /// Constructs a `Borrowed` reference to the object owned by `owner`.
  constexpr Borrowed(Owner& owner) noexcept : impl_{owner} {}  // NOLINT(*-explicit-*)

  /// Returns a reference to the borrowed value.
  constexpr T& Get() const noexcept { return impl_.Get(); }
  /// Returns a reference to the borrowed value.
  constexpr T& operator*() const noexcept { return Get(); }
  /// Returns a pointer to the borrowed value.
  constexpr T* operator->() const noexcept { return &Get(); }

 private:
  /// Type of `impl_`.
  using Impl = typename horus_internal::OwnedImpl<std::remove_const_t<T>, Mode>::Borrowed;

  /// The implementation of the object.
  Impl impl_;
};

template <class T, OwnedMode Mode>
constexpr Borrowed<T, Mode> Owned<T, Mode>::Borrow() noexcept {
  return *this;
}
template <class T, OwnedMode Mode>
constexpr Borrowed<const T, Mode> Owned<T, Mode>::Borrow() const noexcept {
  return *this;
}

}  // namespace horus

#endif  // HORUS_TYPES_OWNED_H_
