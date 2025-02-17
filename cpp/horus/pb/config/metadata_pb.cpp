#include "horus/pb/config/metadata_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

Vector2f::Vector2f(const Vector2f& other) noexcept(false)
    : x_{other.x_}
    , y_{other.y_}
    , set_fields_{other.set_fields_} {}

void Vector2f::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, x_);
  }
  if (set_fields_[1]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, y_);
  }
}

void Vector2f::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, x_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, y_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

Vector3f::Vector3f(const Vector3f& other) noexcept(false)
    : x_{other.x_}
    , y_{other.y_}
    , z_{other.z_}
    , set_fields_{other.set_fields_} {}

void Vector3f::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, x_);
  }
  if (set_fields_[1]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, y_);
  }
  if (set_fields_[2]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, z_);
  }
}

void Vector3f::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, x_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, y_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, z_);
        set_fields_[2] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

Quaterniond::Quaterniond(const Quaterniond& other) noexcept(false)
    : qw_{other.qw_}
    , qx_{other.qx_}
    , qy_{other.qy_}
    , qz_{other.qz_}
    , set_fields_{other.set_fields_} {}

void Quaterniond::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, qw_);
  }
  if (set_fields_[1]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, qx_);
  }
  if (set_fields_[2]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, qy_);
  }
  if (set_fields_[3]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 4, qz_);
  }
}

void Quaterniond::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, qw_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, qx_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, qy_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, qz_);
        set_fields_[3] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

Range::Range(const Range& other) noexcept(false)
    : start_{other.start_}
    , end_{other.end_}
    , set_fields_{other.set_fields_} {}

void Range::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, start_);
  }
  if (set_fields_[1]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, end_);
  }
}

void Range::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, start_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, end_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

UInt32List::UInt32List(const UInt32List& other) noexcept(false)
    : values_{other.values_}
    , set_fields_{other.set_fields_} {}

void UInt32List::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowRepeated<std::uint32_t>>(writer, /*tag=*/ 1, values_);
  }
}

void UInt32List::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowRepeated<std::uint32_t>>(reader, values_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

Timestamp::Timestamp(const Timestamp& other) noexcept(false)
    : seconds_{other.seconds_}
    , nanos_{other.nanos_}
    , set_fields_{other.set_fields_} {}

void Timestamp::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::int64_t>(writer, /*tag=*/ 1, seconds_);
  }
  if (set_fields_[1]) {
    SerializeField<std::int32_t>(writer, /*tag=*/ 2, nanos_);
  }
}

void Timestamp::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::int64_t>(reader, seconds_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::int32_t>(reader, nanos_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

Duration::Duration(const Duration& other) noexcept(false)
    : seconds_{other.seconds_}
    , nanos_{other.nanos_}
    , set_fields_{other.set_fields_} {}

void Duration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::int64_t>(writer, /*tag=*/ 1, seconds_);
  }
  if (set_fields_[1]) {
    SerializeField<std::int32_t>(writer, /*tag=*/ 2, nanos_);
  }
}

void Duration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::int64_t>(reader, seconds_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::int32_t>(reader, nanos_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

AffineTransform3f::AffineTransform3f(const AffineTransform3f& other) noexcept(false)
    : data_{other.data_}
    , set_fields_{other.set_fields_} {}

void AffineTransform3f::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowSpan<float>, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, data_);
  }
}

void AffineTransform3f::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowSpan<float>, PbDeserFlags::kFixed>(reader, data_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
