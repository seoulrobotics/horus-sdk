#include "horus/future/core_owner.h"

#include <cassert>

#include "horus/event_loop/waker.h"
#include "horus/future/poll.h"

namespace horus {
namespace horus_internal {

void BaseHandle::OwnSelf(CoreFutureOwner& owner) & noexcept {
  assert(owner_ == nullptr);
  owner_ = &owner;
  owner.Handles().PushBack(*this);
}

void BaseHandle::DisownSelf() & noexcept {
  assert(owner_ != nullptr);
  owner_->Handles().Remove(*this);
}

void CoreFutureOwner::InitializeChild(PollContext& context) & noexcept {
  if (parent_ == &context.FutureOwner()) {
    // Already initialized.
    return;
  }
  assert(!DidStartExecution());
  parent_ = &context.FutureOwner();
  assert(!parent_->destroy_started_);
  parent_->children_.PushBack(*this);
}

CoreFutureOwner::~CoreFutureOwner() noexcept {
  assert(destroy_started_ && "FutureOwner destroyed before calling PollDestroy()");
  assert(DidCompleteDestruction() && "FutureOwner destroyed before PollDestroy() completed");
}

bool CoreFutureOwner::PollDestroy(const FutureWaker& waker) & noexcept {
  // We're not allowed to recurse, so we have to manually walk the tree.

  /// Returns the leftmost leaf node under `ptr`.
  ///
  /// For instance, given
  ///
  /// ```
  ///        A
  ///       / `
  ///      B   F
  ///     / `
  ///    C   D
  ///       /
  ///      E
  /// ```
  ///
  /// This function's calls will map:
  ///  - `A | B | C -> C`
  ///  - `D | E     -> E`
  ///  - `F         -> F`
  const auto leftmost_leaf_of = +[](CoreFutureOwner* ptr) noexcept -> CoreFutureOwner* {
    // Go to the first leaf descendant that hasn't been destroyed yet.
    while (ptr->children_.Front() != nullptr && !ptr->children_.Front()->destroy_started_) {
      // We disallow initializing new children once `destroy_started_` is true, so we don't need to
      // worry about visiting children of `owner` for which `destroy_started_` is false.
      ptr = ptr->children_.Front();
    }
    return ptr;
  };

  /// Returns whether the node is fully destroyed (including descendants and handles).
  const auto is_fully_destroyed = [&waker](CoreFutureOwner& owner) noexcept -> bool {
    return owner.PollDestroyHandles(waker) && owner.children_.IsEmpty();
  };

  destroy_started_ = true;
  CoreFutureOwner* owner{leftmost_leaf_of(this)};
  while (owner != this) {
    // Poll-destroy the node.
    owner->destroy_started_ = true;
    CoreFutureOwner* const next_sibling{owner->ParentLink().next};
    if (is_fully_destroyed(*owner)) {
      // If the node is fully destroyed, remove it from its parent. `parent_` is non-null because
      // the root of the current call is `this`, and we checked that `owner != this` above.
      owner->parent_->children_.Remove(*owner);
    }

    // Move on to the next node to process.
    if (next_sibling != nullptr) {
      // The next node is the next sibling; move to its leftmost leaf before proceeding.
      owner = leftmost_leaf_of(next_sibling);
    } else {
      // The next node is the parent; at this point all its children have been processed, so we can
      // safely process the parent itself.
      owner = owner->parent_;
    }
  }
  if (!is_fully_destroyed(*this)) {
    return false;
  }
  if (parent_ != nullptr) {
    parent_->children_.Remove(*this);
  }
  return true;
}

bool CoreFutureOwner::PollDestroyHandles(const FutureWaker& waker) & noexcept {
  bool all_destroyed{true};
  BaseHandle* handle{handles_.Front()};
  while (handle != nullptr) {
    BaseHandle* const next{handle->Link().next};
    if (!handle->PollDestroy(waker)) {
      all_destroyed = false;
    }
    handle = next;
  }
  return all_destroyed;
}

}  // namespace horus_internal
}  // namespace horus
