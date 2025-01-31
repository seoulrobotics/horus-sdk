#include "horus/pb/cow_bytes.h"

#include <cassert>
#include <string>
#include <utility>

#include "horus/pb/buffer.h"
#include "horus/types/string_view.h"

namespace horus {

StringView CowBytes::Str() const noexcept {
  switch (data_.Tag()) {
    case Data::kTagFor<std::string>: {
      return *data_.TryAs<std::string>();
    }
    case Data::kTagFor<PbView>:
    default: {
      return data_.TryAs<PbView>()->Str();
    }
  }
}

std::string& CowBytes::String() noexcept(false) {
  switch (data_.Tag()) {
    case Data::kTagFor<std::string>: {
      return *data_.TryAs<std::string>();
    }
    case Data::kTagFor<PbView>:
    default: {
      std::string string{data_.TryAs<PbView>()->Str()};
      return data_.Emplace<std::string>(std::move(string));
    }
  }
}

PbView CowBytes::View() const& noexcept(false) {
  switch (data_.Tag()) {
    case Data::kTagFor<std::string>: {
      return PbView{PbBuffer{std::string{data_.As<std::string>()}}};
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
    case Data::kTagFor<std::string>: {
      return PbView{PbBuffer{std::move(data_).As<std::string>()}};
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
