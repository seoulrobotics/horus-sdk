/// @file
///
/// The `Handle` class.

#ifndef HORUS_EVENT_LOOP_HANDLE_H_
#define HORUS_EVENT_LOOP_HANDLE_H_

#include <cassert>
#include <cstdint>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/event_loop/uv.h"
#include "horus/event_loop/waker.h"
#include "horus/future/core_owner.h"
#include "horus/future/poll.h"
#include "horus/internal/pointer_cast.h"

namespace horus {
namespace horus_internal {

/// A handle to the Horus event loop.
template <class H, class Owner>
class Handle final : public BaseHandle {
 public:
  /// Constructs a not-yet-engaged `Handle`.
  Handle() noexcept = default;

  /// Handles may not be copied.
  Handle(const Handle&) = delete;
  /// Handles may not be copied.
  Handle& operator=(const Handle&) = delete;

  /// Move-constructs a `Handle`.
  Handle(Handle&& other) noexcept : handle_{other.handle_} {
    other.AssertNotEngaged();
    other.handle_ = {};
  }

  /// Move-assigns a `Handle`.
  Handle& operator=(Handle&& other) noexcept {
    if (&other != this) {
      AssertNotEngaged();
      if (handle_.loop_ != nullptr) {
        ~Handle();
      }
      handle_ = other.handle_;
      other.AssertNotEngaged();
      other.handle_ = nullptr;
    }
    return *this;
  }

  /// Destroys the handle. The handle may only be destroyed if it is no longer engaged (i.e. after
  /// `PollDestroy()` returns true).
  ~Handle() noexcept { AssertNotEngaged(); }

  /// Returns a reference to the raw handle.
  const H& Raw() const noexcept { return *handle_; }

  /// Engages the handle and returns true. If the handle had already been engaged, returns false.
  ///
  /// This function may be called multiple times, although it must always be called with a `context`
  /// which refers to a same `EventLoop` and `FutureWaker`.
  template <class... Args>
  bool Engage(Owner* owner, Handle(Owner::*handle), PollContext& context,
              std::int32_t (*init)(uv_loop_t* loop, H* handle, Args... args), Args... args) {
    assert(&(owner->*handle) == this);
    static_cast<void>(handle);
    if (UvGetLoop(handle_) != nullptr) {
      assert(UvGetLoop(handle_) == EventLoopToUv(&context.Loop()));
      return false;
    }
    owner_ = owner;
    init(EventLoopToUv(&context.Loop()), &handle_.Get(), std::forward<Args>(args)...);
    OwnSelf(context.FutureOwner());
    waker_ = context.Waker();
    UvSetData(handle_.Get(), this);
    return true;
  }

  /// Engages the handle, then calls `F` with a reference to the handle. If the handle had already
  /// been engaged, does nothing and returns false.
  template <class... Args, class F>
  bool Engage(Owner* owner, Handle(Owner::*handle), PollContext& context,
              std::int32_t (*init)(uv_loop_t* loop, H* handle, Args... args), Args... args,
              const F& with_handle) {
    if (!Engage(owner, handle, context, init, std::forward<Args>(args)...)) {
      return false;
    }
    with_handle(handle_.Get());
    return true;
  }

  /// Closes the handle and wakes up the current task. `IsEngaged()` must be true when this function
  /// is called.
  ///
  /// Note that the handle will be closed asynchronously.
  void CloseAndWakeUp() noexcept {
    assert(IsEngaged());
    if (uv_is_closing(&UvToHandle(handle_))) {
      return;
    }
    uv_close(&UvToHandle(handle_), [](uv_handle_t* raw_handle) noexcept {
      Handle& self{*UnsafePointerCast<Handle>(UvGetData(*raw_handle))};
      self.waker_.Wake();
      self.DisownSelf();
      self.handle_ = {};
    });
  }

  /// Returns whether the handle was `Engage`d and not yet `Close`d.
  bool IsEngaged() const noexcept { return UvGetLoop(handle_) != nullptr; }

  /// Returns a pointer to the `Handle` recovered from `handle`.
  static Handle* SelfFromHandle(H* handle) noexcept {
    return UnsafePointerCast<Handle>(UvGetData(*handle));
  }

  /// Returns a pointer to the owner stored in the given `handle`.
  static Owner* OwnerFromHandle(H* handle) noexcept { return SelfFromHandle(handle)->owner_; }

  /// Returns a callback which wakes up the owning future when called.
  template <class... UnusedArgs>
  void (*CreateCloseAndWakeUpCallback() noexcept)(H* handle, UnusedArgs... args) {
    return [](H* handle, UnusedArgs... /* unused */) noexcept {
      SelfFromHandle(handle)->CloseAndWakeUp();
    };
  }

  /// Returns a callback which calls `Do` and wakes up the owning future when called.
  template <void (Owner::*Do)(), class... UnusedArgs>
  void (*CreateDoAndCloseAndWakeUpCallback() noexcept)(H* handle, UnusedArgs... args) {
    return [](H* handle, UnusedArgs... /* unused */) noexcept {
      (OwnerFromHandle(handle)->*Do)();
      SelfFromHandle(handle)->CloseAndWakeUp();
    };
  }

  /// @copydoc BaseHandle::PollDestroy()
  bool PollDestroy(const FutureWaker& waker) & noexcept final {
    if (!IsEngaged()) {
      return true;
    }
    waker_ = waker;
    CloseAndWakeUp();
    return false;
  }

 private:
  /// Asserts (in debug builds) that the `Handle` hasn't been engaged yet.
  void AssertNotEngaged() const noexcept { assert(!IsEngaged()); }

  /// The handle as used by libuv. Its `data` will point to `this`.
  Uv<H> handle_{};
  /// A pointer to the object which owns this `Handle`.
  Owner* owner_{};
  /// The waker to resume execution.
  FutureWaker waker_;
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_EVENT_LOOP_HANDLE_H_
