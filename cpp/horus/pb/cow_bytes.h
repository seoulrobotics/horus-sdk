/// @file
///
/// The `CowBytes` class.

#ifndef HORUS_PB_COW_BYTES_H_
#define HORUS_PB_COW_BYTES_H_

#include <cassert>
#include <cstddef>
#include <cstring>
#include <protozero/iterators.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>
#include <string>
#include <utility>

#include "horus/internal/attributes.h"
#include "horus/pb/buffer.h"
#include "horus/pb/serialize.h"
#include "horus/strings/str_sink.h"
#include "horus/types/one_of.h"
#include "horus/types/string_view.h"

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
  using Data = OneOf<PbView, std::string>;

 public:
  /// Returns a `CowBytes` container which refers to the data in `string`.
  static CowBytes Borrowed(StringView string HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    return CowBytes{string};
  }

  /// Returns a `CowBytes` container which contains a copy of the given `string`.
  ///
  /// @throws std::bad_alloc If the copy fails.
  static CowBytes Copied(StringView string) noexcept(false) {
    return CowBytes{std::string{string}};
  }

  /// Creates an empty bytes container.
  CowBytes() noexcept : data_{InPlaceType<PbView>, PbBuffer{}} {}

  /// Creates a `CowBytes` container which wraps a shared `view`.
  explicit CowBytes(PbView&& view) noexcept : data_{InPlaceType<PbView>, std::move(view)} {}

  /// Creates a `CowBytes` container with its own mutable data.
  explicit CowBytes(std::string&& string) noexcept
      : data_{InPlaceType<std::string>, std::move(string)} {}

  /// Creates a `CowBytes` by deserializing from `reader`.
  explicit CowBytes(PbReader& reader) noexcept : CowBytes{reader.View()} {}

  /// Creates a `CowBytes container which refers to a string literal.
  template <std::size_t N>
  // NOLINTNEXTLINE(*-avoid-c-arrays, *-explicit-*)
  CowBytes(const char (&literal)[N]) noexcept : CowBytes{StringView{literal, N - 1}} {
    assert(std::strlen(literal) == N - 1);
  }

  /// Returns a reference to the stored bytes.
  StringView Str() const noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

  /// Returns a mutable reference to the stored bytes.
  ///
  /// @throws std::bad_alloc If the contents of the string had to be copied and the allocation
  /// failed.
  std::string& String() noexcept(false) HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

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
