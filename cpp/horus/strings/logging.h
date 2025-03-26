/// @file
///
/// Helpers for logging messages.

#ifndef HORUS_STRINGS_LOGGING_H_
#define HORUS_STRINGS_LOGGING_H_

#include <exception>
#include <utility>

#include "horus/attributes.h"
#include "horus/pointer/unsafe_cast.h"
#include "horus/strings/erased_sink.h"
#include "horus/strings/stringify.h"

namespace horus {

/// A class which can be used to specify the logger to use in `Log()` for a scope. Note that this
/// applies to all threads.
class ScopedLogger final {
 public:
  /// Constructs a logger, globally enabling it until it is destroyed. This function is thread-safe.
  ///
  /// `logger` must outlive the `ScopedLogger` and be noexcept-invocable with a `const F&` argument,
  /// with `F` a noexcept closure which accepts a `const ErasedSink&`. That is, `logger([](const
  /// ErasedSink&) noexcept {})` must be valid and noexcept.
  template <class F>
  explicit ScopedLogger(F& logger HORUS_LIFETIME_BOUND) noexcept;

  /// Cannot copy/move.
  ScopedLogger(const ScopedLogger&) = delete;
  /// Cannot copy/move.
  ScopedLogger(ScopedLogger&&) = delete;
  /// Cannot copy/move.
  ScopedLogger& operator=(const ScopedLogger&) = delete;
  /// Cannot copy/move.
  ScopedLogger& operator=(ScopedLogger&&) = delete;

  /// Destroys the logger, re-activating the previous one.
  ~ScopedLogger() noexcept { static_cast<void>(ExchangeCurrentLogger(parent_)); }

  /// Invokes `invocable(sink)` with `sink` a reference to the currently active logger.
  template <class F>
  static void LogWith(const F& invocable) noexcept {
    LogWith(
        &invocable, +[](const void* data, const ErasedSink& sink) noexcept {
          try {
            (*UnsafePointerCast<const F>(data))(sink);
          } catch (const std::exception& e) {
            sink.Append("log function threw an exception: ");
            sink.Append(e.what());
          }
        });
  }

 private:
  /// Exchanges `ptr` with the currently active logger, returning the previous one.
  static ScopedLogger* ExchangeCurrentLogger(ScopedLogger* ptr) noexcept;

  /// Calls `log(data, sink)` with `sink` the currently active global sink.
  static void LogWith(const void* data, void (*log)(const void* data, const ErasedSink& sink));

  /// The pointer to the logger to be given to `log_with_`.
  void* const logger_;
  /// The pointer to the noexcept function which implements `LogWith()`.
  void (*const log_with_)(void* logger, const void* data,
                          void (*log)(const void* data, const ErasedSink& sink));

  /// The logger which was in `kCurrentLogger` when this logger was created. This must be the last
  /// member of the logger since other threads may start accessing previous fields once this is set.
  ScopedLogger* const parent_{ExchangeCurrentLogger(this)};
};

template <class F>
ScopedLogger::ScopedLogger(F& logger) noexcept
    : logger_{&logger},
      log_with_{+[](void* logger_ptr, const void* data,
                    void (*log)(const void* data, const ErasedSink& sink)) noexcept {
        (*UnsafePointerCast<F>(logger_ptr))(
            [data, log](const ErasedSink& sink) noexcept { log(data, sink); });
      }} {
  class NoExceptLog final {
   public:
    constexpr void operator()(const ErasedSink& /* sink */) noexcept {}
  };
  static_assert(noexcept(logger(std::declval<NoExceptLog&>())), "logger(...) must be noexcept");
}

/// Logs the concatenation of the given arguments followed by a line return to the current logger
/// (as specified by `ScopedLogger`), defaulting to `stderr`.
template <class... Args>
void Log(const Args&... args) noexcept {
  ScopedLogger::LogWith([&args...](auto& sink) { StringifyTo(sink, args...); });
}

}  // namespace horus

#endif  // HORUS_STRINGS_LOGGING_H_
