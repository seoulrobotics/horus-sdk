/// @file
///
/// Error type for RPC methods invocations

#ifndef HORUS_SDK_ERRORS_H_
#define HORUS_SDK_ERRORS_H_

#include <stdexcept>
#include <vector>

#include "horus/pb/detection_service/detection_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"

namespace horus {
namespace sdk {

/// Represents an application error returned by a RPC endpoint.
class RpcApplicationError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

/// The data returned by the RPC endpoint are unexpected.
class UnexpectedDataReceivedFromRpcError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_ERRORS_H_
