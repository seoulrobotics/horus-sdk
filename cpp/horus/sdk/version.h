/// @file
///
/// Types for version

#ifndef HORUS_SDK_VERSION_H_
#define HORUS_SDK_VERSION_H_

#include <cstdint>
#include <string>

#include "horus/pb/status_service/service_pb.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace sdk {

/// Represents a software version following semver format.
class Version final {
 public:
  /// Creates from a `pb::Version`.
  explicit Version(const pb::GetVersionResponse& version_pb)
      : major_{version_pb.version().major()},
        patch_{version_pb.version().patch()},
        pre_{version_pb.version().pre().Str()} {}

  /// Returns the major part of the version.
  constexpr std::uint32_t Major() const noexcept { return major_; }

  /// Returns the patch part of the version.
  constexpr std::uint32_t Patch() const noexcept { return patch_; }

  /// Returns the pre-crelease, such as "beta". May be empty.
  StringView Pre() const noexcept { return pre_; }

  /// Stringifies to version to format "r{major}.{patch}-{pre}+{build}".
  std::string ToString() const;

 private:
  std::uint32_t major_;
  std::uint32_t patch_;
  std::string pre_;
};

/// Request to call `StatusService.GetVersion()`.
struct GetVersionRequest {};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_VERSION_H_