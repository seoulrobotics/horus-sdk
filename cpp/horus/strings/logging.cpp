#include "horus/strings/logging.h"

#include <atomic>
#include <iostream>
#include <mutex>

#include "horus/strings/str_sink_erased.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

/// A structure used to store a `ScopedLogger*` pointer statically.
struct LoggerPointer {
  /// A pointer to the active logger.
  mutable std::atomic<ScopedLogger*> pointer{nullptr};
};

/// The global active logger.
constexpr LoggerPointer kCurrentLogger;

/// State of the default logger.
struct DefaultLogger {
  /// Mutex which guards access to `std::cerr`.
  mutable std::mutex mutex;
};

/// Sink used by the default logger.
struct DefaultSink {
  /// Appends `string` to `stderr`.
  void Append(StringView string) const noexcept {
    static_cast<void>(this);
    std::cerr << string;
  }
};

/// Default log function.
void DefaultLog(const void* data, void (*log)(const void* data, const ErasedSink& sink)) {
  static constexpr DefaultLogger kLogger;

  const std::unique_lock<std::mutex> lock{kLogger.mutex};
  DefaultSink sink;
  log(data, sink);
  std::cerr << "\n";
}

}  // namespace

// static
ScopedLogger* ScopedLogger::ExchangeCurrentLogger(ScopedLogger* ptr) noexcept {
  return kCurrentLogger.pointer.exchange(ptr, std::memory_order_acq_rel);
}

// static
void ScopedLogger::LogWith(const void* data,
                           void (*log)(const void* data, const ErasedSink& sink)) {
  ScopedLogger* logger{kCurrentLogger.pointer.load(std::memory_order_acquire)};

  if (logger == nullptr) {
    DefaultLog(data, log);
  } else {
    logger->log_with_(logger->logger_, data, log);
  }
}

}  // namespace horus
