#include "horus/pb/testing/messages_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

TestMessage_SubMessage::TestMessage_SubMessage(const TestMessage_SubMessage& other) noexcept(false)
    : u32_{other.u32_}
    , i32_{other.i32_}
    , string_{other.string_}
    , set_fields_{other.set_fields_} {}

void TestMessage_SubMessage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, u32_);
  }
  if (set_fields_[1]) {
    SerializeField<std::int32_t>(writer, /*tag=*/ 2, i32_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, string_);
  }
}

void TestMessage_SubMessage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, u32_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::int32_t>(reader, i32_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, string_);
        set_fields_[2] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

TestMessage::TestMessage(const TestMessage& other) noexcept(false)
    : floatf_{other.floatf_}
    , doublef_{other.doublef_}
    , i32_{other.i32_}
    , i64_{other.i64_}
    , u32_{other.u32_}
    , u64_{other.u64_}
    , bytes_{other.bytes_}
    , string_{other.string_}
    , submessage_{other.submessage_}
    , boolf_{other.boolf_}
    , oneof_float_{other.oneof_float_}
    , oneof_double_{other.oneof_double_}
    , oneof_i32_{other.oneof_i32_}
    , oneof_i64_{other.oneof_i64_}
    , oneof_u32_{other.oneof_u32_}
    , oneof_u64_{other.oneof_u64_}
    , oneof_bytes_{other.oneof_bytes_}
    , oneof_string_{other.oneof_string_}
    , oneof_submessage_{other.oneof_submessage_}
    , oneof_bool_{other.oneof_bool_}
    , rep_float_{other.rep_float_}
    , rep_double_{other.rep_double_}
    , rep_i32_{other.rep_i32_}
    , rep_i64_{other.rep_i64_}
    , rep_u32_{other.rep_u32_}
    , rep_u64_{other.rep_u64_}
    , rep_bytes_{other.rep_bytes_}
    , rep_string_{other.rep_string_}
    , rep_submessage_{other.rep_submessage_}
    , rep_bool_{other.rep_bool_}
    , test_oneof_{other.test_oneof_}
    , set_fields_{other.set_fields_} {}

void TestMessage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 1, floatf_);
  }
  if (set_fields_[1]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, doublef_);
  }
  if (set_fields_[2]) {
    SerializeField<std::int32_t>(writer, /*tag=*/ 3, i32_);
  }
  if (set_fields_[3]) {
    SerializeField<std::int64_t>(writer, /*tag=*/ 4, i64_);
  }
  if (set_fields_[4]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 5, u32_);
  }
  if (set_fields_[5]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 6, u64_);
  }
  if (set_fields_[6]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 7, bytes_);
  }
  if (set_fields_[7]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 8, string_);
  }
  if (set_fields_[8]) {
    SerializeField<TestMessage_SubMessage>(writer, /*tag=*/ 9, submessage_);
  }
  if (set_fields_[9]) {
    SerializeField<bool>(writer, /*tag=*/ 10, boolf_);
  }
  if (set_fields_[10]) {
    SerializeField<float, PbDeserFlags::kFixed>(writer, /*tag=*/ 11, oneof_float_);
  }
  if (set_fields_[11]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 12, oneof_double_);
  }
  if (set_fields_[12]) {
    SerializeField<std::int32_t>(writer, /*tag=*/ 13, oneof_i32_);
  }
  if (set_fields_[13]) {
    SerializeField<std::int64_t>(writer, /*tag=*/ 14, oneof_i64_);
  }
  if (set_fields_[14]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 15, oneof_u32_);
  }
  if (set_fields_[15]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 16, oneof_u64_);
  }
  if (set_fields_[16]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 17, oneof_bytes_);
  }
  if (set_fields_[17]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 18, oneof_string_);
  }
  if (set_fields_[18]) {
    SerializeField<TestMessage_SubMessage>(writer, /*tag=*/ 19, oneof_submessage_);
  }
  if (set_fields_[19]) {
    SerializeField<bool>(writer, /*tag=*/ 20, oneof_bool_);
  }
  if (set_fields_[20]) {
    SerializeField<CowSpan<float>, PbDeserFlags::kFixed>(writer, /*tag=*/ 21, rep_float_);
  }
  if (set_fields_[21]) {
    SerializeField<CowSpan<double>, PbDeserFlags::kFixed>(writer, /*tag=*/ 22, rep_double_);
  }
  if (set_fields_[22]) {
    SerializeField<CowRepeated<std::int32_t>>(writer, /*tag=*/ 23, rep_i32_);
  }
  if (set_fields_[23]) {
    SerializeField<CowRepeated<std::int64_t>>(writer, /*tag=*/ 24, rep_i64_);
  }
  if (set_fields_[24]) {
    SerializeField<CowRepeated<std::uint32_t>>(writer, /*tag=*/ 25, rep_u32_);
  }
  if (set_fields_[25]) {
    SerializeField<CowRepeated<std::uint64_t>>(writer, /*tag=*/ 26, rep_u64_);
  }
  if (set_fields_[26]) {
    SerializeField<CowRepeated<CowBytes>>(writer, /*tag=*/ 27, rep_bytes_);
  }
  if (set_fields_[27]) {
    SerializeField<CowRepeated<CowBytes>>(writer, /*tag=*/ 28, rep_string_);
  }
  if (set_fields_[28]) {
    SerializeField<CowRepeated<TestMessage_SubMessage>>(writer, /*tag=*/ 29, rep_submessage_);
  }
  if (set_fields_[29]) {
    SerializeField<CowRepeated<bool>>(writer, /*tag=*/ 30, rep_bool_);
  }
}

void TestMessage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<float, PbDeserFlags::kFixed>(reader, floatf_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, doublef_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::int32_t>(reader, i32_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::int64_t>(reader, i64_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<std::uint32_t>(reader, u32_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<std::uint64_t>(reader, u64_);
        set_fields_[5] = true;
        break;
      }
      case 7: {
        DeserializeField<CowBytes>(reader, bytes_);
        set_fields_[6] = true;
        break;
      }
      case 8: {
        DeserializeField<CowBytes>(reader, string_);
        set_fields_[7] = true;
        break;
      }
      case 9: {
        DeserializeField<TestMessage_SubMessage>(reader, submessage_);
        set_fields_[8] = true;
        break;
      }
      case 10: {
        DeserializeField<bool>(reader, boolf_);
        set_fields_[9] = true;
        break;
      }
      case 11: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofFloat;
        DeserializeField<float, PbDeserFlags::kFixed>(reader, oneof_float_);
        set_fields_[10] = true;
        break;
      }
      case 12: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofDouble;
        DeserializeField<double, PbDeserFlags::kFixed>(reader, oneof_double_);
        set_fields_[11] = true;
        break;
      }
      case 13: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofI32;
        DeserializeField<std::int32_t>(reader, oneof_i32_);
        set_fields_[12] = true;
        break;
      }
      case 14: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofI64;
        DeserializeField<std::int64_t>(reader, oneof_i64_);
        set_fields_[13] = true;
        break;
      }
      case 15: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofU32;
        DeserializeField<std::uint32_t>(reader, oneof_u32_);
        set_fields_[14] = true;
        break;
      }
      case 16: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofU64;
        DeserializeField<std::uint64_t>(reader, oneof_u64_);
        set_fields_[15] = true;
        break;
      }
      case 17: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofBytes;
        DeserializeField<CowBytes>(reader, oneof_bytes_);
        set_fields_[16] = true;
        break;
      }
      case 18: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofString;
        DeserializeField<CowBytes>(reader, oneof_string_);
        set_fields_[17] = true;
        break;
      }
      case 19: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofSubmessage;
        DeserializeField<TestMessage_SubMessage>(reader, oneof_submessage_);
        set_fields_[18] = true;
        break;
      }
      case 20: {
        clear_test_oneof();
        test_oneof_ = TestOneofOneof::kOneofBool;
        DeserializeField<bool>(reader, oneof_bool_);
        set_fields_[19] = true;
        break;
      }
      case 21: {
        DeserializeField<CowSpan<float>, PbDeserFlags::kFixed>(reader, rep_float_);
        set_fields_[20] = true;
        break;
      }
      case 22: {
        DeserializeField<CowSpan<double>, PbDeserFlags::kFixed>(reader, rep_double_);
        set_fields_[21] = true;
        break;
      }
      case 23: {
        DeserializeField<CowRepeated<std::int32_t>>(reader, rep_i32_);
        set_fields_[22] = true;
        break;
      }
      case 24: {
        DeserializeField<CowRepeated<std::int64_t>>(reader, rep_i64_);
        set_fields_[23] = true;
        break;
      }
      case 25: {
        DeserializeField<CowRepeated<std::uint32_t>>(reader, rep_u32_);
        set_fields_[24] = true;
        break;
      }
      case 26: {
        DeserializeField<CowRepeated<std::uint64_t>>(reader, rep_u64_);
        set_fields_[25] = true;
        break;
      }
      case 27: {
        DeserializeField<CowRepeated<CowBytes>>(reader, rep_bytes_);
        set_fields_[26] = true;
        break;
      }
      case 28: {
        DeserializeField<CowRepeated<CowBytes>>(reader, rep_string_);
        set_fields_[27] = true;
        break;
      }
      case 29: {
        DeserializeField<CowRepeated<TestMessage_SubMessage>>(reader, rep_submessage_);
        set_fields_[28] = true;
        break;
      }
      case 30: {
        DeserializeField<CowRepeated<bool>>(reader, rep_bool_);
        set_fields_[29] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
