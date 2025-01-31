#include "horus/pb/point/point_message_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

PointFrame_Header::PointFrame_Header(const PointFrame_Header& other) noexcept(false)
    : calibration_transform_{other.calibration_transform_}
    , lidar_id_{other.lidar_id_}
    , point_cloud_creation_timestamp_{other.point_cloud_creation_timestamp_}
    , set_fields_{other.set_fields_} {}

void PointFrame_Header::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<AffineTransform3f>(writer, /*tag=*/ 1, calibration_transform_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_id_);
  }
  if (set_fields_[2]) {
    SerializeField<Timestamp>(writer, /*tag=*/ 4, point_cloud_creation_timestamp_);
  }
}

void PointFrame_Header::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<AffineTransform3f>(reader, calibration_transform_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[1] = true;
        break;
      }
      case 4: {
        DeserializeField<Timestamp>(reader, point_cloud_creation_timestamp_);
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

AttributedPoints::AttributedPoints(const AttributedPoints& other) noexcept(false)
    : flattened_points_{other.flattened_points_}
    , attributes_{other.attributes_}
    , intensities_{other.intensities_}
    , set_fields_{other.set_fields_} {}

void AttributedPoints::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowSpan<float>, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, flattened_points_);
  }
  if (set_fields_[1]) {
    SerializeField<CowSpan<std::uint32_t>, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, attributes_);
  }
  if (set_fields_[2]) {
    SerializeField<CowSpan<std::uint32_t>, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, intensities_);
  }
}

void AttributedPoints::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowSpan<float>, PbDeserFlags::kFixed>(reader, flattened_points_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowSpan<std::uint32_t>, PbDeserFlags::kFixed>(reader, attributes_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowSpan<std::uint32_t>, PbDeserFlags::kFixed>(reader, intensities_);
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

PointFrame::PointFrame(const PointFrame& other) noexcept(false)
    : header_{other.header_}
    , points_{other.points_}
    , set_fields_{other.set_fields_} {}

void PointFrame::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<PointFrame_Header>(writer, /*tag=*/ 2, header_);
  }
  if (set_fields_[1]) {
    SerializeField<AttributedPoints>(writer, /*tag=*/ 9, points_);
  }
}

void PointFrame::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 2: {
        DeserializeField<PointFrame_Header>(reader, header_);
        set_fields_[0] = true;
        break;
      }
      case 9: {
        DeserializeField<AttributedPoints>(reader, points_);
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

ProcessedPointsEvent::ProcessedPointsEvent(const ProcessedPointsEvent& other) noexcept(false)
    : point_frame_{other.point_frame_}
    , set_fields_{other.set_fields_} {}

void ProcessedPointsEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<PointFrame>(writer, /*tag=*/ 1, point_frame_);
  }
}

void ProcessedPointsEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<PointFrame>(reader, point_frame_);
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

AggregatedPointEvents::AggregatedPointEvents(const AggregatedPointEvents& other) noexcept(false)
    : events_{other.events_}
    , set_fields_{other.set_fields_} {}

void AggregatedPointEvents::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowRepeated<ProcessedPointsEvent>>(writer, /*tag=*/ 1, events_);
  }
}

void AggregatedPointEvents::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowRepeated<ProcessedPointsEvent>>(reader, events_);
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
