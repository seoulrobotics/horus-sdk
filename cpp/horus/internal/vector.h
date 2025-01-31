/// @file
///
/// Helpers related to `std::vector<T>`.

#ifndef HORUS_INTERNAL_VECTOR_H_
#define HORUS_INTERNAL_VECTOR_H_

#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

namespace horus {
namespace horus_internal {

/// Removes the element at `index` from the given `vector`, replacing it by the last element.
///
/// @returns true if the removal succeeded, false if `index` is out of bounds.
///
/// This is more efficient than `vector.erase()` because elements after `index` do not have to be
/// reordered. It _does_ mean that ordering of the vector is not preserved.
///
/// Pointers into the vector will not be invalidated **except** for the last element which is now
/// at `index`.
///
/// If `index` is the index of the last element, this is equivalent to `vector.pop_back()`.
template <class T, class A>
bool SwapRemove(std::vector<T, A>& vector, std::size_t index) noexcept {
  static_assert(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_destructible<T>::value,
                "");

  if (index >= vector.size()) {
    return false;
  }
  if (index != vector.size() - 1) {
    std::swap(vector[index], vector.back());
  }
  vector.pop_back();
  return true;
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_VECTOR_H_
