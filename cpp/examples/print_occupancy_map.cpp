/// @file
///
/// A command line executable which connects to a Horus notification service and prints all
/// received Occupancy Grid messages to the standard output.
///
/// Note that the executable will continuously try to silently reconnect to the notification
/// service, so even if it is not running no error message will be printed.

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "examples/helpers.h"
#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/preprocessing/messages_pb.h"
#include "horus/rpc/services.h"
#include "horus/sdk.h"
#include "horus/strings/chrono.h"  // IWYU pragma: keep
#include "horus/strings/stdio.h"
#include "horus/strings/stringify.h"
#include "horus/types/span.h"

int main(int argc, const char* argv[]) {
  const horus::Span<const char*> args{argv, static_cast<std::size_t>(argc)};

  horus::RpcServices::ServiceResolutionMap service_map;
  if (!horus::ParseArgs(service_map.notification, args)) {
    return 1;
  }
  horus::Sdk sdk{service_map};
  horus::SdkSubscription const subscription{
      sdk.SubscribeToOccupancyGrid(
             {/*on_occupancy_grid=*/[](horus::pb::OccupancyGridEvent const& event) {
               horus::StringifyTo(horus::StdoutSink(), "Occupancy Grid received\n");
               horus::StringifyTo(horus::StdoutSink(), "  resolution: ", event.resolution(), "\n");
               horus::StringifyTo(horus::StdoutSink(), "  rows: ", event.grid().rows(), "\n");
               horus::StringifyTo(horus::StdoutSink(), "  cols: ", event.grid().cols(), "\n");
               horus::StringifyTo(horus::StdoutSink(), "  cells: ", event.grid().cells().size(),
                                  "\n");
               horus::StringifyTo(horus::StdoutSink(), "  detection range x: ", event.x_min(),
                                  " - ", event.x_min(), "\n");
               horus::StringifyTo(horus::StdoutSink(), "  detection range y: ", event.y_min(),
                                  " - ", event.y_min(), "\n");
               horus::StringifyTo(horus::StdoutSink(), "  timestamp: ", event.timestamp().seconds(),
                                  "s ", event.timestamp().nanos(), "ns\n");

               std::vector<horus::pb::OccupancyClassification> classifications;

               std::size_t const rows{event.grid().rows()};
               std::size_t const cols{event.grid().cols()};
               classifications.reserve(rows * cols);

               constexpr std::uint32_t const kNumCountBits{29U};
               constexpr std::uint32_t const kValueMask{(1U << kNumCountBits) - 1U};
               for (const std::uint32_t cell : event.grid().cells()) {
                 std::uint32_t const value{cell >> kNumCountBits};
                 std::uint32_t const count{cell & kValueMask};
                 classifications.insert(classifications.end(), count,
                                        static_cast<horus::pb::OccupancyClassification>(value));
               }

               auto const num_occluded = std::count_if(
                   classifications.begin(), classifications.end(),
                   [](horus::pb::OccupancyClassification const& classification) {
                     return classification == horus::pb::OccupancyClassification::kOccluded;
                   });
               auto const num_static_occupied =
                   std::count_if(classifications.begin(), classifications.end(),
                                 [](horus::pb::OccupancyClassification const& classification) {
                                   return classification ==
                                          horus::pb::OccupancyClassification::kStationaryOccupied;
                                 });
               auto const num_free =
                   static_cast<std::uint32_t>(rows * cols) - num_occluded - num_static_occupied;
               horus::StringifyTo(horus::StdoutSink(), "  num occluded: ", num_occluded,
                                  " num static occupied: ", num_static_occupied,
                                  " num free: ", num_free, "\n");
             }})
          .Wait()};

  horus::WaitForTermination();
  return 0;
}
