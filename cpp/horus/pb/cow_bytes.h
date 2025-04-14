/// @file
///
/// The `CowBytes` class.

#ifndef HORUS_PB_COW_BYTES_H_
#define HORUS_PB_COW_BYTES_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <protozero/iterators.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>
#include <string>
#include <utility>
#include <vector>

#include "horus/attributes.h"
#include "horus/pb/buffer.h"
#include "horus/pb/serialize.h"
#include "horus/strings/string_view.h"
#include "horus/strings/stringify.h"
#include "horus/types/one_of.h"

namespace horus {

/// A string which either points to a `PbBuffer` or to an allocated string.
///
/// If the object references a `PbView` or `StringView`, attempting to mutate it will
/// replace its contents with a `std::string` it owns.
///
/// Note that copying a `CowBytes` internally storing an owned `std::string` (because it was
/// mutated) will make a copy of the string.
class CowBytes final {
  /// Type of the inner `data_`.
  using Data = OneOf<PbView, std::vector<std::uint8_t>>;

 public:
  /// The type of an owned buffer.
  using Owned = std::vector<std::uint8_t>;

  /// Returns a `CowBytes` container which refers to the data in `string`.
  static CowBytes Borrowed(StringView string HORUS_LIFETIME_BOUND) noexcept {
    return CowBytes{string};
  }

  /// Returns a `CowBytes` container which contains an owned copy of the given `string`.
  ///
  /// @throws std::bad_alloc If the copy fails.
  static CowBytes OwnedCopy(StringView string) noexcept(false);

  /// Returns a `CowBytes` container which contains a shared copy of the given `string`.
  ///
  /// @throws std::bad_alloc If the copy fails.
  static CowBytes SharedCopy(StringView string) noexcept(false);

  /// Creates an empty bytes container.
  CowBytes() noexcept : data_{InPlaceType<PbView>, PbBuffer{}} {}

  /// Creates a `CowBytes` container which wraps a shared `view`.
  explicit CowBytes(PbView&& view) noexcept : data_{InPlaceType<PbView>, std::move(view)} {}

  /// Creates a `CowBytes` container with its own mutable data.
  explicit CowBytes(Owned&& data) noexcept : data_{InPlaceType<Owned>, std::move(data)} {}

  /// Creates a `CowBytes` by deserializing from `reader`.
  explicit CowBytes(PbReader& reader) noexcept : CowBytes{reader.View()} {}

  /// Creates a `CowBytes container which refers to a string literal.
  template <std::size_t N>
  // NOLINTNEXTLINE(*-avoid-c-arrays, *-explicit-*)
  CowBytes(const char (&literal)[N]) noexcept : CowBytes{StringView{literal, N - 1}} {
    assert(std::strlen(literal) == N - 1);
  }

  /// Returns a reference to the stored bytes.
  StringView Str() const noexcept HORUS_LIFETIME_BOUND;

  /// Returns a mutable reference to the stored bytes.
  ///
  /// @throws std::bad_alloc If the contents of the string had to be copied and the allocation
  /// failed.
  Owned& String() & noexcept(false) HORUS_LIFETIME_BOUND;

  /// Returns a view of the stored bytes, making a copy of the inner string first if necessary.
  ///
  /// @throws std::bad_alloc If the contents of the string had to be copied and the allocation
  /// failed.
  PbView View() const& noexcept(false);

  /// Returns a view of the stored bytes.
  PbView View() && noexcept;

 private:
  /// Constructs a `CowBytes` which refers to a borrowed string.
  explicit CowBytes(StringView borrowed) noexcept
      : data_{InPlaceType<PbView>, PbBuffer::Borrowed(borrowed)} {}

  /// The actual data.
  Data data_;
};

template <>
class PbTraits<CowBytes> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, const CowBytes& value) {
    writer.Writer().add_bytes(tag, value.Str().data(), value.Str().size());
  }

  /// Deserializes a `CowBytes` from `reader`.
  static CowBytes Deserialize(PbReader& reader) {
    return CowBytes{reader.View(reader.Reader().get_view())};
  }
};

/// Appends `cow_bytes` to `sink`.
template <class Sink>
void HorusStringify(Sink& sink, const CowBytes& cow_bytes) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(cow_bytes.Str());
}

}  // namespace horus

#endif  // HORUS_PB_COW_BYTES_H_
