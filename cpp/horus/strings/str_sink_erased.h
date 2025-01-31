/// @file
///
/// The `ErasedSink` class.

#ifndef HORUS_STRINGS_STR_SINK_ERASED_H_
#define HORUS_STRINGS_STR_SINK_ERASED_H_

#include <utility>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_cast.h"
#include "horus/types/string_view.h"

namespace horus {

/// A wrapper around a `Sink` which forwards `Append()` calls to it.
class ErasedSink final {
 public:
  /// Constructs an `ErasedSink` which forwards `Append()` calls to `sink`.
  template <class Sink, decltype(std::declval<Sink&>().Append(StringView{}))* = nullptr>
  ErasedSink(Sink& sink HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept  // NOLINT(*-explicit-*)
      : sink_{&sink}, append_{[](const void* sink_ptr, StringView string) {
          horus_internal::UnsafePointerCast<Sink>(horus_internal::UnsafeConstCast(sink_ptr))
              ->Append(string);
        }} {}

  /// Appends `string` to the underlying sink.
  ///
  /// @throws std::exception Any exception thrown by the underlying sink.
  void Append(StringView string) const noexcept(false) { append_(sink_, string); }

 private:
  /// A (non-null) pointer to the underlying sink accepted by `append_`.
  const void* sink_;
  /// A (non-null) pointer to a function which, when called with `sink` and a `string`, appends
  /// `string` to `sink`.
  void (*append_)(const void* sink, StringView string);
};

}  // namespace horus

#endif  // HORUS_STRINGS_STR_SINK_ERASED_H_
