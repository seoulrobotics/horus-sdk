#include "horus/event_loop/waker.h"

#include <uv.h>

#include <atomic>

#include "horus/event_loop/uv.h"
#include "horus/internal/source_location.h"

namespace horus {
namespace horus_internal {

void FutureWaker::Shared::Wake(SourceLocation source_location) {
  wake_source_location.store(source_location.File().data(), std::memory_order_release);

  UvAssert(uv_async_send(&handle.Get()));
}

}  // namespace horus_internal
}  // namespace horus
