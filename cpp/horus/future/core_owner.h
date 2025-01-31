/// @file
///
/// The `CoreFutureOwner` class.

#ifndef HORUS_FUTURE_CORE_OWNER_H_
#define HORUS_FUTURE_CORE_OWNER_H_

#include "horus/event_loop/waker.h"
#include "horus/types/list.h"

namespace horus {

class PollContext;

namespace horus_internal {

class CoreFutureOwner;

/// Base class of `Handle`. Defined in this file since `CoreFutureOwner` depends on it, and to a
/// header dependency cycle.
class BaseHandle {
 public:
  /// Constructs a not-yet-engaged `BaseHandle`.
  BaseHandle() noexcept = default;

  /// Handles represent resources, and cannot be copied.
  BaseHandle(const BaseHandle&) = delete;
  /// Handles represent resources, and cannot be copied.
  BaseHandle& operator=(const BaseHandle&) = delete;

  /// Move constructor.
  BaseHandle(BaseHandle&&) noexcept = default;
  /// Move assignment operator.
  BaseHandle& operator=(BaseHandle&&) noexcept = default;

  /// The link which binds this `BaseHandle` to a `HandleList`.
  constexpr IntrusiveListLink<BaseHandle>& Link() & noexcept { return link_; }

  /// Destroys the handle, returning whether its destruction has completed. If not, `waker` should
  /// be used to later resume the destruction.
  virtual bool PollDestroy(const FutureWaker& waker) & noexcept = 0;

 protected:
  /// Connects the handle to the given owner.
  void OwnSelf(CoreFutureOwner& owner) & noexcept;

  /// Disconnects the handle from its current owner. `OwnSelf()` must have been called before.
  void DisownSelf() & noexcept;

  /// Destroys the handle. `protected` to ensure the virtual class can only be destroyed through a
  /// derived class.
  ~BaseHandle() noexcept = default;

 private:
  /// The owner of this handle.
  CoreFutureOwner* owner_{nullptr};
  /// @see Link()
  IntrusiveListLink<BaseHandle> link_{};
};

/// A list of `BaseHandle`s.
using HandleList = IntrusiveList<BaseHandle, &BaseHandle::Link>;

/// Core state of a `FutureOwner<F>`. For more information, see `FutureOwner<F>`.
///
/// You probably want to use `FutureOwner<F>` instead of this class; the only component which should
/// use `CoreFutureOwner` directly is the event loop in order to break a header dependency cycle.
class CoreFutureOwner {
 public:
  /// Constructs an empty owner.
  CoreFutureOwner() noexcept = default;

  /// Non-copyable.
  CoreFutureOwner(const CoreFutureOwner&) = delete;
  /// Non-copyable.
  CoreFutureOwner& operator=(const CoreFutureOwner&) = delete;
  /// Move constructor; this may not be called after initializing the owner.
  CoreFutureOwner(CoreFutureOwner&&) noexcept = default;
  /// Non-move-assignable.
  CoreFutureOwner& operator=(CoreFutureOwner&&) = delete;

  /// Destroys the `CoreFutureOwner`. It is illegal for the `CoreFutureOwner` to be destroyed before
  /// `PollDestroy()` returns a ready `PollResult`. Failure to respect this rule will lead to broken
  /// assertions in debug builds, and use-after-free's in release builds.
  ~CoreFutureOwner() noexcept;

  /// Initializes the `CoreFutureOwner`. This will connect it to its parent.
  void InitializeChild(PollContext& context) & noexcept;

  /// Destroys the future, returning whether its destruction has completed. If not, `waker` will be
  /// used to later resume the destruction.
  bool PollDestroy(const FutureWaker& waker) & noexcept;

  /// Returns a reference to the list of handles owned by the future.
  constexpr HandleList& Handles() & noexcept { return handles_; }

  /// Returns whether the owned future has started to execute in some way.
  constexpr bool DidStartExecution() const& noexcept {
    return parent_ != nullptr || destroy_started_ || !children_.IsEmpty();
  }

  /// Returns whether the destruction of the future was started with a call to `PollDestroy()`.
  constexpr bool DidStartDestruction() const& noexcept { return destroy_started_; }

  /// Returns whether the destruction of the future and its resources has completed.
  constexpr bool DidCompleteDestruction() const& noexcept {
    return destroy_started_ && handles_.IsEmpty() && children_.IsEmpty();
  }

 private:
  /// Destroys the handles of the future.
  bool PollDestroyHandles(const FutureWaker& waker) & noexcept;

  /// Returns a reference to `parent_link_`.
  constexpr IntrusiveListLink<CoreFutureOwner>& ParentLink() & noexcept { return parent_link_; }

  /// A pointer to the parent `CoreFutureOwner`, or `nullptr` if this is the root.
  CoreFutureOwner* parent_{nullptr};
  /// The link of this owner in its parent's `children_`, if it is not the root.
  IntrusiveListLink<CoreFutureOwner> parent_link_;
  /// The list of `CoreFutureOwner`s owned by this owner.
  IntrusiveList<CoreFutureOwner, &CoreFutureOwner::ParentLink> children_;
  /// The list of handles managed by this owner.
  HandleList handles_;
  /// Whether `PollDestroy()` was called at least once.
  bool destroy_started_{false};
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_FUTURE_CORE_OWNER_H_
