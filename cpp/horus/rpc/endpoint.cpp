#include "horus/rpc/endpoint.h"

// IWYU pragma: no_include <new>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/resolved.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

/// Returns whether `chr` needs to be escaped in `MakeInvalidDataErrorString()`.
constexpr bool NeedsEscaping(char chr) noexcept { return chr >= 32 && chr < 127; }

/// Constructs the error message for `InvalidDataError`.
std::string MakeInvalidDataErrorString(StringView invalid_message_bytes) noexcept(false) {
  static constexpr StringView kPrefix{"invalid protobuf data: \""};
  static constexpr StringView kSuffix{"\""};
  static constexpr StringView kHexChars{"0123456789abcdef"};

  // Compute size of string.
  std::size_t encoded_string_size{0};
  for (const char chr : invalid_message_bytes) {
    switch (chr) {
      case '\n':
      case '\r':
      case '\t':
      case '\"':
      case '\'':
      case '\\': {
        encoded_string_size += 2;
        break;
      }
      default: {
        if (NeedsEscaping(chr)) {
          encoded_string_size += 4;
        } else {
          encoded_string_size += 1;
        }
        break;
      }
    }
  }

  // Build and return string.
  std::string error_string;
  error_string.reserve(kPrefix.size() + encoded_string_size + kSuffix.size());
  error_string.append(kPrefix);
  for (const char chr : invalid_message_bytes) {
    switch (chr) {
      case '\n': {
        error_string.append("\\n");
        break;
      }
      case '\r': {
        error_string.append("\\r");
        break;
      }
      case '\t': {
        error_string.append("\\t");
        break;
      }
      case '\"': {
        error_string.append("\\\"");
        break;
      }
      case '\'': {
        error_string.append("\\'");
        break;
      }
      case '\\': {
        error_string.append("\\\\");
        break;
      }
      default: {
        if (NeedsEscaping(chr)) {
          const std::array<char, 4> buffer{'\\',  //
                                           'x',   //
                                           kHexChars[static_cast<std::size_t>(chr / 16)],
                                           kHexChars[static_cast<std::size_t>(chr % 16)]};
          error_string.append(buffer.data(), buffer.size());
        } else {
          error_string.push_back(chr);
        }
        break;
      }
    }
  }
  error_string.append(kSuffix);
  return error_string;
}

}  // namespace

InvalidDataError::InvalidDataError(StringView invalid_message_bytes)
    : std::runtime_error{MakeInvalidDataErrorString(invalid_message_bytes)} {}

MessageHandler NoMessageHandler() noexcept {
  return +[](const RpcContext& /* context */,
             sdk::pb::RpcMessage&& message) -> AnyFuture<sdk::pb::RpcMessage> {
    static_cast<void>(sdk::pb::RpcMessage{std::move(message)});
    return ResolveWith(sdk::pb::RpcMessage{});
  };
}

}  // namespace horus
