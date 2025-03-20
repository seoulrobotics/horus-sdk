/// @file
///
/// The `PbBuffer` and `PbView` classes.

#ifndef HORUS_PB_BUFFER_H_
#define HORUS_PB_BUFFER_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "horus/internal/attributes.h"
#include "horus/types/string_view.h"

namespace horus {

class PbView;

/// A read-only reference to a buffer which stores a Protobuf message.
class PbBuffer final {
 public:
  /// Constructs a `PbBuffer` which refers to an existing buffer without copying it.
  static PbBuffer Borrowed(StringView borrowed HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    return PbBuffer{borrowed};
  }

  /// Constructs a `PbBuffer` which refers to the copy of the given string.
  static PbBuffer Copy(StringView string) noexcept(false);

  /// Constructs an empty `PbBuffer`.
  PbBuffer() noexcept : buffer_{nullptr}, size_{0} {}

  /// Constructs a `PbBuffer` which refers to a `std::string`.
  ///
  /// @throws std::bad_alloc If the shared state cannot be allocated.
  explicit PbBuffer(std::string&& buffer) noexcept(false);

  /// Constructs a `PbBuffer` which refers to a `std::vector<std::uint8_t>`.
  ///
  /// @throws std::bad_alloc If the shared state cannot be allocated.
  explicit PbBuffer(std::vector<std::uint8_t>&& buffer) noexcept(false);

  /// Returns a read-only view into the buffer.
  StringView Str() const noexcept { return {buffer_.get(), size_}; }

  /// Returns a view into the whole buffer.
  inline PbView View() const noexcept;

  /// Returns a view into a portion of the buffer.
  inline PbView View(std::size_t offset, std::size_t size) const noexcept;

 private:
  /// Constructs a `PbBuffer` which refers to a borrowed string.
  ///
  /// This uses the _aliasing constructor_ of `std::shared_ptr` with an empty source
  /// `std::shared_ptr`. This is valid, and leads to a `std::shared_ptr` that simply refers to
  /// `borrowed.data()` without allocating or managing any memory.
  explicit PbBuffer(StringView borrowed) noexcept
      : buffer_{std::shared_ptr<void>{}, borrowed.data()}, size_{borrowed.size()} {}

  /// Constructs a `PbBuffer` which refers to a string.
  explicit PbBuffer(std::shared_ptr<const char>&& buffer, std::size_t size) noexcept
      : buffer_{std::move(buffer)}, size_{size} {}

  /// A shared pointer to the underlying buffer.
  std::shared_ptr<const char> buffer_;

  /// The size of the buffer.
  std::size_t size_;
};

/// A view into a `PbBuffer`.
class PbView final {
 public:
  /// Constructs a `PbView` which refers to a whole `PbBuffer`.
  explicit PbView(PbBuffer&& buffer) noexcept
      : buffer_{std::move(buffer)}, offset_{0}, size_{buffer_.Str().size()} {}

  /// Constructs a `PbView` which refers to a portion of a `PbBuffer`.
  PbView(PbBuffer&& buffer, std::size_t offset, std::size_t size) noexcept
      : buffer_{std::move(buffer)}, offset_{offset}, size_{size} {
    assert(offset_ + size <= buffer_.Str().size());
  }

  /// Returns a reference to the underlying `PbBuffer`.
  constexpr const PbBuffer& Buffer() const& noexcept { return buffer_; }

  /// Returns a reference to the underlying `PbBuffer`.
  constexpr PbBuffer&& Buffer() && noexcept { return std::move(buffer_); }

  /// Returns the view as a `StringView`.
  // NOLINTNEXTLINE(*-exception-escape): `offset_`/`size_` are checked in constructor
  StringView Str() const noexcept { return buffer_.Str().substr(offset_, size_); }

  /// Returns the offset in `Buffer()` where the view begins.
  constexpr std::size_t Offset() const noexcept { return offset_; }

  /// Returns the size of the view in `Buffer()`.
  constexpr std::size_t Size() const noexcept { return size_; }

  /// Returns a subview starting at `offset`.
  PbView View(std::size_t offset) const noexcept {
    assert(offset <= size_);
    return buffer_.View(offset_ + offset, Size() - offset);
  }

  /// Returns a subview starting at `offset` and with the given `size`.
  PbView View(std::size_t offset, std::size_t size) const noexcept {
    assert(size <= size_);
    return buffer_.View(offset_ + offset, size);
  }

 private:
  /// The underlying buffer.
  PbBuffer buffer_;
  /// The offset from the start of the buffer.
  std::size_t offset_;
  /// The size of the view.
  std::size_t size_;
};

inline PbView PbBuffer::View() const noexcept { return PbView{PbBuffer{*this}}; }

inline PbView PbBuffer::View(std::size_t offset, std::size_t size) const noexcept {
  return PbView{PbBuffer{*this}, offset, size};
}

}  // namespace horus

#endif  // HORUS_PB_BUFFER_H_
