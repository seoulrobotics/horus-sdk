#include "horus/pb/detection_service/detection_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

BoundingBox::BoundingBox(const BoundingBox& other) noexcept(false)
    : base_{other.base_}
    , size_{other.size_}
    , yaw_{other.yaw_}
    , set_fields_{other.set_fields_} {}

void BoundingBox::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Vector3f>(writer, /*tag=*/ 1, base_);
  }
  if (set_fields_[1]) {
    SerializeField<Vector3f>(writer, /*tag=*/ 2, size_);
  }
  if (set_fields_[2]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, yaw_);
  }
}

void BoundingBox::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Vector3f>(reader, base_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<Vector3f>(reader, size_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, yaw_);
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

LabeledPointCloud::LabeledPointCloud(const LabeledPointCloud& other) noexcept(false)
    : point_cloud_{other.point_cloud_}
    , point_index_to_object_id_{other.point_index_to_object_id_}
    , set_fields_{other.set_fields_} {}

void LabeledPointCloud::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<PointFrame>(writer, /*tag=*/ 1, point_cloud_);
  }
  if (set_fields_[1]) {
    SerializeField<UInt32List>(writer, /*tag=*/ 2, point_index_to_object_id_);
  }
}

void LabeledPointCloud::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<PointFrame>(reader, point_cloud_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<UInt32List>(reader, point_index_to_object_id_);
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

DetectedObject_Classification::DetectedObject_Classification(const DetectedObject_Classification& other) noexcept(false)
    : class_label_{other.class_label_}
    , set_fields_{other.set_fields_} {}

void DetectedObject_Classification::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ObjectLabel>(writer, /*tag=*/ 1, class_label_);
  }
}

void DetectedObject_Classification::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ObjectLabel>(reader, class_label_);
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

DetectedObject_Kinematics::DetectedObject_Kinematics(const DetectedObject_Kinematics& other) noexcept(false)
    : linear_velocity_{other.linear_velocity_}
    , yaw_rate_{other.yaw_rate_}
    , set_fields_{other.set_fields_} {}

void DetectedObject_Kinematics::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Vector2f>(writer, /*tag=*/ 1, linear_velocity_);
  }
  if (set_fields_[1]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, yaw_rate_);
  }
}

void DetectedObject_Kinematics::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Vector2f>(reader, linear_velocity_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, yaw_rate_);
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

DetectedObject_Shape::DetectedObject_Shape(const DetectedObject_Shape& other) noexcept(false)
    : bounding_box_{other.bounding_box_}
    , set_fields_{other.set_fields_} {}

void DetectedObject_Shape::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<BoundingBox>(writer, /*tag=*/ 1, bounding_box_);
  }
}

void DetectedObject_Shape::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<BoundingBox>(reader, bounding_box_);
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

DetectedObject_Status::DetectedObject_Status(const DetectedObject_Status& other) noexcept(false)
    : id_{other.id_}
    , tracking_status_{other.tracking_status_}
    , set_fields_{other.set_fields_} {}

void DetectedObject_Status::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, id_);
  }
  if (set_fields_[1]) {
    SerializeField<TrackingStatus>(writer, /*tag=*/ 2, tracking_status_);
  }
}

void DetectedObject_Status::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<TrackingStatus>(reader, tracking_status_);
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

DetectedObject::DetectedObject(const DetectedObject& other) noexcept(false)
    : classification_{other.classification_}
    , kinematics_{other.kinematics_}
    , shape_{other.shape_}
    , status_{other.status_}
    , set_fields_{other.set_fields_} {}

void DetectedObject::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<DetectedObject_Classification>(writer, /*tag=*/ 1, classification_);
  }
  if (set_fields_[1]) {
    SerializeField<DetectedObject_Kinematics>(writer, /*tag=*/ 2, kinematics_);
  }
  if (set_fields_[2]) {
    SerializeField<DetectedObject_Shape>(writer, /*tag=*/ 3, shape_);
  }
  if (set_fields_[3]) {
    SerializeField<DetectedObject_Status>(writer, /*tag=*/ 4, status_);
  }
}

void DetectedObject::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<DetectedObject_Classification>(reader, classification_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<DetectedObject_Kinematics>(reader, kinematics_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<DetectedObject_Shape>(reader, shape_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<DetectedObject_Status>(reader, status_);
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

DetectionEvent_FrameInfo::DetectionEvent_FrameInfo(const DetectionEvent_FrameInfo& other) noexcept(false)
    : frame_timestamp_{other.frame_timestamp_}
    , set_fields_{other.set_fields_} {}

void DetectionEvent_FrameInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Timestamp>(writer, /*tag=*/ 1, frame_timestamp_);
  }
}

void DetectionEvent_FrameInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Timestamp>(reader, frame_timestamp_);
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

DetectionEvent::DetectionEvent(const DetectionEvent& other) noexcept(false)
    : objects_{other.objects_}
    , labeled_point_clouds_{other.labeled_point_clouds_}
    , frame_info_{other.frame_info_}
    , set_fields_{other.set_fields_} {}

void DetectionEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowRepeated<DetectedObject>>(writer, /*tag=*/ 1, objects_);
  }
  if (set_fields_[1]) {
    SerializeField<CowRepeated<LabeledPointCloud>>(writer, /*tag=*/ 2, labeled_point_clouds_);
  }
  if (set_fields_[2]) {
    SerializeField<DetectionEvent_FrameInfo>(writer, /*tag=*/ 3, frame_info_);
  }
}

void DetectionEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowRepeated<DetectedObject>>(reader, objects_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowRepeated<LabeledPointCloud>>(reader, labeled_point_clouds_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<DetectionEvent_FrameInfo>(reader, frame_info_);
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus
