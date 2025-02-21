#include "examples/helpers.h"

#include <array>
#include <atomic>
#include <condition_variable>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <mutex>
#include <utility>

#include "horus/rpc/services.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

/// Parses a `<host>:<port>` or `<port>` `url` into `entry`. If unsuccessful, prints an error on
/// `stderr` and returns `false`.
bool ParseServiceEntry(RpcServices::ServiceResolutionMap::Entry& entry, StringView url) noexcept {
  url = StripSuffix(StripPrefix(url, "ws://"), "/");

  StringView::size_type const colon_index{url.find(':')};
  StringView host{"127.0.0.1"};
  StringView port_str{url};
  if (colon_index != StringView::npos) {
    if (colon_index != 0) {
      host = host.substr(0, colon_index);
    }
    port_str = host.substr(colon_index + 1);
  }
  char* port_end{const_cast<char*>(port_str.end())};  // NOLINT(*-const-cast): not modified, but
                                                      // `std::strtoul()` requires non-const end.
  std::uint64_t const port{std::strtoul(port_str.begin(), &port_end, 10)};
  if (port >= static_cast<std::uint64_t>(std::numeric_limits<std::uint16_t>::max())) {
    std::cerr << "invalid port specified: " << port << "\n";
    return false;
  }

  entry.host = host;
  entry.port = static_cast<std::uint16_t>(port);
  return true;
}

}  // namespace

bool ParseArgs(RpcServices::ServiceResolutionMap::Entry& entry,
               Span<const char* const> args) noexcept {
  if (args.size() <= 1) {
    return true;
  }
  if (args.size() != 2) {
    std::cerr << "usage: " << args[0] << " [[host:]port | port]\n";
    return false;
  }
  return ParseServiceEntry(entry, args[1]);
}

bool ParseArgs(RpcServices::ServiceResolutionMap& entries, Span<const char* const> args) noexcept {
  const std::array<std::pair<StringView, RpcServices::ServiceResolutionMap::Entry*>, 4> arg_entries{
      std::pair<StringView, RpcServices::ServiceResolutionMap::Entry*>("--detection=",
                                                                       &entries.detection),
      std::pair<StringView, RpcServices::ServiceResolutionMap::Entry*>("--notification=",
                                                                       &entries.notification),
      std::pair<StringView, RpcServices::ServiceResolutionMap::Entry*>("--point-aggregator=",
                                                                       &entries.point_aggregator),
      std::pair<StringView, RpcServices::ServiceResolutionMap::Entry*>("--project-manager=",
                                                                       &entries.project_manager),
  };
  for (const StringView arg : args.subspan(1)) {
    bool found{false};
    for (const auto& arg_entry : arg_entries) {
      if (StartsWith(arg, arg_entry.first)) {
        if (!ParseServiceEntry(*arg_entry.second, arg.substr(arg_entry.first.size()))) {
          return false;
        }
        found = true;
        break;
      }
    }
    if (!found) {
      std::cerr << "usage: " << args[0]
                << " [--detection=[host:]port]"
                   " [--notification=[host:]port]"
                   " [--point-aggregator=[host:]port]\n";
      return false;
    }
  }
  return true;
}

void WaitForTermination() noexcept {
  static std::atomic<std::mutex*> mutex_ptr{nullptr};
  static std::atomic<std::condition_variable*> termination_cv_ptr{nullptr};

  std::mutex mutex;
  std::condition_variable termination_cv;
  std::unique_lock<std::mutex> lock{mutex};
  mutex_ptr = &mutex;
  termination_cv_ptr = &termination_cv;

  const decltype(SIG_DFL) previous_handler{std::signal(
      SIGINT, +[](decltype(SIGINT) /* signum */) noexcept {
        if (mutex_ptr == nullptr || termination_cv_ptr == nullptr) {
          // This shouldn't happen, but we're in a signal handler so we won't do anything here.
          return;
        }
        const std::unique_lock<std::mutex> handler_lock{*mutex_ptr};
        termination_cv_ptr.load()->notify_one();
        mutex_ptr = nullptr;
        termination_cv_ptr = nullptr;
      })};

  termination_cv.wait(lock, []() noexcept -> bool { return mutex_ptr == nullptr; });

  static_cast<void>(std::signal(SIGINT, previous_handler));

  termination_cv_ptr = nullptr;  // To please clang analyzer.
}

}  // namespace horus
