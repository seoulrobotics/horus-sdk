#include "horus/pb/preprocessing/messages_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

OccupancyGrid::OccupancyGrid(const OccupancyGrid& other) noexcept(false)
    : rows_{other.rows_}
    , cols_{other.cols_}
    , cells_{other.cells_}
    , set_fields_{other.set_fields_} {}

void OccupancyGrid::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, rows_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, cols_);
  }
  if (set_fields_[2]) {
    SerializeField<CowRepeated<std::uint32_t>>(writer, /*tag=*/ 3, cells_);
  }
}

void OccupancyGrid::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, rows_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, cols_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowRepeated<std::uint32_t>>(reader, cells_);
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

OccupancyGridEvent::OccupancyGridEvent(const OccupancyGridEvent& other) noexcept(false)
    : grid_{other.grid_}
    , x_min_{other.x_min_}
    , x_max_{other.x_max_}
    , y_min_{other.y_min_}
    , y_max_{other.y_max_}
    , resolution_{other.resolution_}
    , set_fields_{other.set_fields_} {}

void OccupancyGridEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<OccupancyGrid>(writer, /*tag=*/ 1, grid_);
  }
  if (set_fields_[1]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, x_min_);
  }
  if (set_fields_[2]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, x_max_);
  }
  if (set_fields_[3]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 4, y_min_);
  }
  if (set_fields_[4]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 5, y_max_);
  }
  if (set_fields_[5]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 6, resolution_);
  }
}

void OccupancyGridEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<OccupancyGrid>(reader, grid_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, x_min_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, x_max_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, y_min_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, y_max_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, resolution_);
        set_fields_[5] = true;
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
