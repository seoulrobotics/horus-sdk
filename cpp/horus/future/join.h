/// @file
///
/// Functions used to execute futures concurrently until all of them complete.

#ifndef HORUS_FUTURE_JOIN_H_
#define HORUS_FUTURE_JOIN_H_

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"
#include "horus/internal/tuple.h"
#include "horus/internal/unsafe_union.h"
#include "horus/types/span.h"

namespace horus {
namespace horus_internal {

/// Implementation of `JoinFuture<>` and `JoinArrayFuture<>`.
template <class I, class O, class Unions>
class JoinFutureImpl final {
  /// The size of the input/output containers.
  static constexpr std::size_t kSize{std::tuple_size<I>::value};

  /// Type of `done_`.
  using Bitset = std::bitset<kSize>;

 public:
  /// Constructs the object with the given `futures`.
  explicit JoinFutureImpl(I&& futures) noexcept;

  /// Futures cannot be copied.
  JoinFutureImpl(const JoinFutureImpl&) = delete;
  /// Futures cannot be copied.
  JoinFutureImpl& operator=(const JoinFutureImpl&) = delete;

  /// Move constructs the future from `other`.
  ///
  /// `other` will be left in an invalid state, and should no longer be used.
  JoinFutureImpl(JoinFutureImpl&& other) noexcept;

  /// Move-assigns the future from `other`.
  ///
  /// If `other` is not `this`, it will be left in an invalid state, and should no longer be used.
  JoinFutureImpl& operator=(JoinFutureImpl&& other) noexcept;

  /// Destroys the future.
  ~JoinFutureImpl();

  /// Implementation of `UnsafePoll()`.
  PollResult<O> UnsafePoll(PollContext& context);

 private:
  /// Transforms `futures_` into the resulting `std::tuple<>` or `std::array<>` assuming that all
  /// futures have completed.
  template <std::size_t... Indices>
  O GetResult(std::index_sequence<Indices...> /* indices */) noexcept {
    return O{
        std::move(*std::get<Indices>(futures_).template As<std::tuple_element_t<Indices, O>>())...};
  }

  /// A list of unions (either a `std::tuple<UnsafeUnion<>...>` or a `std::array<UnsafeUnion<>, _>`)
  /// whose active variant is determined by `done_`.
  ///
  /// Each union is either a `Future` or its `FutureResult`.
  Unions futures_;

  /// A bitset which stores whether stored `futures_` are done (in which case they are
  /// `FutureResult`s) or not (in which case they are `Future`s).
  Bitset done_;
};

}  // namespace horus_internal

// MARK: Join(Futures...)

/// Future returned by `Join(Futures&&...)`.
template <class... Futures>
class JoinFuture final : public Future<std::tuple<FutureResult<Futures>...>> {
 public:
  /// @copydoc Future::Result
  using typename JoinFuture::Future::Result;

  /// Constructs the future.
  explicit JoinFuture(Futures&&... futures, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : impl_{std::make_tuple(std::move(futures)...)}, info_{info} {}

  /// Polls the future until all the given futures have completed.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "Join", info_};
    return impl_.UnsafePoll(context);
  }

 private:
  /// Implementation of the future.
  horus_internal::JoinFutureImpl<
      /*I=*/std::tuple<Futures...>, /*O=*/std::tuple<FutureResult<Futures>...>,
      /*Unions=*/std::tuple<horus_internal::UnsafeUnion<Futures, FutureResult<Futures>>...>>
      impl_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a `Future` which will complete when all the given futures complete.
template <class... Futures, std::tuple<FutureResult<Futures>...>* = nullptr>
inline JoinFuture<Futures...> Join(Futures&&... futures) noexcept {
  return JoinFuture<Futures...>{std::forward<Futures>(futures)..., UnknownFutureInfo()};
}

// MARK: Join(std::array<F, N>)

/// Future returned by `Join(std::array<F, N>&&)`.
template <class F, std::size_t N>
class JoinArrayFuture final : public Future<std::array<FutureResult<F>, N>> {
 public:
  /// Constructs the future.
  explicit JoinArrayFuture(std::array<F, N>&& futures,
                           const FutureInfo& info = CurrentFutureInfo()) noexcept
      : impl_{std::move(futures)}, info_{info} {}

  /// Polls the future until all the given futures have completed.
  PollResult<typename JoinArrayFuture::Result> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "Join", info_};
    return impl_.UnsafePoll(context);
  }

 private:
  /// Implementation of the future.
  horus_internal::JoinFutureImpl<
      /*I=*/std::array<F, N>, /*O=*/std::array<FutureResult<F>, N>,
      /*Unions=*/std::array<horus_internal::UnsafeUnion<F, FutureResult<F>>, N>>
      impl_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a `Future` which will complete when all the given futures complete.
template <class F, std::size_t N>
inline JoinArrayFuture<F, N> Join(std::array<F, N>&& futures,
                                  const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return JoinArrayFuture<F, N>{std::move(futures), info};
}

// MARK: Join(Span<F>)

/// Default chunk size in `JoinSpanFuture`.
static constexpr std::size_t kJoinDefaultChunkSize{32};

/// Future returned by `Join(Vector)`.
template <class F, std::size_t ChunkSize = kJoinDefaultChunkSize>
class JoinSpanFuture final : public Future<void> {
 public:
  static_assert(std::is_void<FutureResult<F>>::value,
                "Join(Vector) may only be called with void Futures");

  /// Constructs the future.
  explicit JoinSpanFuture(Span<F> futures, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : futures_{futures}, info_{info} {}

  /// Polls the future until all the given futures have completed.
  PollResult<void> UnsafePoll(PollContext& context) final;

  /// Returns the span referring to the futures to drive to completion.
  constexpr Span<F> AllFutures() noexcept { return futures_; }

 private:
  /// The futures to bring to completion. If `futures_.size() > ChunkSize`, all futures up to
  /// `ChunkSize` will be brought to completion, then `done_` will be cleared, `futures_` advanced
  /// by `ChunkSize`, and `UnsafePoll()` will continue.
  Span<F> futures_;

  /// A bitset which stores whether futures in `futures_` have completed.
  std::bitset<ChunkSize> done_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when all the given futures complete.
///
/// Because the number of futures is not known ahead of time and some bookkeeping is necessary,
/// futures in the given span will be processed in chunks. For instance, if `futures` has 100
/// futures, the first 64 will be brought to completion, and the next 36 will be started after
/// that.
template <std::size_t ChunkSize = kJoinDefaultChunkSize, class F>
inline JoinSpanFuture<F, ChunkSize> Join(Span<F> futures,
                                         const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return JoinSpanFuture<F, ChunkSize>{futures, info};
}

// MARK: Join(Vector)

/// Future returned by `Join(Vector)`.
template <class Vector, std::size_t ChunkSize = kJoinDefaultChunkSize,
          bool IsVoid = std::is_void<FutureResult<typename Vector::value_type>>::value>
class JoinVectorFuture;

template <class Vector, std::size_t ChunkSize>
class JoinVectorFuture<Vector, ChunkSize, /*IsVoid=*/true> final : public Future<void> {
 public:
  /// Constructs the future.
  explicit JoinVectorFuture(Vector&& futures) noexcept : futures_{std::move(futures)} {}

  /// Polls the future until all the given futures have completed.
  PollResult<void> UnsafePoll(PollContext& context) final;

 private:
  /// The futures to bring to completion.
  Vector futures_;

  /// The future which actually performs the computation.
  ///
  /// This is kept empty until `UnsafePoll()` is called; see `UnsafePoll()` comment for reasoning.
  JoinSpanFuture<typename Vector::value_type, ChunkSize> inner_{};
};

template <class Vector, std::size_t ChunkSize>
class JoinVectorFuture<Vector, ChunkSize, /*IsVoid=*/false> final
    : public Future<std::vector<FutureResult<typename Vector::value_type>>> {
 public:
  /// @copydoc Future::Result
  using typename JoinVectorFuture::Future::Result;

  /// Type of each item of the result.
  using T = typename Result::value_type;

  static_assert(std::is_default_constructible<Result>::value,
                "Join(Vector&&) results must be default-constructible");

  /// Constructs the future.
  explicit JoinVectorFuture(Vector&& futures, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : futures_{std::move(futures)}, info_{info} {}

  /// Polls the future until all the given futures have completed.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The futures to bring to completion.
  Vector futures_;

  /// The results of the futures.
  std::vector<T> results_;

  /// The offset in `futures_` and `results_` where the current chunk begins.
  std::size_t offset_{0};

  /// A bitset which stores whether futures in `futures_` have completed.
  std::bitset<ChunkSize> done_;

  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when all the given futures complete.
///
/// Because the number of futures is not known ahead of time and some bookkeeping is necessary,
/// futures in the given span will be processed in chunks. For instance, if `futures` has 100
/// futures, the first 64 will be brought to completion, and the next 36 will be started after
/// that.
template <std::size_t ChunkSize = kJoinDefaultChunkSize, class Vector,
          typename Vector::value_type* = nullptr>
inline JoinVectorFuture<Vector, ChunkSize> Join(
    Vector&& futures, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return JoinVectorFuture<Vector, ChunkSize>{std::forward<Vector>(futures), info};
}

// MARK: Function definitions

namespace horus_internal {

template <class I, class O, class Unions>
JoinFutureImpl<I, O, Unions>::JoinFutureImpl(I&& futures) noexcept {
  I owned_futures{std::move(futures)};
  ForEachIndexTo<kSize>([this, &owned_futures](auto index) noexcept {
    constexpr std::size_t kIndex{decltype(index)::value};

    using Future = std::tuple_element_t<kIndex, I>;

    new (std::get<kIndex>(futures_).template As<Future>())
        Future(std::move(std::get<kIndex>(owned_futures)));
  });
}

template <class I, class O, class Unions>
JoinFutureImpl<I, O, Unions>::JoinFutureImpl(JoinFutureImpl&& other) noexcept {
  ForEachIndexTo<kSize>([this, &other](auto index) noexcept {
    constexpr std::size_t kIndex{decltype(index)::value};

    using Future = std::tuple_element_t<kIndex, I>;
    using FutureResult = std::tuple_element_t<kIndex, O>;

    UnsafeUnion<Future, FutureResult>& self_union{std::get<kIndex>(futures_)};
    UnsafeUnion<Future, FutureResult>& other_union{std::get<kIndex>(other.futures_)};

    if (other.done_[kIndex]) {
      new (self_union.template As<FutureResult>())
          FutureResult(std::move(*other_union.template As<FutureResult>()));
      done_[kIndex] = true;
    } else {
      new (self_union.template As<Future>()) Future(std::move(*other_union.template As<Future>()));
    }
  });
}

template <class I, class O, class Unions>
JoinFutureImpl<I, O, Unions>& JoinFutureImpl<I, O, Unions>::operator=(
    JoinFutureImpl&& other) noexcept {
  if (this != &other) {
    ~JoinFutureImpl();
    new (this) JoinFutureImpl(std::move(other));
  }
  return *this;
}

template <class I, class O, class Unions>
JoinFutureImpl<I, O, Unions>::~JoinFutureImpl() {
  ForEachIndexTo<kSize>([this](auto index) noexcept {
    constexpr std::size_t kIndex{decltype(index)::value};

    using Future = std::tuple_element_t<kIndex, I>;
    using FutureResult = std::tuple_element_t<kIndex, O>;

    UnsafeUnion<Future, FutureResult>& self_union{std::get<kIndex>(futures_)};

    if (done_[kIndex]) {
      self_union.template As<FutureResult>()->~FutureResult();
    } else {
      self_union.template As<Future>()->~Future();
    }
  });
}

template <class I, class O, class Unions>
PollResult<O> JoinFutureImpl<I, O, Unions>::UnsafePoll(PollContext& context) {
  ForEachIndexTo<kSize>([this, &context](auto index) noexcept(false) {
    constexpr std::size_t kIndex{decltype(index)::value};

    using Future = std::tuple_element_t<kIndex, I>;
    using FutureResult = std::tuple_element_t<kIndex, O>;

    if (done_[kIndex]) {
      return;
    }

    UnsafeUnion<Future, FutureResult>& self_union{std::get<kIndex>(futures_)};

    Future& future{*self_union.template As<Future>()};
    PollResult<FutureResult> maybe_result{future.UnsafePoll(context)};
    FutureResult* result{maybe_result.template TryAs<FutureResult>()};
    if (result == nullptr) {
      return;
    }

    future.~Future();
    new (self_union.template As<FutureResult>()) FutureResult(std::move(*result));
    done_[kIndex] = true;
  });
  if (!done_.all()) {
    return Pending{};
  }
  return GetResult(std::make_index_sequence<kSize>{});
}

/// Polls all futures in `futures` in chunks, writing the state of each future to `done`.
///
/// `is_done(poll_result, index_in_chunk)` will be called for each pending future. When all futures
/// in a chunk have completed, `futures` and `done` will be updated for the next chunk and
/// `on_next_chunk(skip)` will be called with the number of items which were processed in the
/// current chunk.
template <class F, std::size_t ChunkSize, class IsDone, class OnNextChunk>
void PollChunks(Span<F>& futures, std::bitset<ChunkSize>& done, PollContext& context,
                const IsDone& is_done, const OnNextChunk& on_next_chunk) {
  while (!futures.empty()) {
    std::size_t const chunk_size{std::min(futures.size(), ChunkSize)};
    bool all_done{true};
    for (std::size_t i = 0; i < chunk_size; ++i) {
      if (done[i]) {
        continue;
      }
      if (is_done(futures[i].UnsafePoll(context), i)) {
        done[i] = true;
      } else {
        all_done = false;
      }
    }
    if (!all_done) {
      return;
    }
    futures = futures.subspan(chunk_size);
    done.reset();
    on_next_chunk(chunk_size);
  }
}

}  // namespace horus_internal

template <class F, std::size_t ChunkSize>
PollResult<void> JoinSpanFuture<F, ChunkSize>::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "Join", info_};

  horus_internal::PollChunks(
      futures_, done_, context,
      [](const PollResult<void>& result, std::size_t /* index_in_chunk */) noexcept -> bool {
        return !result.Is<Pending>();
      },
      [](std::size_t /* skip */) noexcept {});
  if (!futures_.empty()) {
    return PendingResult<void>();
  }
  return ReadyResult<void>();
}

template <class Vector, std::size_t ChunkSize>
PollResult<void> JoinVectorFuture<Vector, ChunkSize, /*IsVoid=*/true>::UnsafePoll(
    PollContext& context) {
  // We don't trace this call since it trivially wraps the `JoinSpan`.
  if (inner_.AllFutures().empty()) {
    // We're being polled for the first time, so we must initialize the span.
    //
    // This is done here rather than in the constructor because until polled the `JoinVectorFuture`
    // might be moved around. For a traditional `std::vector` this isn't a problem because items
    // will stay in place when the vector is moved, but for e.g. an `absl::InlinedVector` this could
    // cause items to be moved.
    inner_ = Join(Span<typename Vector::value_type>{futures_});
  }
  return PollFuture(inner_, context);
}

template <class Vector, std::size_t ChunkSize>
auto JoinVectorFuture<Vector, ChunkSize, /*IsVoid=*/false>::UnsafePoll(PollContext& context)
    -> PollResult<Result> {
  const PollContext::Trace trace{context, "Join", info_};

  if (results_.empty()) {
    results_.resize(futures_.size());
  }
  Span<typename Vector::value_type> futures{futures_};
  futures = futures.subspan(offset_);

  horus_internal::PollChunks(
      futures, done_, context,
      [this](PollResult<T>&& result, std::size_t index_in_chunk) noexcept -> bool {
        if (result.template Is<Pending>()) {
          static_cast<void>(PollResult<T>{std::move(result)});
          return false;
        }
        results_[offset_ + index_in_chunk] = std::move(result).template As<T>();
        return true;
      },
      [this](std::size_t skip) { offset_ += skip; });

  if (!futures.empty()) {
    return PendingResult<Result>();
  }
  return ReadyResult<Result>(std::move(results_));
}

}  // namespace horus

#endif  // HORUS_FUTURE_JOIN_H_
