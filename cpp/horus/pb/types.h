/// @file
///
/// Common Protobuf-related type aliases used in Horus.

#ifndef HORUS_PB_TYPES_H_
#define HORUS_PB_TYPES_H_

#include <cstdint>
#include <protozero/types.hpp>

namespace horus {

/// Type of a tag in the encoding.
using PbTag = protozero::pbf_tag_type;

/// Underlying type of all enums.
using PbEnum = std::int32_t;

}  // namespace horus

#endif  // HORUS_PB_TYPES_H_
