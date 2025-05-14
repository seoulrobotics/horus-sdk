/// @file
///
/// Optimization-related macros.

#ifndef HORUS_OPTIMIZATION_H_
#define HORUS_OPTIMIZATION_H_

// Inspired by Abseil's `ABSL_PREDICT_TRUE` and `ABSL_PREDICT_FALSE`:
// https://github.com/abseil/abseil-cpp/blob/310e6f4f0f202da13720fdd6cb0095e10a98fe1c/absl/base/optimization.h#L167-L196
//
// We use `false || (X)` to avoid implicit conversions to `bool` since `__builtin_expect` takes an
// integer argument.

// NOLINTBEGIN(*-macro-usage)

/// Indicates to the compiler that the given expression is expected to evaluate to `true`.
///
/// @see https://llvm.org/docs/BranchWeightMetadata.html#builtin-expect
#define HORUS_PREDICT_TRUE(X) static_cast<bool>(__builtin_expect(false || (X), true))

/// Indicates to the compiler that the given expression is expected to evaluate to `false`.
///
/// @see https://llvm.org/docs/BranchWeightMetadata.html#builtin-expect
#define HORUS_PREDICT_FALSE(X) static_cast<bool>(__builtin_expect(false || (X), false))

/// Indicates to the compiler that this function is unlikely to be called.
#define HORUS_COLD __attribute__((cold))

/// Indicate (strongly) to the compiler that this function should be inlined.
#define HORUS_INLINE __attribute__((always_inline))

// NOLINTEND(*-macro-usage)

#endif  // HORUS_OPTIMIZATION_H_
