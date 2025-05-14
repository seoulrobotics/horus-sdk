/// @file
///
/// Bridge between `<stdio>` and Horus formatting helpers.

#ifndef HORUS_STRINGS_STDIO_H_
#define HORUS_STRINGS_STDIO_H_

#include <iostream>
#include <ostream>

#include "horus/strings/string_view.h"

namespace horus {

/// A formatting string which outputs to an `std::ostream`.
class OstreamSink final {
 public:
  /// Constructs an `OstreamSink` which outputs text to `out`.
  constexpr explicit OstreamSink(std::ostream& out HORUS_LIFETIME_BOUND) noexcept : out_{&out} {}

  /// Appends `string` to the output stream. The output string will not be flushed automatically.
  void Append(StringView string) const { *out_ << string; }

 private:
  /// A (non-null) pointer to an output stream.
  std::ostream* out_;
};

/// Returns a formatting sink which outputs values to `stdout`.
inline OstreamSink const& StdoutSink() noexcept {
  static const OstreamSink kSink{std::cout};
  return kSink;
}

/// Returns a formatting sink which outputs values to `stderr`.
inline OstreamSink const& StderrSink() noexcept {
  static const OstreamSink kSink{std::cerr};
  return kSink;
}

}  // namespace horus

#endif  // HORUS_STRINGS_STDIO_H_
