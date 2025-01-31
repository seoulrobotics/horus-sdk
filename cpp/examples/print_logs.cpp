/// @file
///
/// A command line executable which connects to a Horus notification service and prints all
/// received logs to the standard output.
///
/// Note that the executable will continuously try to silently reconnect to the notification
/// service, so even if it is not running no error message will be printed.

#include <cstddef>
#include <cstdlib>

#include "examples/helpers.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/sdk.h"
#include "horus/sdk/logs.h"
#include "horus/strings/ansi.h"
#include "horus/strings/chrono.h"  // IWYU pragma: keep
#include "horus/strings/pad.h"
#include "horus/strings/stdio.h"
#include "horus/strings/str_sink.h"
#include "horus/types/span.h"

int main(int argc, const char* argv[]) {
  const horus::Span<const char*> args{argv, static_cast<std::size_t>(argc)};

  horus::Sdk::ServiceResolutionMap service_map;
  if (!horus::ParseArgs(service_map.notification, args)) {
    return 1;
  }
  horus::Sdk sdk{service_map};
  horus::SdkSubscription const subscription{
      sdk.SubscribeToLogs({/*on_log_message=*/[](const horus::pb::LogMessage& log_message) {
           const horus::sdk::Log log{log_message};
           horus::ColoredFormat<const char*> (*format)(const char* const&){
               &horus::MagentaColored<const char*>};
           char chr{'?'};
           switch (log.severity) {
             case horus::sdk::Log::Severity::kDebug: {
               format = &horus::BlackColored;
               chr = 'D';
               break;
             }
             case horus::sdk::Log::Severity::kInfo: {
               format = &horus::GreenColored;
               chr = 'I';
               break;
             }
             case horus::sdk::Log::Severity::kWarning: {
               format = &horus::YellowColored;
               chr = 'W';
               break;
             }
             case horus::sdk::Log::Severity::kError: {
               format = &horus::RedColored;
               chr = 'E';
               break;
             }
             case horus::sdk::Log::Severity::kFatal: {
               format = &horus::RedColored;
               chr = 'F';
               break;
             }
             case horus::sdk::Log::Severity::kUnknownWireValue:
             case horus::sdk::Log::Severity::kUnspecified:
             default:
               break;
           }
           constexpr auto kSep = " | ";
           const horus::ColoredFormat<const char*> sep{format(kSep)};

           horus::StrAppendToSink(horus::StdoutSink(), horus::Iso8601{log.time}, sep, "#",
                                  horus::PadLeftBy(3, log.id, '0'), sep, log.node_id, sep,
                                  horus::Char{chr}, sep, log.message, "\n");
         }})
          .Wait()};

  horus::WaitForTermination();
  return 0;
}
