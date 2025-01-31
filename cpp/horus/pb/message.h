/// @file
///
///

#ifndef HORUS_PB_MESSAGE_H_
#define HORUS_PB_MESSAGE_H_

#include <bitset>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

#include "horus/pb/serialize.h"
#include "horus/pb/types.h"
#include "horus/types/string_view.h"

namespace horus {

/// Base class for all Protobuf message types.
class PbMessage {
 public:
  /// Default-initializes the `PbMessage`.
  PbMessage() = default;

  /// Protobuf messages cannot be implicitly copied.
  explicit PbMessage(const PbMessage&) = default;  // NOLINT(*-explicit-*)
  /// Protobuf messages cannot be implicitly copied.
  PbMessage& operator=(const PbMessage&) = default;

  /// Move-constructs the message.
  PbMessage(PbMessage&&) noexcept = default;
  /// Move-assigns the message.
  PbMessage& operator=(PbMessage&&) noexcept = default;

  /// Returns the qualified name of the message without leading dot, e.g. `horus.pb.Foo.Bar`.
  virtual StringView MessageTypeName() const noexcept = 0;

  /// Serializes the message to `writer`.
  ///
  /// Note that, if no fields are set, nothing will be written to `writer`.
  virtual void SerializeTo(PbWriter& writer) const noexcept(false) = 0;

  /// Deserializes the message from `reader`.
  virtual void DeserializeFrom(PbReader& reader) noexcept(false) = 0;

  /// Returns whether the message is empty, i.e. none of its fields is set.
  virtual bool IsEmpty() const noexcept = 0;

  /// Destroys the message.
  virtual ~PbMessage() = default;

  /// Serializes the message to a `std::string`.
  std::string SerializeToString() const noexcept(false);
  /// Serializes the message to a `std::string`.
  void SerializeToString(std::string& string) const noexcept(false);

 protected:
  /// Serializes a field of type `T`.
  template <class T, PbDeserFlags kFlags = PbDeserFlags::kNone>
  static void SerializeField(
      PbWriter& writer, PbTag tag,
      std::conditional_t<(sizeof(T) <= sizeof(void*) * 2 && std::is_trivially_copyable<T>::value),
                         T, const T&>
          value) {
    PbTraits<T, kFlags>::Serialize(writer, tag, value);
  }

  /// Deserializes a field of type `T` in-place.
  template <class T, PbDeserFlags kFlags = PbDeserFlags::kNone,
            decltype(PbTraits<T, kFlags>::Deserialize(std::declval<PbReader&>(),
                                                      std::declval<T&>()))* = nullptr>
  static void DeserializeField(PbReader& reader, T& field) noexcept(false) {
    PbTraits<T, kFlags>::Deserialize(reader, field);
  }

  /// Deserializes a field of type `T` and assigns it to `field`.
  template <class T, PbDeserFlags kFlags = PbDeserFlags::kNone,
            decltype(PbTraits<T, kFlags>::Deserialize(std::declval<PbReader&>()))* = nullptr>
  static void DeserializeField(PbReader& reader, T& field) noexcept(false) {
    field = PbTraits<T, kFlags>::Deserialize(reader);
  }

  /// Clears the bit corresponding to the oneof with the given tag in `set_fields` and resets
  /// `oneof_tag`.
  template <std::size_t N>
  static void ClearOneof(std::bitset<N>& set_fields, PbTag& oneof_tag) noexcept {
    if (oneof_tag != 0) {
      set_fields.set(static_cast<std::size_t>(oneof_tag), false);
      oneof_tag = 0;
    }
  }
};

}  // namespace horus

#endif  // HORUS_PB_MESSAGE_H_
