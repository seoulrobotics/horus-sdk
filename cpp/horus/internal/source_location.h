#ifndef HORUS_INTERNAL_SOURCE_LOCATION_H_
#define HORUS_INTERNAL_SOURCE_LOCATION_H_

/// @file
///
/// The `SourceLocation` type.

#include <cstdint>

#include "horus/strings/str_sink.h"
#include "horus/types/string_view.h"

namespace horus {

/// The location of a line in source code.
class SourceLocation final {
 public:
  /// Constructs a `SourceLocation`.
  constexpr SourceLocation(const char* file, std::uint32_t line) noexcept
      : file_{file}, line_{line} {}

  /// The path to the file. Empty if source locations are disabled.
  constexpr StringView File() const noexcept { return file_; }
  /// The line number (starting at 1). 0 if source locations are disabled.
  constexpr std::uint32_t Line() const noexcept { return line_; }

 private:
  /// @see File()
  const char* file_;
  /// @see Line()
  std::uint32_t line_;
};

/// Returns the `SourceLocation` of the caller.
constexpr SourceLocation CurrentSourceLocation(const char* file = __builtin_FILE(),
                                               std::uint32_t line = __builtin_LINE()) noexcept {
  return SourceLocation{file, line};
}

/// Appends `source_location` to `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink,
                              SourceLocation source_location) noexcept(IsNoexceptSink<Sink>()) {
  StrAppendToSink(sink, source_location.File(), ":", source_location.Line());
}

/// Appends `source_location` to `out`.
template <class Output>
Output& operator<<(Output& out, SourceLocation source_location) {
  return out << source_location.File() << ":" << source_location.Line();
}

}  // namespace horus

#endif  // HORUS_INTERNAL_SOURCE_LOCATION_H_
