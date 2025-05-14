/// @file
///
/// The `ErasedSink` class.

#ifndef HORUS_STRINGS_ERASED_SINK_H_
#define HORUS_STRINGS_ERASED_SINK_H_

#include <type_traits>
#include <utility>

#include "horus/attributes.h"
#include "horus/pointer/erased.h"
#include "horus/strings/string_view.h"

namespace horus {

/// A wrapper around a Horus formatting `Sink` which forwards `Append()` calls to it.
class ErasedSink final {
 public:
  /// Constructs an `ErasedSink` which forwards `Append()` calls to `sink`.
  template <class Sink,
            std::enable_if_t<!std::is_same<std::decay_t<Sink>, ErasedSink>::value>* = nullptr,
            decltype(std::declval<Sink&>().Append(StringView{}))* = nullptr>
  ErasedSink(Sink& sink HORUS_LIFETIME_BOUND) noexcept  // NOLINT(*-explicit-*)
      : sink_{&sink}, append_{[](ErasedPointer sink_ptr, StringView string) {
          sink_ptr.UnsafeAs<Sink>()->Append(string);
        }} {}

  /// Appends `string` to the underlying sink.
  ///
  /// @throws std::exception Any exception thrown by the underlying sink.
  void Append(StringView string) const { append_(sink_, string); }

 private:
  /// A (non-null) pointer to the underlying sink accepted by `append_`.
  ErasedPointer sink_;
  /// A (non-null) pointer to a function which, when called with `sink` and a `string`, appends
  /// `string` to `sink`.
  void (*append_)(ErasedPointer sink, StringView string);
};

}  // namespace horus

#endif  // HORUS_STRINGS_ERASED_SINK_H_
