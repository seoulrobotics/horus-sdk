#include "horus/pb/serialize.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <protozero/data_view.hpp>
#include <protozero/types.hpp>
#include <protozero/varint.hpp>

#include "horus/pb/buffer.h"
#include "horus/pb/types.h"

namespace horus {

PbView PbReader::ViewIncludingTag(PbTag tag, protozero::data_view view) const noexcept {
  // To get to the view, we first read `tag_and_type` and `view.size()`, both of which are
  // varints.
  std::uint32_t const tag_and_type{
      protozero::tag_and_type(tag, protozero::pbf_wire_type::length_delimited)};
  std::size_t const tag_size{static_cast<std::size_t>(protozero::length_of_varint(tag_and_type))};
  std::size_t const len_size{static_cast<std::size_t>(protozero::length_of_varint(view.size()))};
  std::size_t const skipped{tag_size + len_size};

  PbView result{buffer_.View(static_cast<std::size_t>(view.data() - buffer_.Str().data()) - skipped,
                             view.size() + skipped)};

  // Checks that the given `view` points to a length-delimited field with the given `tag` and
  // `view_len`.
  assert(([&result, tag, &view]() noexcept {  // NOLINT(*-exception-escape)
    PbReader reader{PbView{result}};
    assert(reader.Reader().next());
    assert(reader.Reader().tag() == tag);
    assert(reader.Reader().wire_type() == protozero::pbf_wire_type::length_delimited);
    assert(reader.Reader().get_view().size() == view.size());
    return true;
  }()));
  return result;
}

}  // namespace horus
