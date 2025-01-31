#include "horus/types/string_view.h"

#include <cstddef>
#include <cstdint>
#include <functional>  // IWYU pragma: keep for std::hash<>
#include <ios>         // IWYU pragma: keep for std::streamsize
#include <ostream>

namespace horus {
namespace {

/// Returns the 64-bits FNV-1a hash of the given string.
///
/// @see http://www.isthe.com/chongo/tech/comp/fnv/
constexpr std::uint64_t Fnv1a(StringView string) noexcept {
  // http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
  constexpr std::uint64_t kFnv1aPrime{14695981039346656037ULL};
  constexpr std::uint64_t kFnv1aOffsetBasis{1099511628211ULL};

  std::uint64_t hash{kFnv1aPrime};
  for (const char chr : string) {
    hash ^= static_cast<std::uint8_t>(chr);
    hash *= kFnv1aOffsetBasis;
  }
  return hash;
}

/// Result of case #8 in http://www.isthe.com/chongo/src/fnv/test_fnv.c.
constexpr std::uint64_t kFooFnv1a{0xdcb27518fed9d577ULL};

static_assert(Fnv1a("foo") == kFooFnv1a, "");

}  // namespace

std::ostream& operator<<(std::ostream& out, StringView string) {
  return out.write(string.data(), static_cast<std::streamsize>(string.size()));
}

}  // namespace horus

std::size_t std::hash<horus::StringView>::operator()(horus::StringView string) const noexcept {
  return horus::Fnv1a(string);
}
