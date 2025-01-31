/// @file
///
/// A command line executable which connects to Horus services and prints all events.

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <mutex>

#include "examples/helpers.h"
#include "horus/logs/format.h"  // IWYU pragma: keep
#include "horus/pb/cow.h"
#include "horus/pb/detection_service/detection_pb.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/point/point_message_pb.h"
#include "horus/pb/profiling_pb.h"
#include "horus/sdk.h"
#include "horus/strings/ansi.h"
#include "horus/strings/chrono.h"  // IWYU pragma: keep
#include "horus/strings/stdio.h"
#include "horus/strings/str_sink.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

void PrintObject(const pb::DetectedObject& object) {
  StrAppendToSink(StdoutSink(), "id=", object.status().id(),
                  ", label=", object.classification().class_label());
}

void PrintDetectionResults(const pb::DetectionEvent& event) {
  StrAppendToSink(StdoutSink(), event.objects().size(), " object(s) in ",
                  event.labeled_point_clouds().size(), " point cloud(s)",
                  event.objects().empty() ? "" : ":", "\n");
  for (const Cow<pb::DetectedObject>& object : event.objects()) {
    StrAppendToSink(StdoutSink(), "  ");
    PrintObject(object.Ref());
    StrAppendToSink(StdoutSink(), "\n");
  }
}

void PrintLogMessage(const pb::LogMessage& log_message) {
  StrAppendToSink(StdoutSink(), log_message.metadata().severity(), " log message from ",
                  log_message.metadata().node_id(), ": ", log_message.data(), "\n");
}

void PrintPointCloud(const pb::PointFrame& point_cloud) {
  StrAppendToSink(StdoutSink(),
                  "points=", point_cloud.points().flattened_points().Span().size() / 3, "\n");
}

void PrintProfilingInformation(const pb::ProfilingInfo& profiling_info) {
  switch (profiling_info.profiling_set_case()) {
    case pb::ProfilingInfo::ProfilingSetOneof::kNotSet:
      StrAppendToSink(StdoutSink(), "(empty)\n");
      break;
    case pb::ProfilingInfo::ProfilingSetOneof::kGeneralProfilingSet:
      StrAppendToSink(StdoutSink(), "general profiling set for ",
                      profiling_info.general_profiling_set().profiled_service(), "\n");
      break;
    case pb::ProfilingInfo::ProfilingSetOneof::kBundledFrameProfilingSet:
      StrAppendToSink(StdoutSink(), "bundled frame profiling set\n");
      break;
    default:
      StrAppendToSink(StdoutSink(), "(unknown)\n");
      break;
  }
}

void PrintHeader(StringView event_type, ColoredFormat<StringView> (*color)(const StringView&)) {
  StrAppendToSink(StdoutSink(), Iso8601{std::chrono::system_clock::now()}, " ", color(event_type),
                  " | ");
}

}  // namespace
}  // namespace horus

int main(int argc, const char* argv[]) {
  const horus::Span<const char*> args{argv, static_cast<std::size_t>(argc)};

  horus::Sdk::ServiceResolutionMap service_map;
  if (!horus::ParseArgs(service_map, args)) {
    return 1;
  }
  horus::Sdk sdk{service_map};

  constexpr std::size_t kSubscriptionCount{4};

  std::mutex print_mutex;
  std::array<horus::SdkFuture<horus::SdkSubscription>, kSubscriptionCount> subscription_futures{
      sdk.SubscribeToLogs({[&print_mutex](const horus::pb::LogMessage& log_message) {
        const std::unique_lock<std::mutex> lock{print_mutex};
        horus::PrintHeader("log", &horus::BlueColored);
        horus::PrintLogMessage(log_message);
      }}),
      sdk.SubscribeToObjects({[&print_mutex](const horus::pb::DetectionEvent& event) {
        const std::unique_lock<std::mutex> lock{print_mutex};
        horus::PrintHeader("detection", &horus::GreenColored);
        horus::PrintDetectionResults(event);
      }}),
      sdk.SubscribeToPointClouds({[&print_mutex](const horus::pb::PointFrame& point_cloud) {
        const std::unique_lock<std::mutex> lock{print_mutex};
        horus::PrintHeader("point cloud", &horus::MagentaColored);
        horus::PrintPointCloud(point_cloud);
      }}),
      sdk.SubscribeToProfiling({[&print_mutex](const horus::pb::ProfilingInfo& profiling_info) {
        const std::unique_lock<std::mutex> lock{print_mutex};
        horus::PrintHeader("profiling", &horus::CyanColored);
        horus::PrintProfilingInformation(profiling_info);
      }}),
  };
  const std::array<horus::SdkSubscription, kSubscriptionCount> subscriptions{
      subscription_futures[0].Wait(),
      subscription_futures[1].Wait(),
      subscription_futures[2].Wait(),
      subscription_futures[3].Wait(),
  };
  horus::WaitForTermination();
  return 0;
}
