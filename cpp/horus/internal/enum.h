/// @file
///
/// The `UnhandledEnumValueError` class.

#ifndef HORUS_INTERNAL_ENUM_H_
#define HORUS_INTERNAL_ENUM_H_

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

#include "horus/strings/string_view.h"

namespace horus {
namespace horus_internal {

/// The exception thrown by `UnhandledEnumValue()`.
///
/// This is considered a **logic error** as code should not produce invalid enums (instead
/// validating enums on construction) and switch statements should check all cases of an enum.
class UnhandledEnumValueError final : public std::logic_error {
 public:
  /// Creates an error caused by an enum of the given value.
  template <class E, std::enable_if_t<std::is_enum<E>::value &&
                                      sizeof(E) <= sizeof(std::int32_t)>* = nullptr>
  explicit UnhandledEnumValueError(E value)
      : horus::horus_internal::UnhandledEnumValueError{typeid(E).name(),
                                                       static_cast<std::int32_t>(value)} {}

 private:
  /// Constructs the error.
  UnhandledEnumValueError(StringView type_name, std::int32_t value);
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_ENUM_H_
