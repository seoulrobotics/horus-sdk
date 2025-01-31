#include "horus/strings/ansi.h"

#include <unistd.h>

#include <cstdlib>

namespace horus {

bool DisplayTerminalColors() noexcept {
  static const bool result{(std::getenv("NO_COLOR") == nullptr) &&  // NOLINT(concurrency-mt-unsafe)
                           static_cast<bool>(isatty(STDOUT_FILENO))};
  return result;
}

}  // namespace horus
