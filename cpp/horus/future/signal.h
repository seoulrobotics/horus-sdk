/// @file
///
/// The `AwaitSignal()` function.

#ifndef HORUS_FUTURE_SIGNAL_H_
#define HORUS_FUTURE_SIGNAL_H_

#include <cassert>
#include <csignal>

#include "horus/event_loop/handle.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"

namespace horus {

// MARK: AwaitSignal()

/// A signal to await in `AwaitSignal()`.
enum class Signal : decltype(SIGINT) {  // NOLINT(*-enum-size)
  /// Interactive attention signal.
  kInt = SIGINT,
  /// Illegal instruction.
  kIll = SIGILL,
  /// Abnormal termination.
  kAbrt = SIGABRT,  // spell-checker: disable-line
  /// Erroneous arithmetic operation.
  kFpe = SIGFPE,
  /// Invalid access to storage.
  kSegv = SIGSEGV,
  /// Termination request.
  kTerm = SIGTERM,
};

/// Future returned by `AwaitSignal()`.
class AwaitSignalFuture final : public Future<void> {
 public:
  /// Constructs the future.
  explicit AwaitSignalFuture(Signal signal, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : signum_{static_cast<decltype(SIGINT)>(signal)}, info_{info} {
    assert(signum_ != 0);
  }

  /// Polls the future until the specified signal is received.
  PollResult<void> UnsafePoll(PollContext& context) final;

 private:
  /// Marks that the future was signaled.
  constexpr void MarkSignaled() noexcept { signum_ = 0; }

  /// The number of the signal to await. If 0, the signal was received and the future has completed.
  decltype(SIGINT) signum_;

  /// The signal handle.
  horus_internal::Handle<uv_signal_t, AwaitSignalFuture> handle_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when a signal is received.
inline AwaitSignalFuture AwaitSignal(Signal signal,
                                     const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return AwaitSignalFuture{signal, info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_SIGNAL_H_
