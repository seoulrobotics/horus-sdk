/// @file
///
/// The `Bitset` class.

#ifndef HORUS_INTERNAL_BITSET_H_
#define HORUS_INTERNAL_BITSET_H_

#include <cstddef>
#include <cstdint>
#include <memory>

namespace horus {
namespace horus_internal {

/// A bitset with fixed capacity.
template <class Chunk = std::uint32_t>
class FixedBitset final {
 public:
  /// Creates a `FixedBitset` which can store `size` bits.
  explicit FixedBitset(std::size_t size) noexcept(false)
      : size_{size}, chunks_{std::make_unique<Chunk[]>(size)} {}  // NOLINT(*-avoid-c-arrays)

  /// The size of the bitset as chosen when constructing it.
  constexpr std::size_t Size() const noexcept { return size_; }

  /// Returns whether the bit at the given `index` is set.
  constexpr bool Get(std::size_t index) const noexcept {
    return (chunks_[ChunkOffset(index)] & Bit(index)) != 0;
  }

  /// Sets the value of the bit at the given `index`.
  constexpr void Set(std::size_t index, bool value) noexcept {
    if (value) {
      chunks_[ChunkOffset(index)] |= Bit(index);
    } else {
      chunks_[ChunkOffset(index)] &= ~Bit(index);
    }
  }

  /// Returns any index such that `Get(index) == 0`. If no such index is available, returns
  /// `Size()`.
  constexpr std::size_t AnyOffIndex() const noexcept {
    // If needed, we could optimize this with `__builtin_clz()` later on.
    for (std::size_t i{0}; i < size_; ++i) {
      if (!Get(i)) {
        return i;
      }
    }
    return size_;
  }

 private:
  /// Number of bits in a byte.
  static constexpr std::size_t kBitsPerByte{8};
  /// Number of bits in a `Chunk`.
  static constexpr std::size_t kBitsPerChunk{sizeof(Chunk) * kBitsPerByte};

  /// Returns the number of `Chunk`s needed to store `capacity` bits.
  static constexpr std::size_t RequiredChunksToStore(std::size_t capacity) noexcept {
    return (capacity + kBitsPerChunk - 1) / kBitsPerChunk;
  }

  /// Returns the offset in `chunks_` of the chunk which stores the bit at `index`.
  static constexpr std::size_t ChunkOffset(std::size_t index) noexcept {
    return index / kBitsPerChunk;
  }
  /// Returns the offset in a `Chunk` of the bit which stores `index`.
  static constexpr std::size_t BitOffset(std::size_t index) noexcept {
    return index % kBitsPerChunk;
  }
  /// Returns a `Chunk` with the `index`th bit set.
  static constexpr Chunk Bit(std::size_t index) noexcept {
    return static_cast<Chunk>(1U << BitOffset(index));
  }

  /// The size of the bitset.
  std::size_t size_;
  /// The allocated chunks.
  std::unique_ptr<Chunk[]> chunks_;  // NOLINT(*-avoid-c-arrays)
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_BITSET_H_
