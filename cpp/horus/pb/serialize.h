/// @file
///
/// Classes used for Protobuf deserialization: `PbReader`, `PbWriter`, `PbTraits` and
/// `PbEnumTraits`.

#ifndef HORUS_PB_SERIALIZE_H_
#define HORUS_PB_SERIALIZE_H_

#include <cstddef>
#include <cstdint>
#include <protozero/basic_pbf_writer.hpp>
#include <protozero/data_view.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/types.hpp>
#include <protozero/varint.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"
#include "horus/pb/buffer.h"
#include "horus/pb/pbf_buffer_specialization.h"  // IWYU pragma: keep
#include "horus/pb/types.h"

namespace horus {

class PbMessage;

/// Helper class used to deserialize Protobuf messages from a `PbBuffer`.
class PbReader final {
 public:
  /// Constructs a `PbReader` which deserializes from `buffer`.
  explicit PbReader(PbBuffer&& buffer) noexcept
      : buffer_{std::move(buffer)}, reader_{buffer_.Str().data(), buffer_.Str().size()} {}

  /// Constructs a `PbReader` which deserializes from `view`.
  explicit PbReader(PbView&& view) noexcept : buffer_{}, reader_{} {
    std::size_t const offset{view.Offset()};
    std::size_t const size{view.Size()};
    buffer_ = std::move(view).Buffer();  // NOLINT(*-prefer-member-initializer): cannot due to move
    reader_ = {horus_internal::PointerAdd(buffer_.Str().data(), offset), size};
  }

  /// Constructs a `PbReader` which deserializes a submessage of `parent`.
  PbReader(PbReader& parent, PbTag /* tag */) noexcept
      : buffer_{parent.buffer_}, reader_{parent.reader_.get_message()} {}

  // We disallow copies and moves since `reader_` refers to `buffer_`.

  /// `PbWriter`s cannot be copied or moved.
  PbReader(const PbReader&) = delete;
  /// `PbReader`s cannot be copied or moved.
  PbReader(PbReader&&) = delete;
  /// `PbReader`s cannot be copied or moved.
  PbReader& operator=(const PbReader&) = delete;
  /// `PbReader`s cannot be copied or moved.
  PbReader& operator=(PbReader&&) = delete;

  /// Destructor.
  ~PbReader() noexcept = default;

  /// Returns a reference to the input `Buffer()`.
  constexpr const PbBuffer& Buffer() const noexcept { return buffer_; }

  /// Returns a reference to the underlying `pbf_reader`.
  ///
  /// See
  /// https://github.com/mapbox/protozero/blob/master/doc/tutorial.md#parsing-protobuf-encoded-messages
  /// for documentation.
  constexpr protozero::pbf_reader& Reader() noexcept { return reader_; }

  /// Returns a `PbView` which wraps the entire view.
  PbView View() const noexcept {
    return buffer_.View(static_cast<std::size_t>(reader_.data().data() - buffer_.Str().data()),
                        reader_.data().size());
  }

  /// Returns a `PbView` which wraps the bytes, string or message represented by `view`.
  PbView View(protozero::data_view view) const noexcept {
    return buffer_.View(static_cast<std::size_t>(view.data() - buffer_.Str().data()), view.size());
  }

  /// Returns a `PbView` which wraps the given tag followed by the bytes, string or message
  /// represented by `view`.
  PbView ViewIncludingTag(PbTag tag, protozero::data_view view) const noexcept;

  /// Skips until the end of the current message.
  void SkipMessage() {
    while (Reader().next()) {
      Reader().skip();
    }
  }

 private:
  PbBuffer buffer_;
  protozero::pbf_reader reader_;
};

/// Helper class used to serialize Protobuf messages to a `PbBuffer`.
class PbWriter final {
 public:
  /// The output buffer type.
  using Buffer = std::vector<std::uint8_t>;
  /// The Protobuf writer type.
  using PbfWriter = protozero::basic_pbf_writer<Buffer>;

  /// Constructs an empty `PbWriter`.
  PbWriter() noexcept : writer_{buffer_} {}

  /// Constructs a `PbWriter` which will append its content to the given `output`.
  explicit PbWriter(Buffer&& output) noexcept : buffer_{std::move(output)}, writer_{buffer_} {}

  /// Constructs an empty `PbWriter` with the given `initial_capacity`.
  explicit PbWriter(std::size_t initial_capacity) noexcept(false) : PbWriter{} {
    buffer_.reserve(initial_capacity);
  }

  /// Constructs a `PbWriter` used to encode a submessage in `parent`.
  PbWriter(PbWriter& parent, PbTag tag) noexcept : writer_{parent.writer_, tag} {}

  // We disallow copies and moves since `writer_` refers to `buffer_`.

  /// `PbWriter`s cannot be copied or moved.
  PbWriter(const PbWriter&) = delete;
  /// `PbWriter`s cannot be copied or moved.
  PbWriter(PbWriter&&) = delete;
  /// `PbWriter`s cannot be copied or moved.
  PbWriter& operator=(const PbWriter&) = delete;
  /// `PbWriter`s cannot be copied or moved.
  PbWriter& operator=(PbWriter&&) = delete;

  /// Destructor.
  ~PbWriter() noexcept = default;

  /// Returns a reference to the underlying `pbf_writer`.
  ///
  /// See
  /// https://github.com/mapbox/protozero/blob/master/doc/tutorial.md#writing-protobuf-encoded-messages
  /// for documentation.
  constexpr PbfWriter& Writer() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND { return writer_; }

  /// Returns the resulting string.
  Buffer ToVector() && noexcept { return std::move(buffer_); }

  /// Returns the resulting buffer as a `PbBuffer`.
  PbBuffer ToBuffer() && noexcept(false) { return PbBuffer{std::move(buffer_)}; }

 private:
  /// The output string buffer.
  Buffer buffer_;
  /// The output `pbf_writer`.
  PbfWriter writer_;
};

/// Compile-time flags for Protobuf deserialization.
enum class PbDeserFlags : std::uint8_t {
  /// No flags.
  kNone = 0,
  /// Integer is stored as fixed, rather than varint.
  kFixed = 1,
};

/// Traits for (de)serialization of Protobuf types.
template <class T, PbDeserFlags kFlags = PbDeserFlags::kNone>
class PbTraits final {
 public:
  /// Serializes `message` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, const T& message) {
    static_assert(std::is_base_of<PbMessage, T>::value, "PbTraits<T> is not defined for T");

    if (message.IsEmpty()) {
      // protozero does not write anything for empty messages, but if `Serialize()` was called our
      // caller definitely wants something written, so we'll write an empty message manually.
      writer.Writer().add_message(tag, "", 0);
    } else {
      PbWriter for_message{writer, tag};
      message.SerializeTo(for_message);
    }
  }

  /// Deserializes a `T` from `reader`.
  static void Deserialize(PbReader& reader, T& message) {
    static_assert(std::is_base_of<PbMessage, T>::value, "PbTraits<T> is not defined for T");

    PbReader for_message{reader, 0};
    message.DeserializeFrom(for_message);
  }
};

template <>
class PbTraits<std::uint32_t> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::uint32_t value) {
    writer.Writer().add_uint32(tag, value);
  }

  /// Deserializes a `std::uint32_t` from `reader`.
  static std::uint32_t Deserialize(PbReader& reader) { return reader.Reader().get_uint32(); }
};

template <>
class PbTraits<std::uint32_t, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::uint32_t value) {
    writer.Writer().add_fixed32(tag, value);
  }

  /// Deserializes a `std::uint32_t` from `reader`.
  static std::uint32_t Deserialize(PbReader& reader) { return reader.Reader().get_fixed32(); }
};

template <>
class PbTraits<std::int32_t> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::int32_t value) {
    writer.Writer().add_int32(tag, value);
  }

  /// Deserializes a `std::int32_t` from `reader`.
  static std::int32_t Deserialize(PbReader& reader) { return reader.Reader().get_int32(); }
};

template <>
class PbTraits<std::int32_t, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::int32_t value) {
    writer.Writer().add_sfixed32(tag, value);
  }

  /// Deserializes a `std::int32_t` from `reader`.
  static std::int32_t Deserialize(PbReader& reader) { return reader.Reader().get_sfixed32(); }
};

template <>
class PbTraits<std::uint64_t> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::uint64_t value) {
    writer.Writer().add_uint64(tag, value);
  }

  /// Deserializes a `std::uint64_t` from `reader`.
  static std::uint64_t Deserialize(PbReader& reader) { return reader.Reader().get_uint64(); }
};

template <>
class PbTraits<std::uint64_t, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::uint64_t value) {
    writer.Writer().add_fixed64(tag, value);
  }

  /// Deserializes a `std::uint64_t` from `reader`.
  static std::uint64_t Deserialize(PbReader& reader) { return reader.Reader().get_fixed64(); }
};

template <>
class PbTraits<std::int64_t> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::int64_t value) {
    writer.Writer().add_int64(tag, value);
  }

  /// Deserializes a `std::int64_t` from `reader`.
  static std::int64_t Deserialize(PbReader& reader) { return reader.Reader().get_int64(); }
};

template <>
class PbTraits<std::int64_t, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, std::int64_t value) {
    writer.Writer().add_sfixed64(tag, value);
  }

  /// Deserializes a `std::int64_t` from `reader`.
  static std::int64_t Deserialize(PbReader& reader) { return reader.Reader().get_sfixed64(); }
};

template <>
class PbTraits<bool> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, bool value) {
    writer.Writer().add_bool(tag, value);
  }

  /// Deserializes a `bool` from `reader`.
  static bool Deserialize(PbReader& reader) { return reader.Reader().get_bool(); }
};

template <>
class PbTraits<float, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, float value) {
    writer.Writer().add_float(tag, value);
  }

  /// Deserializes a `float` from `reader`.
  static float Deserialize(PbReader& reader) { return reader.Reader().get_float(); }
};

template <>
class PbTraits<double, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, double value) {
    writer.Writer().add_double(tag, value);
  }

  /// Deserializes a `double` from `reader`.
  static double Deserialize(PbReader& reader) { return reader.Reader().get_double(); }
};

/// Traits for metadata of enum types.
template <class T>
class PbEnumTraits;

}  // namespace horus

#endif  // HORUS_PB_SERIALIZE_H_
