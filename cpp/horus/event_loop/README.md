# Horus event loop

The Horus event loop is used to execute [`Future`](../future/README.md)s
concurrently on a single thread. It is currently backed by
[libuv](https://libuv.org), though that may change in the future.

## File organization

- `event_loop.h` defines the `EventLoop` class.

- `handle.h` defines the `Handle<H>` class (with `H` a libuv
  [handle type](https://docs.libuv.org/en/v1.x/handle.html)), used by `Future`s
  to refer to the event loop and schedule themselves to be polled when external
  events occur.

- `uv.h` defines helpers and forward references to libuv's `uv.h`.

  - `uv_handles_generator.cpp` generates `uv_handles.h`, which defines the
    layout of libuv handle types needed by `uv.h`.

- `waker.h` defines the `FutureWaker` class, used by `Future`s to request to be
  polled.
