#ifndef HORUS_PB_STATUS_SERVICE_MESSAGES_PB_H_
#define HORUS_PB_STATUS_SERVICE_MESSAGES_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/status_service/messages.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/attributes.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/message.h"
#include "horus/pb/serialize.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

// NOLINTBEGIN(readability-identifier-length)

namespace horus {
namespace sdk {
namespace pb {

// MARK: Message forward declarations

class Version;

// MARK: Message declarations

/// Semver representation "major.minor.patch-pre+build"
///
/// Source: horus/pb/status_service/messages.proto:6:1
class Version final : public PbMessage {
 public:

  /// Constructs a default-initialized `Version`.
  Version() noexcept = default;

  /// Move constructor.
  Version(Version&&) noexcept = default;
  /// Move assignment operator.
  Version& operator=(Version&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Version(const Version& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Version& operator=(const Version&) = delete;

  /// Default destructor.
  ~Version() noexcept final = default;

  /// Creates a `Version` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Version(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.Version`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Version"; }

  /// The full name of the message: `horus.pb.Version`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `major` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr std::uint32_t major() const& noexcept HORUS_LIFETIME_BOUND {
    return major_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  std::uint32_t& mutable_major() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[0] = true;
    return major_;
  }

  /// Returns whether `major` (no 1) is set.
  constexpr bool has_major() const noexcept { return set_fields_[0]; }

  /// Clears `major` (no 1).
  void clear_major() & noexcept {
    set_fields_[0] = false;
    major_ = {};
  }

  /// Sets `major` (no 1) and returns `*this`.
  Version& set_major(std::uint32_t major) & noexcept {
    set_fields_[0] = true;
    major_ = major;
    return *this;
  }
  /// Sets `major` (no 1) and returns `*this`.
  Version&& set_major(std::uint32_t major) && noexcept {
    return std::move(set_major(major));
  }

  // Field `minor` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr std::uint32_t minor() const& noexcept HORUS_LIFETIME_BOUND {
    return minor_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  std::uint32_t& mutable_minor() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[1] = true;
    return minor_;
  }

  /// Returns whether `minor` (no 2) is set.
  constexpr bool has_minor() const noexcept { return set_fields_[1]; }

  /// Clears `minor` (no 2).
  void clear_minor() & noexcept {
    set_fields_[1] = false;
    minor_ = {};
  }

  /// Sets `minor` (no 2) and returns `*this`.
  Version& set_minor(std::uint32_t minor) & noexcept {
    set_fields_[1] = true;
    minor_ = minor;
    return *this;
  }
  /// Sets `minor` (no 2) and returns `*this`.
  Version&& set_minor(std::uint32_t minor) && noexcept {
    return std::move(set_minor(minor));
  }

  // Field `patch` (no 3).
  // -----

  /// No documentation.
  ///
  /// Field no: 3.
  constexpr std::uint32_t patch() const& noexcept HORUS_LIFETIME_BOUND {
    return patch_;
  }

  /// No documentation.
  ///
  /// Field no: 3.
  std::uint32_t& mutable_patch() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[2] = true;
    return patch_;
  }

  /// Returns whether `patch` (no 3) is set.
  constexpr bool has_patch() const noexcept { return set_fields_[2]; }

  /// Clears `patch` (no 3).
  void clear_patch() & noexcept {
    set_fields_[2] = false;
    patch_ = {};
  }

  /// Sets `patch` (no 3) and returns `*this`.
  Version& set_patch(std::uint32_t patch) & noexcept {
    set_fields_[2] = true;
    patch_ = patch;
    return *this;
  }
  /// Sets `patch` (no 3) and returns `*this`.
  Version&& set_patch(std::uint32_t patch) && noexcept {
    return std::move(set_patch(patch));
  }

  // Field `pre` (no 4).
  // -----

  /// No documentation.
  ///
  /// Field no: 4.
  constexpr const CowBytes& pre() const& noexcept HORUS_LIFETIME_BOUND {
    return pre_;
  }

  /// If `pre` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 4.
  CowBytes pre() && noexcept {
    if (!set_fields_[3]) {
      return {};
    }
    return std::move(pre_);
  }

  /// No documentation.
  ///
  /// Field no: 4.
  CowBytes& mutable_pre() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[3] = true;
    return pre_;
  }

  /// Returns whether `pre` (no 4) is set.
  constexpr bool has_pre() const noexcept { return set_fields_[3]; }

  /// Clears `pre` (no 4).
  void clear_pre() & noexcept {
    set_fields_[3] = false;
    pre_ = {};
  }

  /// Sets `pre` (no 4) and returns `*this`.
  Version& set_pre(CowBytes&& pre) & noexcept {
    set_fields_[3] = true;
    pre_ = std::move(pre);
    return *this;
  }
  /// Sets `pre` (no 4) and returns `*this`.
  Version&& set_pre(CowBytes&& pre) && noexcept {
    return std::move(set_pre(std::move(pre)));
  }

 private:
  /// @see major()
  std::uint32_t major_{};
  /// @see minor()
  std::uint32_t minor_{};
  /// @see patch()
  std::uint32_t patch_{};
  /// @see pre()
  CowBytes pre_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<4> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_STATUS_SERVICE_MESSAGES_PB_H_
