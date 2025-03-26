#include "horus/sdk/version.h"

#include <string>

#include "horus/strings/str_cat.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace sdk {

std::string Version::ToString() const {
  StringView const pre_separator{pre_.empty() ? "" : "-"};
  return StrCat(major_, ".", minor_, ".", patch_, pre_separator, pre_);
}

}  // namespace sdk
}  // namespace horus