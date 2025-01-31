#include "horus/future/poll.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>

#include "horus/future/info.h"

namespace horus {
namespace {

/// Returns a reference to the pointer which points to the current `PollContext::Trace`, if any.
PollContext::Trace*& CurrentTrace() noexcept {
  thread_local PollContext::Trace* current_trace{nullptr};  // NOLINT(*-non-const-global-variables)
  return current_trace;
}

}  // namespace

PollContext::Trace::Trace(PollContext& context, const char* type, const FutureInfo& info) noexcept
    : parent_{std::exchange(CurrentTrace(), this)}, context_{context}, type_{type}, info_{info} {
  if (context.trace_ != nullptr) {
    context.trace_(context.trace_receiver_, type, &info);
  }
}

PollContext::Trace::~Trace() noexcept {
  if (context_.trace_ != nullptr) {
    context_.trace_(context_.trace_receiver_, nullptr, nullptr);
  }
  CurrentTrace() = parent_;
}

/// Prints the stack trace of the current to stderr, if any.
extern void PrintFutureStackTrace() {
  const PollContext::Trace* trace{CurrentTrace()};
  std::cerr << "future backtrace:";
  if (trace == nullptr) {
    std::cerr << " <no active future>\n";
    return;
  }
  std::cerr << "\n";

  // Compute size of each column.
  std::int32_t max_type_len{1};
  std::int32_t max_name_len{1};
  {
    const PollContext::Trace* count_trace{trace};
    while (count_trace != nullptr) {
      max_type_len =
          std::max(max_type_len, static_cast<std::int32_t>(count_trace->FutureType().size()));
      max_name_len = std::max(
          max_name_len,
          static_cast<std::int32_t>(std::strlen(count_trace->FutureInformation().function_name)));
      count_trace = count_trace->Parent();
    }
  }

  // Print stack trace.
  std::uint32_t offset{1};
  while (trace != nullptr) {
    const FutureInfo& info{trace->FutureInformation()};

    std::cerr << "  #" << std::left << std::setw(2) << offset << " "  //
              << std::right << std::setw(max_type_len) << trace->FutureType().data()
              << ", called by " << std::right << std::setw(max_name_len) << info.function_name
              << "() at " << info.source_location.File() << ":" << info.source_location.Line()
              << "\n";
    trace = trace->Parent();
    ++offset;
  }
}

}  // namespace horus
