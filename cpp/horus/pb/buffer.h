/// @file
///
/// The `PbBuffer` and `PbView` classes.

#ifndef HORUS_PB_BUFFER_H_
#define HORUS_PB_BUFFER_H_

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>

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
  static PbBuffer Copy(StringView borrowed) noexcept(false) {
    return PbBuffer{std::string{borrowed}};
  }

  /// Constructs an empty `PbBuffer`.
  PbBuffer() noexcept : ownership_{nullptr}, str_{} {}

  /// Constructs a `PbBuffer` which refers to a `std::string`.
  ///
  /// @throws std::bad_alloc If the shared state cannot be allocated.
  explicit PbBuffer(std::string&& buffer) noexcept(false)
      : ownership_{std::make_shared<std::string>(std::move(buffer))}, str_{*ownership_} {}

  /// Returns a read-only view into the buffer.
  constexpr StringView Str() const noexcept { return str_; }

  /// Returns a view into the whole buffer.
  inline PbView View() const noexcept;

  /// Returns a view into a portion of the buffer.
  inline PbView View(std::size_t offset, std::size_t size) const noexcept;

 private:
  /// Constructs a `PbBuffer` which refers to a borrowed string.
  explicit PbBuffer(StringView borrowed) noexcept : ownership_{nullptr}, str_{borrowed} {}

  /// A shared pointer to the string referenced by `str_`. This may be `nullptr`, in which case the
  /// string is borrowed.
  std::shared_ptr<const std::string> ownership_;

  /// A view into the buffer.
  StringView str_;
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
  constexpr StringView Str() const noexcept { return buffer_.Str().substr(offset_, size_); }

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
