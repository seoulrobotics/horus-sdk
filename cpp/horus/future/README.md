# Horus futures

Horus futures are the mechanism for structured concurrency in Horus. They are
heavily inspired by the Rust
[`Future<T>`](https://doc.rust-lang.org/stable/std/future/trait.Future.html)
trait, with some differences [^rust-differences].

[^rust-differences]: notably, `PollContext` is specialized for use with the
Horus event loop, rather than generic for integration with any async runtime.

Horus futures are designed to deal with several restrictions:

- C++14 compatibility (where `co_await` is unavailable).

- Autosar compliance (which forbids usage of macros).

- Handling of memory exhaustion.

Note that futures run concurrently, but are bound to a single thread (and more
specifically to a single [`EventLoop`](../event_loop/README.md)).

## Composition

Horus futures are intended to compose using templates; for instance, a function
`Join()` which takes two futures and awaits them concurrently does not accept
`Future<T>` classes; instead, it is templated on the types of each input future,
and returns a future generic on them both:

```cpp
template <class F1, class F2>
JoinFuture<F1, F2> Join(F1&&, F2&&) noexcept;
```

This approach means that most futures can be constructed with no allocation, and
that functions such as `Join()` above are guaranteed not to fail. The
`AnyFuture<T>` class can be used to type-erase futures, similarly to how
`std::function<F>` is used to type-erase closure types.

## Piping

Many of the helpers listed below expose a "pipe operator" version which works
with `operator|(Future&&, Operator&&)`. For instance, `Map()` has two versions:

- The "direct" version:

  ```cpp
  template <class F, class M>
  MapFuture<F, M> Map(F&& future, M&& map) noexcept;

  auto bar = Map(Foo(), [](std::int32_t foo) -> std::int32_t { return foo + 1; });
  ```

- The "piping" version:

  ```cpp
  template <class M>
  FutureOperator<MapFuture, void, F> Map(M&& map) noexcept;

  auto bar = Foo() | Map([](std::int32_t foo) -> std::int32_t { return foo + 1; });
  ```

## Construction

Futures can be constructed using the following classes and functions:

- [`Async()`](async.h) executes high-level code with a syntax similar to
  `co_await` (which blocks the caller thread).

- [`AwaitSignal()`](signal.h) creates a future which completes when a
  [signal](https://en.cppreference.com/w/cpp/utility/program/signal) is
  received.

- [`Channel<T>`](channel.h) is a MPSC channel which can send data from any
  thread to a running future.

- [`CompleteAt() and CompleteIn()`](time.h) create futures which return at a
  given point in time.

- [`CompletedFuture<T>`](completed.h) is a future which immediately completes
  with a given value or fails with a given exception.

- [`DeferWork()`](work.h) is used to execute blocking or CPU-bound work in a
  worker thread.

- [`FromContinuation()`](from_continuation.h) creates a future which completes
  when a function is called.

- [`FromPoll()`](from_poll.h) creates a future with a custom polling logic.

- [`RejectedFuture<T>`](rejected.h) is a future which immediately fails with a
  given exception.

- [`ResolveWith()`](resolved.h) creates a future which immediately completes
  with a given value.

## Operators

The following helpers can be used to manipulate and compose futures:

- [`AnyFuture<T>`](any.h) is used to type-erase any `Future<T>` (at the cost of
  a possible allocation).

- [`Attach()`](attach.h) is used to attach state whose lifetime should be bound
  to a future.

- [`ByRef()`](by_ref.h) is used to poll a future by reference, rather than
  transferring ownership.

- [`CancelAt()` and `CancelIn()`](cancel.h) are used to execute futures until
  some deadline is reached.

- [`Loop()`](loop.h) is used to continuously create and execute futures until
  they break.

- [`Map()`](map.h) is used to map the output of a future using a function.

- [`MapTo()`](map_to.h) is used to ignore the output of a future, instead
  returning a given value.

- [`Race()`](race.h) is used to execute multiple futures concurrently, returning
  the output of the first one which completes.

- [`Then()`](then.h) is used to map the output of a future to a new future, then
  awaiting its completion.

- [`Try() and Catch()`](try.h) are used to catch exceptions in futures, possibly
  handling or rethrowing them. They can also be used to execute a function on
  future completion, no matter whether it failed or succeeded.

## Debugging

Futures, no matter their implementation, are notoriously hard to debug. Horus
futures were designed with this in mind, and offer features and helpers that
alleviate this problem:

- The use of polling for futures mean that trees of futures execute from top to
  bottom, with parent futures directly polling children futures, thus keeping a
  fairly linear execution.

- Futures trace their execution, and a function `horus::PrintFutureStackTrace()`
  is exposed to print the current future stack trace.
