/// @file
///
/// Helper for defining linked lists of immovable items.

#ifndef HORUS_TYPES_LIST_H_
#define HORUS_TYPES_LIST_H_

#include <cassert>

#include "horus/internal/attributes.h"

namespace horus {
namespace horus_internal {

/// A link in an `IntrusiveList`.
template <class T>
struct IntrusiveListLink {
  /// A pointer to the previous node. If null, the owning node is the `tail_`.
  T* prev{nullptr};
  /// A pointer to the next node. If null, the owning node is the `head_`.
  T* next{nullptr};

  /// Returns whether the link belongs to an `IntrusiveList`.
  constexpr bool IsLinked() const noexcept { return prev != nullptr || next != nullptr; }
};

/// A doubly linked list whose (immovable) items are stored inline in owner structures.
template <class T, IntrusiveListLink<T>& (T::*LinkField)()& noexcept>
class IntrusiveList final {
 public:
  /// Constructs an empty list.
  constexpr IntrusiveList() noexcept = default;

  /// Returns whether the list is empty.
  constexpr bool IsEmpty() const noexcept { return head_ == nullptr; }

  /// Returns a pointer to the node at the front, or nullptr if the list `IsEmpty()`.
  constexpr T* Front() noexcept { return head_; }

  /// Returns the pointer to the front node and removes it from the list. If the list `IsEmpty()`,
  /// returns `nullptr`.
  T* PopFront() noexcept {
    T* const front{head_};
    if (front == nullptr) {
      return nullptr;
    }
    IntrusiveListLink<T>& link{(front->*LinkField)()};
    assert(link.prev == nullptr);
    head_ = link.next;
    if (head_ == nullptr) {
      // List is now empty.
      assert(tail_ == front);
      tail_ = nullptr;
    } else {
      (head_->*LinkField)().prev = nullptr;
    }
    link.prev = nullptr;
    link.next = nullptr;
    return front;
  }

  /// Pushes `node` to the end of the list. If it already belongs to a list, it will be removed from
  /// it.
  void PushBack(T& node HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    IntrusiveListLink<T>& link{(node.*LinkField)()};
    if ((link.prev != nullptr) || (link.next != nullptr) || (tail_ == &node)) {
      // Node is in a list (has siblings or is the only member of its list).
      Remove(node);
    }
    if (tail_ == nullptr) {
      // Insert first node.
      assert(head_ == nullptr);
      head_ = &node;
      tail_ = &node;
    } else {
      assert(head_ != nullptr);
      link.prev = tail_;
      (tail_->*LinkField)().next = &node;
      tail_ = &node;
    }
  }

  /// Removes `node` from the list.
  void Remove(T& node) noexcept {
    IntrusiveListLink<T>& link{(node.*LinkField)()};

    // Update previous link (and `head_` if this node is the first one).
    if (head_ == &node) {
      // Node is the first one.
      assert(link.prev == nullptr);
      head_ = link.next;
    } else if (link.prev != nullptr) {
      // Node is within the list, but not the first.
      (link.prev->*LinkField)().next = link.next;
    } else {
      // Node is not part of the list.
      assert(link.next == nullptr);
    }

    // Update next link (and `tail_` if this node is the last one).
    if (tail_ == &node) {
      // Node is the last one.
      assert(link.next == nullptr);
      tail_ = link.prev;
    } else if (link.next != nullptr) {
      // Node is within the list, but not the last.
      (link.next->*LinkField)().prev = link.prev;
    } else {
      // Node is not part of the list.
      assert(link.prev == nullptr);
    }
    link.prev = nullptr;
    link.next = nullptr;
  }

 private:
  /// A pointer to the first node, aka the head/front. Null iff `tail_` is null.
  T* head_{nullptr};
  /// A pointer to the last node, aka the back/tail. Null iff `head_` is null.
  T* tail_{nullptr};
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_TYPES_LIST_H_
