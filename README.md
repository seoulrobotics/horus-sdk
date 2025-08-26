# Horus SDK

This repository contains the source code for the Horus SDK, which provides
access to Horus services.

The SDK is available in two languages:

1. C++14, with (planned) Autosar compliance.
2. Python 3.8.

## Dependencies

Building the SDK requires the following dependencies to be installed:

- `clang` >=10 or `gcc` >=9.4
  - Development happens with `clang` >=18 as it provides more checks.
- `cmake` >=3.20

## C++

The source code of the C++ SDK lives in `cpp`, and is built using CMake. Clients
should primarily use the `horus::Sdk` class defined in `cpp/horus/sdk.h`

### Using the SDK

1. Add a CMake dependency to Horus. For instance, the code below uses
   [`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html)
   to import the Horus project from GitHub:

   ```cmake
   include(FetchContent)

   FetchContent_Declare(horus
     GIT_REPOSITORY https://github.com/seoulrobotics/horus-sdk.git
     GIT_TAG        ...
     SOURCE_SUBDIR cpp
   )
   FetchContent_MakeAvailable(horus)
   ```

   The `horus::sdk` target can then be used:

   ```cmake
   target_link_libraries(${target} horus::sdk)
   ```

2. Include `horus/sdk.h` to use the `horus::Sdk` class:

   ```cpp
   #include "horus/sdk.h"

   horus::Sdk sdk;
   ```

   For usage documentation, see documentation comments on the `horus::Sdk` class
   and all the classes it refers to.

Examples for using the SDK exist in [`cpp/examples`](cpp/examples).

> [!NOTE]
>
> The SDK includes many implementation details not part of the `horus::sdk`
> public API, and these details may change at any time.
>
> Generally, APIs defined in `horus/sdk.h` and `horus/sdk/*.h`, as well as APIs
> indirectly exposed by these headers (e.g. `pb::Log` in
> `horus::sdk::LogSubscriptionRequest`) are stable. All other APIs (including
> external dependencies) are considered implementation details, and may change
> without notice.

### Installing the SDK

Instead of using `FetchContent_Declare()`, the SDK may be installed with CMake.

First, it must be built and installed with CMake:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D HORUS_SDK_BUILD_EXAMPLES=OFF -D HORUS_SDK_BUILD_TESTING=OFF
cmake --build build --target horus_sdk
cmake --install build --prefix install
```

It can then be loaded in CMake with (assuming that the install prefix is in the
`CMAKE_PREFIX_PATH`):

```cmake
find_package(horus_sdk CONFIG REQUIRED)
```

And used with `horus::sdk`.

## Python

The source code of the Python SDK lives in `py`, and uses `asyncio`. See
[`py/README.md`](py/README.md) for more information.

```python
import asyncio
import horus

async def print_logs():
    sdk = horus.Sdk()

    async with sdk.subscribe_to_logs(on_log_message=print):
        ...
```

See [`py/examples/print_logs.py`](py/examples/print_logs.py) for an example.
