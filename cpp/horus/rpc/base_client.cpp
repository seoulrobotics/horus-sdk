#include "horus/rpc/base_client.h"

#include <cstddef>
#include <cstdint>
#include <utility>

// IWYU wants to include <type_traits> for remove_reference.
// IWYU pragma: no_include <type_traits>

#include "horus/pb/cow_bytes.h"
#include "horus/pb/message.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace horus_internal {

StringView RpcBaseClient::ServiceName() const noexcept {
  const StringView service_full_name{ServiceFullName()};
  std::size_t const separator_pos{service_full_name.rfind('.')};
  return separator_pos == StringView::npos ? service_full_name
                                           : service_full_name.substr(separator_pos + 1);
}

sdk::pb::RpcMessage RpcBaseClient::MakeRequestMessage(std::uint16_t method_id,
                                                      const PbMessage& request) const {
  CowBytes request_bytes{request.SerializeToBuffer()};
  return sdk::pb::RpcMessage{}
      .set_version(sdk::pb::RpcMessage::Version::kOne)
      .set_service_id(ServiceId())
      .set_method_id(method_id)
      .set_message_bytes(std::move(request_bytes));
}

}  // namespace horus_internal
}  // namespace horus
