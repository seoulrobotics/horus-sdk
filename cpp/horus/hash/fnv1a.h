/// @file
///
/// The `Fnv1a()` hash function.

#ifndef HORUS_HASH_FNV1A_H_
#define HORUS_HASH_FNV1A_H_

#include <cstdint>

#include "horus/attributes.h"
#include "horus/strings/string_view.h"

namespace horus {

/// Returns the 64-bits FNV-1a hash of the given string.
///
/// @see http://www.isthe.com/chongo/tech/comp/fnv/
constexpr HORUS_NO_SANITIZE("unsigned-integer-overflow") std::uint64_t
    Fnv1a(StringView string) noexcept {
  // http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
  constexpr std::uint64_t kFnv1aPrime{14695981039346656037ULL};
  constexpr std::uint64_t kFnv1aOffsetBasis{1099511628211ULL};

  std::uint64_t hash{kFnv1aPrime};
  for (char const chr : string) {
    hash ^= static_cast<unsigned char>(chr);
    hash *= kFnv1aOffsetBasis;
  }
  return hash;
}

#if !NDEBUG
/// Result of case #8 in http://www.isthe.com/chongo/src/fnv/test_fnv.c.
static constexpr std::uint64_t kFnv1TestCase{0xdcb27518fed9d577ULL};
static_assert(Fnv1a("foo") == kFnv1TestCase, "");
#endif

}  // namespace horus

#endif  // HORUS_HASH_FNV1A_H_
