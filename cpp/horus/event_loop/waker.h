/// @file
///
/// The `FutureWaker` class.

#ifndef HORUS_EVENT_LOOP_WAKER_H_
#define HORUS_EVENT_LOOP_WAKER_H_

#include <atomic>
#include <memory>

#include "horus/event_loop/uv.h"
#include "horus/internal/source_location.h"

namespace horus {
namespace horus_internal {

/// An object which can schedule for a future to resume its execution in the event loop.
class FutureWaker final {
  /// State shared between `FutureWaker`s and their `Owner`.
  struct Shared {
    /// The inner handle.
    Uv<uv_async_t> handle;
    /// The location of the last call to `Wake()`.
    std::atomic<const char*> wake_source_location{nullptr};

    /// Wakes up the task.
    void Wake(SourceLocation source_location);
  };

 public:
  /// The owner of a `FutureWaker`.
  class Owner final {
   public:
    /// Constructs a `FutureWaker` `Owner` which will schedule an async handle when woken up.
    Owner() noexcept(false) { UvSetData(shared_->handle, this); }

    /// `Owner`s cannot be copied or moved.
    Owner(const Owner&) = delete;
    /// `Owner`s cannot be copied or moved.
    Owner& operator=(const Owner&) = delete;
    /// `Owner`s cannot be copied or moved.
    Owner(Owner&&) = delete;
    /// `Owner`s cannot be copied or moved.
    Owner& operator=(Owner&&) = delete;

    /// Destroys the `Owner` and its owned async handle.
    ~Owner() noexcept = default;

    /// Returns a (non-null) pointer to the async handle.
    uv_async_t* Handle() noexcept { return &shared_->handle.Get(); }

    /// Returns a `FutureWaker` which refers to this object.
    FutureWaker Waker() noexcept { return FutureWaker{shared_}; }

    /// Wakes up the `FutureWaker`.
    void Wake(SourceLocation source_location = CurrentSourceLocation()) noexcept {
      shared_->Wake(source_location);
    }

   private:
    /// The (non-null, owned) shared state.
    std::shared_ptr<Shared> shared_{std::make_shared<Shared>()};
  };

  /// Constructs an empty `FutureWaker`.
  FutureWaker() noexcept = default;

  /// Copy-constructs the waker.
  FutureWaker(const FutureWaker&) noexcept = default;
  /// Copy-assigns the waker.
  FutureWaker& operator=(const FutureWaker&) noexcept = default;

  /// Move-constructs the waker.
  FutureWaker(FutureWaker&&) noexcept = default;

  /// Move-assigns the waker.
  FutureWaker& operator=(FutureWaker&&) noexcept = default;

  /// Destroys the `FutureWaker`.
  ~FutureWaker() noexcept = default;

  /// Returns whether the waker is empty, in which case `Wake()` is a no-op.
  bool IsEmpty() const noexcept { return state_.expired(); }

  /// Schedules for the execution of the bound future to resume in the event loop.
  ///
  /// This function may be called multiple times in a row, which will be coalesced into a single
  /// poll operation.
  void Wake(SourceLocation source_location = CurrentSourceLocation()) noexcept {
    const std::shared_ptr<Shared> state{state_.lock()};
    if (state != nullptr) {
      state->Wake(source_location);
    }
  }

 private:
  /// Constructs a `FutureWaker` owned by `owner`.
  explicit FutureWaker(const std::shared_ptr<Shared>& state) noexcept : state_{state} {}

  /// A (possibly null) pointer to the waker's `Shared` state.
  std::weak_ptr<Shared> state_{};
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_EVENT_LOOP_WAKER_H_
