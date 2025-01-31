/// @file
///
/// The `UnsafeUnion` type.

#ifndef HORUS_INTERNAL_UNSAFE_UNION_H_
#define HORUS_INTERNAL_UNSAFE_UNION_H_

#include "horus/internal/attributes.h"

#if HORUS_SDK_USE_UNION
#include "horus/internal/unsafe_union_union.h"  // IWYU pragma: export
#else
#include "horus/internal/unsafe_union_aligned_bytes.h"  // IWYU pragma: export
#endif

#endif  // HORUS_INTERNAL_UNSAFE_UNION_H_
