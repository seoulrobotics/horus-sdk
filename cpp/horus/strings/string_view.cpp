#include "horus/strings/string_view.h"

#include <cstddef>
#include <functional>  // IWYU pragma: keep for std::hash<>
#include <ios>         // IWYU pragma: keep for std::streamsize
#include <ostream>

#include "horus/hash/fnv1a.h"

namespace horus {

std::ostream& operator<<(std::ostream& out, StringView string) {
  return out.write(string.data(), static_cast<std::streamsize>(string.size()));
}

}  // namespace horus

std::size_t std::hash<horus::StringView>::operator()(horus::StringView string) const noexcept {
  return horus::Fnv1a(string);
}
