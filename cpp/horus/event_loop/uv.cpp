#include "horus/event_loop/uv.h"

#include <uv.h>

#include <cassert>
#include <new>
#include <system_error>

#include "horus/internal/pointer_cast.h"

namespace horus {
namespace horus_internal {

void UvThrowIfError(UvStatus libuv_status) noexcept(false) {
  if (libuv_status < 0) {
    // https://docs.libuv.org/en/v1.x/errors.html:
    //
    // > Implementation detail: on Unix error codes are the negated errno (or -errno) [...]
    if (libuv_status == UV_ENOMEM) {
      // Allocator function failed, so `std::bad_alloc` is the most precise error which
      // corresponds.
      throw std::bad_alloc{};
    }
    throw std::system_error{-libuv_status, std::generic_category(), uv_err_name(libuv_status)};
  }
}

void UvAssert(UvStatus libuv_status) noexcept {
  assert_perror(-libuv_status);
  static_cast<void>(libuv_status);
}

static_assert(UvErrors::kCanceled == UV_ECANCELED, "");

template <class T>
Uv<T>::Uv() noexcept {
  static_assert(sizeof(T) == sizeof(Uv<T>), "");
  static_assert(alignof(T) == alignof(Uv<T>), "");

  Get() = T{};
}

template Uv<uv_async_t>::Uv() noexcept;
template Uv<uv_loop_t>::Uv() noexcept;
template Uv<uv_req_t>::Uv() noexcept;
template Uv<uv_signal_t>::Uv() noexcept;
template Uv<uv_tcp_t>::Uv() noexcept;
template Uv<uv_timer_t>::Uv() noexcept;
template Uv<uv_work_t>::Uv() noexcept;

uv_loop_t* UvGetLoop(uv_work_t const& work) noexcept { return work.loop; }

}  // namespace horus_internal
}  // namespace horus
