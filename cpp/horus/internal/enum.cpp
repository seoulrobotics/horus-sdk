#include "horus/internal/enum.h"

#include <cstdint>
#include <stdexcept>

#include "horus/strings/str_cat.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace horus_internal {

UnhandledEnumValueError::UnhandledEnumValueError(StringView type_name, std::int32_t value)
    : std::logic_error{StrCat("unhandled enum value of type ", type_name, ": ", value)} {}

}  // namespace horus_internal
}  // namespace horus
