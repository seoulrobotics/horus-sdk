/// @file
///
/// The `FutureInfo` class.

#ifndef HORUS_FUTURE_INFO_H_
#define HORUS_FUTURE_INFO_H_

#include <cstdint>

#include "horus/source_location.h"
#include "horus/strings/stringify.h"

namespace horus {

/// Information about a `Future`.
struct FutureInfo {
  /// The source code location where the future was created.
  SourceLocation source_location;
  /// The name of the function where the future was created.
  const char* function_name;
};

/// Returns the `FutureInfo` of the caller.
constexpr FutureInfo CurrentFutureInfo(const char* file = __builtin_FILE(),
                                       std::uint32_t line = __builtin_LINE(),
                                       const char* function_name = __builtin_FUNCTION()) noexcept {
  return FutureInfo{SourceLocation{file, line}, function_name};
}

/// Returns a `FutureInfo` corresponding to an unknown location (e.g. because the caller accepts an
/// arbitrary amount of parameters and cannot have a `CurrentFutureInfo()` default argument).
constexpr FutureInfo UnknownFutureInfo(const char* function_name = __builtin_FUNCTION()) noexcept {
  return FutureInfo{SourceLocation{"<unknown>", 0}, function_name};
}

/// Appends `source_location` to `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink,
                              const FutureInfo& future_info) noexcept(IsNoexceptSink<Sink>()) {
  StringifyTo(sink, future_info.function_name, " ", future_info.source_location);
}

}  // namespace horus

#endif  // HORUS_FUTURE_INFO_H_
