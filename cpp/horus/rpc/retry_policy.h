/// @file
///
/// The `RpcRetryPolicy` class.

#ifndef HORUS_RPC_RETRY_POLICY_H_
#define HORUS_RPC_RETRY_POLICY_H_

#include <chrono>
#include <cstdint>

namespace horus {

/// A policy when attempting to retry an RPC send through a `BaseClient`.
struct RpcRetryPolicy {
  /// `std::chrono` clock used by `deadline`.
  using DeadlineClock = std::chrono::steady_clock;

  /// The maximum number of retries. If -1, there is no maximum.
  std::uint16_t max_retries{};
  /// The interval at which retries should be attempted.
  std::chrono::milliseconds retry_interval{};
  /// If greater than `retry_interval`, `retry_interval` will be multiplied by two on every retry
  /// until it reaches `max_retry_interval`.
  std::chrono::milliseconds max_retry_interval{};

  /// The deadline after which we should stop trying to send the RPC or receive a response.
  DeadlineClock::time_point deadline{DeadlineClock::time_point::max()};

  /// Returns a copy of `this` with `deadline` set to `new_deadline`.
  constexpr RpcRetryPolicy WithDeadline(DeadlineClock::time_point new_deadline) const noexcept {
    RpcRetryPolicy result{*this};
    result.deadline = new_deadline;
    return result;
  }

  /// Returns a copy of `this` with `deadline` set to `now() + timeout`.
  RpcRetryPolicy WithTimeout(std::chrono::milliseconds timeout) const noexcept {
    return WithDeadline(DeadlineClock::now() + timeout);
  }
};

/// Returns a `RpcRetryPolicy` which never attempts to retry a `Send()`.
constexpr static RpcRetryPolicy DoNotRetry() noexcept {
  return {
      /*max_retries=*/0,
      /*retry_interval=*/std::chrono::milliseconds::zero(),
      /*max_retry_interval=*/std::chrono::milliseconds::zero(),
  };
}

/// Returns a `RpcRetryPolicy` which attempts to retry indefinitely.
constexpr static RpcRetryPolicy RetryIndefinitely(
    std::chrono::milliseconds retry_interval,
    std::chrono::milliseconds max_retry_interval = std::chrono::milliseconds::zero()) noexcept {
  return {
      /*max_retries=*/static_cast<std::uint16_t>(-1),
      /*retry_interval=*/retry_interval,
      /*max_retry_interval=*/max_retry_interval,
  };
}

/// Returns the default `RpcRetryPolicy` for clients: retry indefinitely, starting at 100ms, up to
/// 10s.
///
/// @note The specific details of this policy may change over time, so if you would like to use
/// specific parameters for the policy, do not use this method.
constexpr static RpcRetryPolicy RetryClientDefault() noexcept {
  constexpr std::chrono::milliseconds kRetryInterval{100};
  constexpr std::chrono::seconds kMaxRetryInterval{10};

  return {
      /*max_retries=*/static_cast<std::uint16_t>(-1),
      /*retry_interval=*/kRetryInterval,
      /*max_retry_interval=*/kMaxRetryInterval,
  };
}

/// Returns the default `RpcRetryPolicy` for clients of servers (usually, subscribers): retry once
/// after 50ms, then throw.
///
/// @note The specific details of this policy may change over time, so if you would like to use
/// specific parameters for the policy, do not use this method.
constexpr static RpcRetryPolicy RetryServerClientDefault() noexcept {
  constexpr std::chrono::milliseconds kRetryInterval{50};

  return {
      /*max_retries=*/1,
      /*retry_interval=*/kRetryInterval,
      /*max_retry_interval=*/kRetryInterval,
  };
}

}  // namespace horus

#endif  // HORUS_RPC_RETRY_POLICY_H_
