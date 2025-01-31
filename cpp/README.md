# Horus C++ SDK

For installation instructions, see [main README](../README.md).

## File organization

- `CMakeLists.txt`: CMake definition of `horus::core`, `horus::sdk` and all
  tests.

- `cmake/`: helpers used by CMake.

- `examples/`: examples which use the Horus SDK.

- `horus/`: headers, source files, and tests for `horus::core` and `horus::sdk`.

  - `sdk.h`: the main entry point of the Horus SDK.

  - [`event_loop/`](horus/event_loop/README.md): the (internal) `EventLoop`
    class and related types.

  - `functional/`: functional helpers.

  - [`future/`](horus/future/README.md): the `Future` class and several
    implementations of it.

  - `internal/`: internal miscellaneous helpers.

  - `logs/`: log formatting helpers.

  - [`pb/`](horus/pb/README.md): common Protobuf types.

  - `rpc/`: RPC helpers.

  - `sdk/`: types used by the `horus::Sdk` class.

  - `strings/`: helpers for manipulating strings, formatting values, and
    logging.

  - `testing/`: helpers used in tests.

  - `types/`: general-purpose types used throughout `horus::core`.

## Linting

[clang-tidy](https://clang.llvm.org/extra/clang-tidy/) should be used for
linting:

```sh
$ (cd .. && run-clang-tidy -config-file cpp/.clang-tidy -p build/current -j$(nproc) cpp/horus)
```

Format can also be checked with
[clang-format](https://clang.llvm.org/docs/ClangFormat.html):

```sh
$ clang-format -Werror --dry-run $(find ./ \( -name '*.h' -or -name '*.cpp' \) -not -path '*/build/*')
```

## Testing

Tests can be run using:

```sh
$ cmake --build ../build/current --target horus_core_test horus_sdk_test \
  && (cd ../build/current && ./horus_core_test && ./horus_sdk_test)
```
