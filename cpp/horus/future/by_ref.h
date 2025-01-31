/// @file
///
/// The `ByRef()` function.

#ifndef HORUS_FUTURE_BY_REF_H_
#define HORUS_FUTURE_BY_REF_H_

#include <functional>

#include "horus/future/future.h"
#include "horus/internal/attributes.h"

namespace horus {

// MARK: ByRef()

/// Future returned by `ByRef()`.
template <class F>
class ByRefFuture final : public Future<FutureResult<F>> {
 public:
  /// Constructs the future.
  explicit ByRefFuture(F& future HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept : future_{future} {}

  /// Polls the referenced future until completion.
  PollResult<FutureResult<F>> UnsafePoll(PollContext& context) final {
    return ByRefFuture::PollFuture(future_.get(), context);
  }

 private:
  /// A reference to the underlying `future`.
  std::reference_wrapper<F> future_;
};

/// Returns a future which polls `future` by reference.
template <class F>
inline ByRefFuture<F> ByRef(F& future HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
  return ByRefFuture<F>{future};
}

}  // namespace horus

#endif  // HORUS_FUTURE_BY_REF_H_
