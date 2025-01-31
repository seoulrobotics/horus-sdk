#include "horus/sdk/logs.h"

#include <chrono>

#include "horus/logs/format.h"  // IWYU pragma: keep
#include "horus/pb/logs/message_pb.h"
#include "horus/strings/str_cat.h"

namespace horus {
namespace sdk {

Log::Log(const pb::LogMessage& log_message) noexcept(false)
    : id{static_cast<Id>(log_message.data().data_case())},
      message{StrCat(log_message.data())},
      severity{log_message.metadata().severity()},
      time{std::chrono::system_clock::time_point{
          std::chrono::milliseconds{log_message.metadata().ms_since_epoch()}}},
      node_id{log_message.metadata().node_id().Str()} {}

}  // namespace sdk
}  // namespace horus
