/// @file
///
/// C++ attributes used throughout Horus.

#ifndef HORUS_ATTRIBUTES_H_
#define HORUS_ATTRIBUTES_H_

// NOLINTBEGIN(*-macro-usage): these cannot be expressed as functions

/// Indicates that the return value of a function or constructor is bound to an input lifetime.
///
/// Examples:
///
///  `Foo()` returns a view into its input string.
///
///   std::string_view Foo(std::string const& input HORUS_LIFETIME_BOUND);
///
///  `Bar()` returns a reference to a member variable, which therefore cannot outlive `this`:
///
///   Foo const& Bar() const & HORUS_LIFETIME_BOUND;
///
/// @see https://clang.llvm.org/docs/AttributeReference.html#id11
#if __has_cpp_attribute(clang::lifetimebound)
#define HORUS_LIFETIME_BOUND [[clang::lifetimebound]]
#else
#define HORUS_LIFETIME_BOUND
#endif

/// Indicates that the marked parameter is "captured" by the specified variable, and must therefore
/// outlive it.
///
/// @see https://clang.llvm.org/docs/AttributeReference.html#lifetime-capture-by
#if __has_cpp_attribute(lifetime_capture_by)
#define HORUS_LIFETIME_CAPTURE_BY(X) [[clang::lifetime_capture_by(X)]]
#else
#define HORUS_LIFETIME_CAPTURE_BY(X)
#endif

/// Indicates that a type is a "view" over some other type, like `Span` or `StringView`.
///
/// Clang may catch lifetime issues similar to `HORUS_LIFETIME_BOUND` when using such types.
///
/// @see https://clang.llvm.org/docs/AttributeReference.html#pointer
#if __has_cpp_attribute(gsl::Pointer)
#define HORUS_VIEW_TYPE [[gsl::Pointer]]
#else
#define HORUS_VIEW_TYPE
#endif

/// Indicates that this value should be used.
///
/// This expands to `[[nodiscard]]` when available, or to some equivalent if possible.
///
/// FUSA already enforces that we use all values; this attribute simply allows us to catch these
/// errors directly with clang, rather than going through CodeQL.
///
/// Note that this is named `HORUS_NO_DISCARD` to match the standard attribute `[[nodiscard]]`.
/// Values marked `HORUS_NO_DISCARD` may still be given to `Discard()` (although this may lead to
/// unexpected behavior as those values are expected to be used).
#if __cplusplus >= 201703L
#define HORUS_NO_DISCARD [[nodiscard]]
#elif defined(__clang__) && __has_cpp_attribute(warn_unused_result)
#define HORUS_NO_DISCARD __attribute__((warn_unused_result))
#else
#define HORUS_NO_DISCARD
#endif

/// Indicates that the marked function should not be sanitized with a reason.
#if defined(__clang__) && __has_attribute(no_sanitize)
#define HORUS_NO_SANITIZE(REASON) __attribute__((no_sanitize(REASON)))
#else
#define HORUS_NO_SANITIZE(REASON)
#endif

/// Indicates that a pointer should not be null.
///
/// References should be preferred over pointers, but behave in strange ways such that they may not
/// be used in some cases (e.g. in `Span<T&>`). In such cases, this attribute should be used to
/// indicate that the pointer should not be null.
///
/// Example:
///
///   void Foo(Bar* HORUS_NONNULL ptr) { ... }
///
/// See
/// https://github.com/abseil/abseil-cpp/blob/12f4be6805ea58052ecf764b381619632b061cbf/absl/base/nullability.h#L225-L237.
#if __clang__
#define HORUS_NONNULL _Nonnull
#else
#define HORUS_NONNULL
#endif

/// Indicates that a pointer may be null.
///
/// This makes it clear that a pointer may be null, as opposed to a `HORUS_NONNULL` pointer.
///
/// Example:
///
///   void Foo(Bar* HORUS_NULLABLE ptr) { ... }
///
/// See
/// https://github.com/abseil/abseil-cpp/blob/12f4be6805ea58052ecf764b381619632b061cbf/absl/base/nullability.h#L242-L253.
#if __clang__
#define HORUS_NULLABLE _Nullable
#else
#define HORUS_NULLABLE
#endif

// NOLINTEND(*-macro-usage)

#endif  // HORUS_ATTRIBUTES_H_
