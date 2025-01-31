/// @file
///
/// Bridge between `<stdio>` and Horus formatting helpers.

#ifndef HORUS_STRINGS_STDIO_H_
#define HORUS_STRINGS_STDIO_H_

#include <iostream>
#include <ostream>

#include "horus/types/string_view.h"

namespace horus {

/// A formatting string which outputs to an `std::ostream`.
class OstreamSink final {
 public:
  /// Constructs an `OstreamSink` which outputs text to `out`.
  constexpr explicit OstreamSink(std::ostream& out) noexcept : out_{&out} {}

  /// Appends `string` to the output stream. The output string will not be flushed automatically.
  void Append(StringView string) const noexcept(false) { *out_ << string; }

 private:
  /// A (non-null) pointer to an output stream.
  std::ostream* out_;
};

/// Returns a formatting sink which outputs values to `stdout`.
inline const OstreamSink& StdoutSink() noexcept {
  static const OstreamSink sink{std::cout};
  return sink;
}

/// Returns a formatting sink which outputs values to `stderr`.
inline const OstreamSink& StderrSink() noexcept {
  static const OstreamSink sink{std::cerr};
  return sink;
}

}  // namespace horus

#endif  // HORUS_STRINGS_STDIO_H_
