/// @file
///
/// Helpers for printing `std::chrono` durations and time points.

#ifndef HORUS_STRINGS_CHRONO_H_
#define HORUS_STRINGS_CHRONO_H_

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <limits>
#include <ratio>

#include "horus/strings/pad.h"
#include "horus/strings/str_sink.h"
#include "horus/types/string_view.h"

namespace horus {

/// Appends a `duration` to `sink` as a `Rep` number followed by the unit corresponding to
/// `Period`.
template <class Sink, class Rep, std::size_t Denom>
constexpr void HorusStringify(
    Sink& sink,
    std::chrono::duration<Rep, std::ratio<1, Denom>> duration) noexcept(IsNoexceptSink<Sink>()) {
  StringView suffix;
  if (Denom == 1L) {
    suffix = "s";
  } else if (Denom == 1'000L) {
    suffix = "ms";
  } else if (Denom == 1'000'000L) {
    suffix = "us";
  } else if (Denom == 1'000'000'000L) {
    suffix = "ns";
  } else {
    StrAppendToSink(sink, std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count(),
                    "ns");
    return;
  }
  StrAppendToSink(sink, duration.count(), suffix);
}

/// A UTC date in the ISO-8601 format with 'T' replaced with ' '.
class Iso8601 final {
 public:
  /// Constructs a date to format given its `time_point`.
  template <class Clock>
  constexpr explicit Iso8601(std::chrono::time_point<Clock> time_point) noexcept
      : ms_{std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())
                .count()} {}

  /// Implementation of `HorusStringify()`.
  template <class Sink>
  constexpr void InternalStringify(Sink& sink) const& noexcept(IsNoexceptSink<Sink>());

 private:
  /// The timestamp in milliseconds.
  std::chrono::milliseconds ms_;
};

template <class Sink>
constexpr void Iso8601::InternalStringify(Sink& sink) const& noexcept(IsNoexceptSink<Sink>()) {
  std::time_t const timestamp_time{
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point{ms_})};
  std::tm timestamp_tm{};
  static_cast<void>(localtime_r(&timestamp_time, &timestamp_tm));
  std::int64_t const last_ms{ms_.count() % 1'000L};

  /// Years to add to `tm_year` to obtain the actual year. Source:
  /// https://en.cppreference.com/w/c/chrono/tm.
  constexpr std::int32_t kStartYear{1900};

  StrAppendToSink(
      sink, PadLeftBy(4, kStartYear + timestamp_tm.tm_year, '0'), "-",
      PadLeftBy(2, timestamp_tm.tm_mon + 1, '0'), "-", PadLeftBy(2, timestamp_tm.tm_mday, '0'), " ",
      PadLeftBy(2, timestamp_tm.tm_hour, '0'), ":", PadLeftBy(2, timestamp_tm.tm_min, '0'), ":",
      PadLeftBy(2, timestamp_tm.tm_sec, '0'), ".", PadLeftBy(3, last_ms, '0'), "Z");
}

/// Appends a `timestamp` to `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink, Iso8601 timestamp) noexcept(IsNoexceptSink<Sink>()) {
  timestamp.InternalStringify(sink);
}

/// A duration value which should be formatted with a unit chosen based on its value.
///
/// For instance, 1000ms will be formatted as "1s", and 100ms will be formatted as "100ms".
class AnyDuration final {
 public:
  /// Constructs a duration given its milliseconds value.
  constexpr explicit AnyDuration(std::chrono::milliseconds milliseconds) noexcept
      : ms_{milliseconds.count()} {}
  /// Constructs a duration given its seconds value.
  constexpr explicit AnyDuration(std::chrono::seconds seconds) noexcept
      : AnyDuration{std::chrono::duration_cast<std::chrono::milliseconds>(seconds)} {}

  /// Implementation of `HorusStringify()`.
  template <class Sink>
  constexpr void InternalStringify(Sink& sink) const& noexcept(IsNoexceptSink<Sink>());

 private:
  /// The duration in milliseconds.
  std::int64_t ms_;
};

template <class Sink>
constexpr void AnyDuration::InternalStringify(Sink& sink) const& noexcept(IsNoexceptSink<Sink>()) {
  if (ms_ == 0) {
    StrAppendToSink(sink, "0s");
    return;
  }

  StringView minus;
  std::uint64_t abs{0};
  if (ms_ == std::numeric_limits<std::int64_t>::min()) {
    abs = std::numeric_limits<std::int64_t>::max();
    minus = "-";
  } else if (ms_ < 0) {
    abs = static_cast<std::uint64_t>(-ms_);
    minus = "-";
  } else {
    abs = static_cast<std::uint64_t>(ms_);
  }

  constexpr std::uint64_t kHundred{100UL};
  constexpr std::uint64_t kThousand{1'000UL};

  if (abs < kThousand) {
    StrAppendToSink(sink, minus, abs, "ms");
  } else if (abs % kThousand == 0) {
    StrAppendToSink(sink, minus, abs / kThousand, "s");
  } else if (abs % kHundred == 0) {
    StrAppendToSink(sink, minus, abs / kThousand, ".", (abs % kThousand) / kHundred, "s");
  } else {
    constexpr std::uint64_t kTen{10UL};
    StrAppendToSink(sink, minus, abs / kThousand, ".", (abs % kThousand) / kTen, "s");
  }
}

/// Appends a `duration` to `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink, AnyDuration duration) noexcept(IsNoexceptSink<Sink>()) {
  duration.InternalStringify(sink);
}

}  // namespace horus

#endif  // HORUS_STRINGS_CHRONO_H_
