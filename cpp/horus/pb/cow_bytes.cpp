#include "horus/pb/cow_bytes.h"

#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

#include "horus/pb/buffer.h"
#include "horus/strings/string_view.h"
#include "horus/types/one_of.h"

namespace horus {
namespace {

/// Returns a `std::vector` with the same contents as `string`.
std::vector<std::uint8_t> CopyToVector(StringView string) {
  std::vector<std::uint8_t> vector;
  vector.reserve(string.size());
  static_cast<void>(vector.insert(vector.end(), string.begin(), string.end()));
  return vector;
}

}  // namespace

// static
CowBytes CowBytes::OwnedCopy(StringView string) noexcept(false) {
  return CowBytes{CopyToVector(string)};
}

// static
CowBytes CowBytes::SharedCopy(StringView string) noexcept(false) {
  return CowBytes{PbView{PbBuffer{CopyToVector(string)}}};
}

StringView CowBytes::Str() const noexcept {
  HORUS_ONEOF_SWITCH(data_) {
    HORUS_ONEOF_CASE(owned, Owned) { return {owned.data(), owned.size()}; }
    HORUS_ONEOF_CASE(view, PbView) { return view.Str(); }
  }
  HORUS_ONEOF_RETURN_NOT_HANDLED;
}

CowBytes::Owned& CowBytes::String() & noexcept(false) {
  HORUS_ONEOF_SWITCH(data_) {
    HORUS_ONEOF_CASE(owned, Owned) { return owned; }
    HORUS_ONEOF_CASE(view, PbView) {
      Owned data{CopyToVector(view.Str())};
      return data_.Emplace<Owned>(std::move(data));
    }
  }
  HORUS_ONEOF_RETURN_NOT_HANDLED;
}

PbView CowBytes::View() const& noexcept(false) {
  HORUS_ONEOF_SWITCH(data_) {
    HORUS_ONEOF_CASE_DISCARD(Owned) { return PbView{PbBuffer::Copy(Str())}; }
    HORUS_ONEOF_CASE(view, PbView) { return view; }
  }
  HORUS_ONEOF_RETURN_NOT_HANDLED;
}

PbView CowBytes::View() && noexcept {
  HORUS_ONEOF_SWITCH(data_) {
    HORUS_ONEOF_CASE(owned, Owned) { return PbView{PbBuffer{std::move(owned)}}; }
    HORUS_ONEOF_CASE(view, PbView) { return std::move(view); }
  }
  HORUS_ONEOF_RETURN_NOT_HANDLED;
}

}  // namespace horus
