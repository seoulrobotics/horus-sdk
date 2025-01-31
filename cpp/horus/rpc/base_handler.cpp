#include "horus/rpc/base_handler.h"

#include <cstddef>

#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

StringView RpcBaseHandler::ServiceName() const noexcept {
  const StringView service_full_name{ServiceFullName()};
  std::size_t const separator_pos{service_full_name.rfind('.')};
  return separator_pos == StringView::npos ? service_full_name
                                           : service_full_name.substr(separator_pos + 1);
}

}  // namespace horus_internal
}  // namespace horus
