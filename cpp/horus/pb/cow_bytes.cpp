#include "horus/pb/cow_bytes.h"

#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

#include "horus/pb/buffer.h"
#include "horus/types/string_view.h"

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
  switch (data_.Tag()) {
    case Data::kTagFor<Owned>: {
      const Owned& data{*data_.TryAs<Owned>()};
      return {data.data(), data.size()};
    }
    case Data::kTagFor<PbView>:
    default: {
      return data_.TryAs<PbView>()->Str();
    }
  }
}

CowBytes::Owned& CowBytes::String() noexcept(false) {
  switch (data_.Tag()) {
    case Data::kTagFor<Owned>: {
      return *data_.TryAs<Owned>();
    }
    case Data::kTagFor<PbView>:
    default: {
      Owned data{CopyToVector(data_.TryAs<PbView>()->Str())};
      return data_.Emplace<Owned>(std::move(data));
    }
  }
}

PbView CowBytes::View() const& noexcept(false) {
  switch (data_.Tag()) {
    case Data::kTagFor<Owned>: {
      return PbView{PbBuffer::Copy(Str())};
    }
    case Data::kTagFor<PbView>: {
      return data_.As<PbView>();
    }
    default: {
      assert(false);
      return PbView{PbBuffer{}};
    }
  }
}

PbView CowBytes::View() && noexcept {
  switch (data_.Tag()) {
    case Data::kTagFor<Owned>: {
      return PbView{PbBuffer{std::move(data_).As<Owned>()}};
    }
    case Data::kTagFor<PbView>: {
      return std::move(data_).As<PbView>();
    }
    default: {
      assert(false);
      return PbView{PbBuffer{}};
    }
  }
}

}  // namespace horus
