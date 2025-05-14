#include "horus/types/one_of.h"

#include <cstdint>
#include <stdexcept>

#include "horus/strings/str_cat.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace one_of_internal {

void OneOfCaseNotHandled(StringView file, std::uint32_t line, std::uint16_t tag) {
  throw std::logic_error{StrCat("OneOf case not handled in ", file, ":", line, ", tag: ", tag)};
}

}  // namespace one_of_internal
}  // namespace horus
