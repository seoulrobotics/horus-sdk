#include "horus/pointer/erased.h"

#include <stdexcept>
#include <string>
#include <typeinfo>

#include "horus/strings/string_view.h"
#include "horus/utility/discard.h"

namespace horus {

// static
void ErasedPointer::ThrowInvalidCastError(std::type_info const& from, std::type_info const& to) {
  std::string error_message;
  error_message.reserve(sizeof("invalid cast from ") + StringView{from.name()}.size() +
                        sizeof(" to ") + StringView{to.name()}.size());
  Discard(error_message.append("invalid cast from ")
              .append(from.name())
              .append(" to ")
              .append(to.name()));
  throw std::logic_error{error_message};
}

// static
void ErasedPointer::ThrowInvalidConstCastError(std::type_info const& type) {
  std::string error_message;
  error_message.reserve(sizeof("invalid const cast of ") + StringView{type.name()}.size());
  Discard(error_message.append("invalid const cast of ").append(type.name()));
  throw std::logic_error{error_message};
}

}  // namespace horus
