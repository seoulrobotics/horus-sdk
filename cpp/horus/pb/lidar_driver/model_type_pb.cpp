#include "horus/pb/lidar_driver/model_type_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LidarModelType::LidarModelType(const LidarModelType& other) noexcept(false)
    : misc_{other.misc_}
    , hesai_{other.hesai_}
    , ros2_topic_{other.ros2_topic_}
    , model_type_{other.model_type_}
    , set_fields_{other.set_fields_} {}

void LidarModelType::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<MiscLidarConfig_Model>(writer, /*tag=*/ 1, misc_);
  }
  if (set_fields_[1]) {
    SerializeField<HesaiLidarConfig_Model>(writer, /*tag=*/ 3, hesai_);
  }
  if (set_fields_[2]) {
    SerializeField<Ros2TopicLidarConfig_Model>(writer, /*tag=*/ 6, ros2_topic_);
  }
}

void LidarModelType::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_model_type();
        model_type_ = ModelTypeOneof::kMisc;
        DeserializeField<MiscLidarConfig_Model>(reader, misc_);
        set_fields_[0] = true;
        break;
      }
      case 3: {
        clear_model_type();
        model_type_ = ModelTypeOneof::kHesai;
        DeserializeField<HesaiLidarConfig_Model>(reader, hesai_);
        set_fields_[1] = true;
        break;
      }
      case 6: {
        clear_model_type();
        model_type_ = ModelTypeOneof::kRos2Topic;
        DeserializeField<Ros2TopicLidarConfig_Model>(reader, ros2_topic_);
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
