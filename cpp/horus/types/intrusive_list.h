/// @file
///
/// Helper for defining linked lists of immovable items.

#ifndef HORUS_TYPES_INTRUSIVE_LIST_H_
#define HORUS_TYPES_INTRUSIVE_LIST_H_

#include <cassert>
#include <cstddef>

#include "horus/attributes.h"

namespace horus {

/// A link in an `IntrusiveList`.
template <class T>
struct IntrusiveListLink {
  /// A pointer to the previous node. If null, the owning node is the `tail_`.
  T* prev{nullptr};
  /// A pointer to the next node. If null, the owning node is the `head_`.
  T* next{nullptr};
};

/// A doubly linked list whose (immovable) items are stored inline in owner structures.
template <class T, IntrusiveListLink<T>& (*GetLink)(T&) noexcept>
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
    IntrusiveListLink<T>& link{GetLink(*front)};
    assert(link.prev == nullptr);
    head_ = link.next;
    if (head_ == nullptr) {
      // List is now empty.
      assert(tail_ == front);
      tail_ = nullptr;
    } else {
      GetLink(*head_).prev = nullptr;
    }
    link.prev = nullptr;
    link.next = nullptr;
    return front;
  }

  /// Pushes `node` to the end of the list. If it already belongs to a list, it will be removed from
  /// it.
  void PushBack(T& node HORUS_LIFETIME_CAPTURE_BY(this)) noexcept {
    IntrusiveListLink<T>& link{GetLink(node)};
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
      GetLink(*tail_).next = &node;
      tail_ = &node;
    }
  }

  /// Pushes `node` to the start of the list. If it already belongs to a list, it will be removed
  /// from it.
  void PushFront(T& node HORUS_LIFETIME_CAPTURE_BY(this)) noexcept {
    IntrusiveListLink<T>& link{GetLink(node)};
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
      link.next = head_;
      GetLink(*head_).prev = &node;
      head_ = &node;
    }
  }

  /// Removes `node` from the list. No-op if `node` is not part of the list.
  void Remove(T& node HORUS_LIFETIME_CAPTURE_BY(this)) noexcept {
    IntrusiveListLink<T>& link{GetLink(node)};

    // Update previous link (and `head_` if this node is the first one).
    if (head_ == &node) {
      // Node is the first one.
      assert(link.prev == nullptr);
      head_ = link.next;
    } else if (link.prev != nullptr) {
      // Node is within the list, but not the first.
      GetLink(*link.prev).next = link.next;
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
      GetLink(*link.next).prev = link.prev;
    } else {
      // Node is not part of the list.
      assert(link.prev == nullptr);
    }
    link.prev = nullptr;
    link.next = nullptr;
  }

  /// Returns the number of nodes in the list. This will traverse the whole list.
  std::size_t Count() const noexcept {
    std::size_t count{0};
    T* node{head_};
    while (node != nullptr) {
      ++count;
      node = GetLink(*node).next;
    }
    return count;
  }

 private:
  /// A pointer to the first node, aka the head/front. Null iff `tail_` is null.
  T* head_{nullptr};
  /// A pointer to the last node, aka the back/tail. Null iff `head_` is null.
  T* tail_{nullptr};
};

/// Returns the `IntrusiveListLink` of a node given a field. Prefer using `IntrusiveListByField`
/// directly instead.
template <class T, IntrusiveListLink<T>(T::*LinkField)>
constexpr IntrusiveListLink<T>& IntrusiveListLinkFromField(T& node HORUS_LIFETIME_BOUND) noexcept {
  return node.*LinkField;
}

/// An `IntrusiveList` whose link is made available by a field of `T`.
template <class T, IntrusiveListLink<T>(T::*LinkField) = &T::link>
using IntrusiveListByField = IntrusiveList<T, &IntrusiveListLinkFromField<T, LinkField>>;

/// Returns the `IntrusiveListLink` of a node given a field. Prefer using `IntrusiveListByField`
/// directly instead.
template <class T, IntrusiveListLink<T>& (T::*LinkMethod)() & noexcept>
constexpr IntrusiveListLink<T>& IntrusiveListLinkFromMethod(T& node HORUS_LIFETIME_BOUND) noexcept {
  return (node.*LinkMethod)();
}

/// An `IntrusiveList` whose link is made available by a field of `T`.
template <class T, IntrusiveListLink<T>& (T::*LinkMethod)()& noexcept = &T::Link>
using IntrusiveListByMethod = IntrusiveList<T, &IntrusiveListLinkFromMethod<T, LinkMethod>>;

}  // namespace horus

#endif  // HORUS_TYPES_INTRUSIVE_LIST_H_
