/// @file
///
/// The `Discard()` function.

#ifndef HORUS_UTILITY_DISCARD_H_
#define HORUS_UTILITY_DISCARD_H_

namespace horus {

/// Discards all given arguments. Note that the given arguments will _still_ be computed.
///
/// May be used to discard values returned by non-void functions to comply with rule `A0-1-2`:
///
/// > The value returned by a function having a non-void return type that is not an overloaded
/// > overloaded operator shall be used.
///
/// Prefer this to `std::ignore = ...` as `std::ignore` requires including `<tuple>`. Also prefer
/// this to `static_void<void>(...)` as it more explicitly states the intent of the operation and
/// supports multiple arguments.
template <class... Args>
constexpr void Discard(Args const&... /* args */) noexcept {}

}  // namespace horus

#endif  // HORUS_UTILITY_DISCARD_H_
