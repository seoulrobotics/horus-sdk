/// @file
///
/// Forward-declarations and aliases for working with libuv without exposing it in public headers.

#ifndef HORUS_EVENT_LOOP_UV_H_
#define HORUS_EVENT_LOOP_UV_H_

#include <array>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "horus/attributes.h"
#include "horus/event_loop/uv_handles.h"  // IWYU pragma: export

// MARK: Forward declarations
//

extern "C" {
/// See https://docs.libuv.org/en/v1.x/threadpool.html#c.uv_after_work_cb.
using uv_after_work_cb = void (*)(uv_work_t*, std::int32_t);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_close_cb.
using uv_close_cb = void (*)(uv_handle_t*);
/// See https://docs.libuv.org/en/v1.x/threadpool.html#c.uv_work_cb.
using uv_work_cb = void (*)(uv_work_t*);

/// See https://docs.libuv.org/en/v1.x/request.html#c.uv_cancel.
std::int32_t uv_cancel(uv_req_t* req);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_close.
void uv_close(uv_handle_t* handle, uv_close_cb close_cb);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_is_closing.
std::int32_t uv_is_closing(uv_handle_t const* handle);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_handle_get_data.
void* uv_handle_get_data(uv_handle_t const* handle);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_handle_get_loop.
uv_loop_t* uv_handle_get_loop(uv_handle_t const* handle);
/// See https://docs.libuv.org/en/v1.x/handle.html#c.uv_handle_set_data.
void uv_handle_set_data(uv_handle_t* handle, void* data);
/// See https://docs.libuv.org/en/v1.x/loop.html#c.uv_loop_get_data.
void* uv_loop_get_data(uv_loop_t const* loop);
/// See https://docs.libuv.org/en/v1.x/loop.html#c.uv_loop_set_data.
void uv_loop_set_data(uv_loop_t* loop, void* data);
/// See https://docs.libuv.org/en/v1.x/threadpool.html#c.uv_queue_work.
std::int32_t uv_queue_work(uv_loop_t* loop, uv_work_t* req, uv_work_cb work_cb,
                           uv_after_work_cb after_work_cb);
/// See https://docs.libuv.org/en/v1.x/request.html#c.uv_req_get_data.
void* uv_req_get_data(uv_req_t const* req);
/// See https://docs.libuv.org/en/v1.x/request.html#c.uv_req_set_data.
void uv_req_set_data(uv_req_t* req, void* data);
}

namespace horus {
namespace horus_internal {

// MARK: UvStatus
//

/// Status returned by libuv functions.
using UvStatus = std::int32_t;

/// Some libuv error codes.
struct UvErrors {
  /// `UV_ECANCELED`.
  static constexpr UvStatus kCanceled{-125};
};

/// Checks a status code returned by a libuv call, throwing a `std::system_error` it is an error
/// (or a `std::bad_alloc` if the error is due to a failed allocation).
void UvThrowIfError(UvStatus libuv_status) noexcept(false);

/// Asserts that the given libuv status is not an error in debug builds. No-op in release builds.
void UvAssert(std::int32_t libuv_status) noexcept;

// MARK: Uv{From,To}Handle()
//

/// Casts `ptr` to `T*`. This is unsafe, if possible, use other cast functions below instead.
template <class T>
T* UvUnsafeCast(void const* ptr) noexcept {
  static_assert(sizeof(T*) == sizeof(void*), "");
  T* result{nullptr};
  static_cast<void>(
      // NOLINTNEXTLINE(*-multi-level-implicit-pointer-conversion): llvm-project/issues/93959
      std::memcpy(static_cast<void*>(&result), static_cast<void const*>(&ptr), sizeof(void*)));
  return result;
}

/// Defines `value` to be true iff `T` is a libuv handle type.
template <class T>
struct UvIsHandleType : std::false_type {};
template <>
struct UvIsHandleType<uv_async_t> : std::true_type {};
template <>
struct UvIsHandleType<uv_handle_t> : std::true_type {};
template <>
struct UvIsHandleType<uv_signal_t> : std::true_type {};
template <>
struct UvIsHandleType<uv_tcp_t> : std::true_type {};
template <>
struct UvIsHandleType<uv_timer_t> : std::true_type {};

/// Casts `handle` to `H const*`.
template <class H>
H const& UvFromHandle(uv_handle_t const& handle HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsHandleType<H>::value, "");
  return *UvUnsafeCast<H const>(&handle);
}
/// Casts `handle` to `H*`.
template <class H>
H& UvFromHandle(uv_handle_t& handle HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsHandleType<H>::value, "");
  return *UvUnsafeCast<H>(&handle);
}

/// Casts `handle` to `uv_handle_t const*`.
template <class H>
uv_handle_t const& UvToHandle(H const& handle HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsHandleType<H>::value, "");
  return *UvUnsafeCast<uv_handle_t const>(&handle);
}
/// Casts `handle` to `uv_handle_t*`.
template <class H>
uv_handle_t& UvToHandle(H& handle HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsHandleType<H>::value, "");
  return *UvUnsafeCast<uv_handle_t>(&handle);
}

// MARK: Uv{From,To}Req()
//

/// Defines `value` to be true iff `T` is a libuv request type.
template <class T>
struct UvIsReqType : std::false_type {};
template <>
struct UvIsReqType<uv_req_t> : std::true_type {};
template <>
struct UvIsReqType<uv_work_t> : std::true_type {};

/// Casts `req` to `R const*`.
template <class R>
R const& UvFromReq(uv_req_t const& req HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsReqType<R>::value, "");
  return *UvUnsafeCast<R const>(&req);
}
/// Casts `req` to `R*`.
template <class R>
R& UvFromReq(uv_req_t& req HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsReqType<R>::value, "");
  return *UvUnsafeCast<R>(&req);
}

/// Casts `req` to `uv_req_t const*`.
template <class R>
uv_req_t const& UvToReq(R const& req HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsReqType<R>::value, "");
  return *UvUnsafeCast<uv_req_t const>(&req);
}
/// Casts `req` to `uv_req_t*`.
template <class R>
uv_req_t& UvToReq(R& req HORUS_LIFETIME_BOUND) noexcept {
  static_assert(UvIsReqType<R>::value, "");
  return *UvUnsafeCast<uv_req_t>(&req);
}

// MARK: Uv<T>
//

/// A wrapper around a (possibly undefined) libuv type.
///
/// This wrapper can be used to refer to a libuv type (such as `uv_async_t`) in a header without
/// including `<uv.h>`. `Uv<uv_async_t>` has the same layout as `uv_async_t` and provides access to
/// the inner `uv_async_t` via `Get()`, but it does not require `uv_async_t` to actually be
/// _defined_ in this scope.
template <class T>
class Uv final {
 public:
  /// Zero-constructs the wrapper.
  ///
  /// This is instantiated for a set of specific types in `uv.cpp`.
  Uv() noexcept;

  /// Returns a reference to the inner value.
  T const& Get() const& noexcept HORUS_LIFETIME_BOUND {
    return *UvUnsafeCast<T const>(data_.data());
  }
  /// Returns a reference to the inner value.
  T& Get() & noexcept HORUS_LIFETIME_BOUND { return *UvUnsafeCast<T>(data_.data()); }

 private:
  /// Actual data needed by the wrapper.
  alignas(LayoutOf<T>::kAlign) std::array<std::uint8_t, LayoutOf<T>::kSize> data_;
};

/// Casts `handle` to `uv_handle_t const*`.
template <class H>
uv_handle_t const& UvToHandle(Uv<H> const& handle HORUS_LIFETIME_BOUND) noexcept {
  return UvToHandle(handle.Get());
}
/// Casts `handle` to `uv_handle_t*`.
template <class H>
uv_handle_t& UvToHandle(Uv<H>& handle HORUS_LIFETIME_BOUND) noexcept {
  return UvToHandle(handle.Get());
}

/// Casts `req` to `uv_req_t const*`.
template <class R>
uv_req_t const& UvToReq(Uv<R> const& req HORUS_LIFETIME_BOUND) noexcept {
  return UvToReq(req.Get());
}
/// Casts `req` to `uv_req_t*`.
template <class R>
uv_req_t& UvToReq(Uv<R>& req HORUS_LIFETIME_BOUND) noexcept {
  return UvToReq(req.Get());
}

// MARK: Uv{Get,Set}Data()
//

/// Returns the user data stored in the given loop.
inline void* UvGetData(uv_loop_t const& loop) noexcept { return uv_loop_get_data(&loop); }
/// Returns the user data stored in the given handle.
template <class T, std::enable_if_t<UvIsHandleType<T>::value>* = nullptr>
inline void* UvGetData(T const& handle) noexcept {
  return uv_handle_get_data(&UvToHandle(handle));
}
/// Returns the user data stored in the given request.
template <class T, std::enable_if_t<UvIsReqType<T>::value>* = nullptr>
inline void* UvGetData(T const& req) noexcept {
  return uv_req_get_data(&UvToReq(req));
}
/// Returns the user data stored in the given value.
template <class T>
inline void* UvGetData(Uv<T> const& value) noexcept {
  return UvGetData(value.Get());
}

/// Sets the user data stored in the given loop.
inline void UvSetData(uv_loop_t& loop, void* data) noexcept { uv_loop_set_data(&loop, data); }
/// Sets the user data stored in the given handle.
template <class T, std::enable_if_t<UvIsHandleType<T>::value>* = nullptr>
inline void UvSetData(T& handle, void* data) noexcept {
  uv_handle_set_data(&UvToHandle(handle), data);
}
/// Sets the user data stored in the given request.
template <class T, std::enable_if_t<UvIsReqType<T>::value>* = nullptr>
inline void UvSetData(T& req, void* data) noexcept {
  uv_req_set_data(&UvToReq(req), data);
}
/// Sets the user data stored in the given value.
template <class T>
inline void UvSetData(Uv<T>& value, void* data) noexcept {
  UvSetData(value.Get(), data);
}

// MARK: UvGetLoop()
//

/// Returns the loop of the given handle.
template <class T, std::enable_if_t<UvIsHandleType<T>::value>* = nullptr>
inline uv_loop_t* UvGetLoop(T const& handle) noexcept {
  return uv_handle_get_loop(&UvToHandle(handle));
}
/// Returns the loop of the given work request.
uv_loop_t* UvGetLoop(uv_work_t const& work) noexcept;
/// Returns the loop of the given value.
template <class T>
inline uv_loop_t* UvGetLoop(Uv<T> const& value) noexcept {
  return UvGetLoop(value.Get());
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_EVENT_LOOP_UV_H_
