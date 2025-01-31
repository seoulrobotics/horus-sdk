/// @file
///
/// Helpers used in Horus SDK examples.

#ifndef HORUS_SDK_CPP_EXAMPLES_HELPERS_H_
#define HORUS_SDK_CPP_EXAMPLES_HELPERS_H_

#include "horus/sdk.h"
#include "horus/types/span.h"

namespace horus {

/// Parses command line arguments. If a `<host>:<port>` or `<port>` string is given, fills `entry`.
///
/// Invalid arguments will lead to a string being printed to `stderr` and `false` returned.
bool ParseArgs(Sdk::ServiceResolutionMap::Entry& entry, Span<const char* const> args) noexcept;

/// Same as `ParseArgs()` above, but accepts multiple entries.
bool ParseArgs(Sdk::ServiceResolutionMap& entries, Span<const char* const> args) noexcept;

/// Blocks the calling thread until a termination is requested by the user (SIGINT, ctrl-c).
void WaitForTermination() noexcept;

}  // namespace horus

#endif  // HORUS_SDK_CPP_EXAMPLES_HELPERS_H_
