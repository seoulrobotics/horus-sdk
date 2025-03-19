# CMake project setup
# -----

cmake_minimum_required(VERSION 3.16)

project(horus_sdk
  VERSION 0.0.1
  LANGUAGES C CXX
)

set(HORUS_SDK_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(HORUS_SDK_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
if(CMAKE_VERSION VERSION_LESS "3.20.0")
  set(HORUS_SDK_ROOT_DIR "${HORUS_SDK_SOURCE_DIR}/..")
else()
  cmake_path(GET HORUS_SDK_SOURCE_DIR PARENT_PATH HORUS_SDK_ROOT_DIR)
endif()

set(HORUS_SDK_IS_ROOT_PROJECT OFF)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(HORUS_SDK_IS_ROOT_PROJECT ON)

  # Generate `compile_commands.json`.
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # Make std headers available in `compile_commands.json`.
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()


# Options
# -----

option(HORUS_SDK_BUILD_EXAMPLES "Enable Horus SDK examples" ${HORUS_SDK_IS_ROOT_PROJECT})
option(HORUS_SDK_BUILD_TESTING "Enable Horus SDK tests" ${HORUS_SDK_IS_ROOT_PROJECT})
option(HORUS_SDK_COVERAGE "Compile with coverage information" OFF)
option(HORUS_SDK_ASAN "Enable address sanitizer" OFF)
option(HORUS_SDK_TSAN "Enable thread sanitizer" OFF)
option(HORUS_SDK_UBSAN "Enable undefined behavior sanitizer" OFF)
option(HORUS_SDK_USE_LIBCXX "Enable usage of libc++ (and therefore thread safety analysis and hardening)" OFF)
option(HORUS_SDK_USE_PB_NAMESPACE_ALIAS "Enable horus::pb = horus::sdk::pb alias" ON)
option(HORUS_SDK_USE_UNION "Enable usage of unions for better debugging" OFF)

set(HORUS_SDK_BUILD_SYMLINK "" CACHE FILEPATH "If not empty, create a symlink from the given path to the build directory")

if(HORUS_SDK_BUILD_SYMLINK)
  execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_BINARY_DIR}" "${HORUS_SDK_BUILD_SYMLINK}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND "[[clang::lifetimebound]]")
  set(HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS "[[no_unique_address]]")
  set(HORUS_SDK_ATTRIBUTE_TRIVIAL_ABI "[[clang::trivial_abi]]")
  set(HORUS_SDK_NO_SANITIZE "__attribute__((no_sanitize(x)))")
  set(HORUS_SDK_USE_CLANG ON)

  if(HORUS_SDK_USE_LIBCXX)
    set(HORUS_SDK_THREAD_ANNOTATION_ATTRIBUTE "__attribute__((x))")
  endif()
endif()

configure_file(
  horus/internal/attributes.h.in
  horus/internal/attributes.h
)


# Third party dependencies
# -----

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)  # Earlier `set()` takes precedence over later `option()`.
endif()
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)  # Set URL timestamps to time of extraction.
endif()

include(FetchContent)

set(horus_external_targets)

if(HORUS_SDK_BUILD_TESTING)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip  # v1.14
    EXCLUDE_FROM_ALL
  )

  FetchContent_MakeAvailable(googletest)
endif()

FetchContent_Declare(
  libuv
  URL https://github.com/libuv/libuv/archive/e9f29cb984231524e3931aa0ae2c5dae1a32884e.zip  # v1.48.0
  EXCLUDE_FROM_ALL
)
set(ASAN               ${HORUS_SDK_ASAN})
set(TSAN               ${HORUS_SDK_TSAN})
set(UBSAN              ${HORUS_SDK_UBSAN})
set(LIBUV_BUILD_SHARED OFF)
set(LIBUV_BUILD_TESTS  OFF)

FetchContent_Declare(
  IXWebSocket
  URL https://github.com/machinezone/IXWebSocket/archive/c5a02f1066fb0fde48f80f51178429a27f689a39.zip  # v11.4.5
  EXCLUDE_FROM_ALL
)
set(USE_ZLIB OFF)

FetchContent_Declare(
  protozero
  URL       https://github.com/mapbox/protozero/archive/542fcf7dd228672b775eb283d18ccd94c4e682d9.zip  # v1.7.1
  PATCH_COMMAND
    # Prevent protozero from defining targets `clang-tidy`.
    sed -Ei "s/find_program\\\\(([A-Z_]+) .+\\\\)/set(\\\\1 0)/g" CMakeLists.txt &&
    # Disable testing in Protozero only.
    sed -i "s/if(BUILD_TESTING)/if(0)/g" CMakeLists.txt &&
    # Expose `basic_pbf_writer` internals to be able to serialize buffers in-place.
    sed -i "s/^class basic_pbf_writer {$/class basic_pbf_writer { public:/g" include/protozero/basic_pbf_writer.hpp &&
    # Don't import Protobuf.
    sed -i "s/find_package(Protobuf)//" CMakeLists.txt
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(IXWebSocket libuv protozero)

list(APPEND horus_external_targets ixwebsocket uv_a)

# UV handle information.
add_executable(horus_uv_handles_generator horus/event_loop/uv_handles_generator.cpp)
add_dependencies(horus_uv_handles_generator uv_a)
target_include_directories(horus_uv_handles_generator SYSTEM PUBLIC $<TARGET_PROPERTY:uv_a,INTERFACE_INCLUDE_DIRECTORIES>)

add_custom_command(
  OUTPUT
    horus/event_loop/uv_handles.h
  COMMAND
    "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/horus/event_loop"
  COMMAND
    horus_uv_handles_generator "${CMAKE_CURRENT_BINARY_DIR}/horus/event_loop/uv_handles.h"
  DEPENDS
    horus_uv_handles_generator
)


# Library
# -----

set(horus_targets horus_core horus_sdk)

add_library(horus_core
  horus/event_loop/event_loop.cpp
  horus/event_loop/event_loop.h
  horus/event_loop/handle.h
  horus/event_loop/uv.cpp
  horus/event_loop/uv.h
  horus/event_loop/waker.cpp
  horus/event_loop/waker.h
  horus/functional/move_only_function.h
  horus/future/future.h
  horus/future/any.h
  horus/future/async.h
  horus/future/attach.h
  horus/future/by_ref.h
  horus/future/cancel.h
  horus/future/channel.h
  horus/future/completed.h
  horus/future/core_owner.cpp
  horus/future/core_owner.h
  horus/future/from_continuation.h
  horus/future/from_poll.h
  horus/future/join.h
  horus/future/loop.h
  horus/future/map.h
  horus/future/map_to.h
  horus/future/one_of.h
  horus/future/operator.h
  horus/future/owner.h
  horus/future/pipe.h
  horus/future/poll.cpp
  horus/future/poll.h
  horus/future/race.h
  horus/future/rejected.h
  horus/future/resolved.h
  horus/future/run.h
  horus/future/shared.h
  horus/future/signal.cpp
  horus/future/signal.h
  horus/future/then.h
  horus/future/time.cpp
  horus/future/time.h
  horus/future/try.h
  horus/future/work.h
  horus/internal/bitset.h
  horus/internal/enum.cpp
  horus/internal/enum.h
  horus/internal/pointer_arithmetic.h
  horus/internal/pointer_cast.h
  horus/internal/source_location.h
  horus/internal/tuple.h
  horus/internal/type_traits.h
  horus/internal/unsafe_union.h
  horus/internal/void.h
  horus/pb/alias.h
  horus/pb/buffer.h
  horus/pb/cow_bytes.cpp
  horus/pb/cow_bytes.h
  horus/pb/cow_repeated.h
  horus/pb/cow_span.h
  horus/pb/cow.h
  horus/pb/message.cpp
  horus/pb/message.h
  horus/pb/repeated_iterator.h
  horus/pb/serialize.cpp
  horus/pb/serialize.h
  horus/pb/types.h
  horus/pb/unaligned_span.h
  horus/rpc/base_client.cpp
  horus/rpc/base_client.h
  horus/rpc/base_handler.cpp
  horus/rpc/base_handler.h
  horus/rpc/endpoint.cpp
  horus/rpc/endpoint.h
  horus/rpc/internal/subscriber_set.h
  horus/rpc/retry_policy.h
  horus/rpc/ws.cpp
  horus/rpc/ws.h
  horus/strings/ansi.cpp
  horus/strings/ansi.h
  horus/strings/chrono.h
  horus/strings/fallback.h
  horus/strings/logging.cpp
  horus/strings/logging.h
  horus/strings/pad.h
  horus/strings/stdio.h
  horus/strings/str_cat.h
  horus/strings/str_sink.cpp
  horus/strings/str_sink.h
  horus/strings/str_sink_erased.h
  horus/types/in_place.h
  horus/types/list.h
  horus/types/one_of.h
  horus/types/owned.h
  horus/types/span.h
  horus/types/string_view.cpp
  horus/types/string_view.h

  "${CMAKE_CURRENT_BINARY_DIR}/horus/event_loop/uv_handles.h"
  "${CMAKE_CURRENT_BINARY_DIR}/horus/internal/attributes.h"
)
add_library(horus::core ALIAS horus_core)
target_compile_features(horus_core PUBLIC cxx_std_14)
target_include_directories(horus_core PUBLIC "${HORUS_SDK_SOURCE_DIR}" "${HORUS_SDK_BINARY_DIR}")

target_include_directories(horus_core SYSTEM PUBLIC
  "${ixwebsocket_SOURCE_DIR}"
  "${protozero_SOURCE_DIR}/include"
  $<TARGET_PROPERTY:uv_a,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(horus_core PUBLIC ixwebsocket::ixwebsocket uv_a)

if(HORUS_SDK_USE_UNION)
  target_sources(horus_core PRIVATE horus/internal/unsafe_union_union.h)
else()
  target_sources(horus_core PRIVATE horus/internal/unsafe_union_aligned_bytes.h)
endif()

add_library(horus_sdk
  horus/logs/format.cpp
  horus/logs/format.h
  horus/rpc/services.h
  horus/sdk.cpp
  horus/sdk.h
  horus/sdk/errors.h
  horus/sdk/health.cpp
  horus/sdk/health.h
  horus/sdk/logs.cpp
  horus/sdk/logs.h
  horus/sdk/objects.h
  horus/sdk/point_clouds.h
  horus/sdk/profiling.cpp
  horus/sdk/profiling.h
  horus/sdk/sensor.h
)
add_library(horus::sdk ALIAS horus_sdk)
target_link_libraries(horus_sdk PUBLIC horus::core)

include(cmake/HorusSdkProtobuf.cmake)
target_sources(horus_core PRIVATE ${horus_core_pb_sources})


# Tests
# -----

if(HORUS_SDK_BUILD_TESTING)
  include(CTest)
  enable_testing()

  add_executable(horus_core_test
    horus/event_loop/event_loop_test.cpp
    horus/future/async_test.cpp
    horus/future/channel_test.cpp
    horus/future/from_continuation_test.cpp
    horus/future/join_test.cpp
    horus/future/loop_test.cpp
    horus/future/race_test.cpp
    horus/future/shared_test.cpp
    horus/future/then_test.cpp
    horus/future/time_test.cpp
    horus/future/try_test.cpp
    horus/future/work_test.cpp
    horus/pb/cow_bytes_test.cpp
    horus/pb/cow_repeated_test.cpp
    horus/pb/cow_span_test.cpp
    horus/pb/cow_test.cpp
    horus/pb/message_test.cpp
    horus/pb/serialize_test.cpp
    horus/rpc/ws_test.cpp
    horus/strings/str_cat_test.cpp
    horus/testing/event_loop.h
    horus/testing/event_loop_test.cpp
    horus/testing/timing.h
    horus/testing/ws_server.cpp
    horus/testing/ws_server.h
  )
  target_compile_definitions(horus_core_test PRIVATE "HORUS_SDK_ROOT_DIR=\"${HORUS_SDK_ROOT_DIR}\"")
  target_link_libraries(horus_core_test PRIVATE horus::core GTest::gmock_main)

  add_executable(horus_sdk_test
    horus/sdk_test.cpp
    horus/testing/ws_server.cpp
    horus/testing/ws_server.h
  )
  target_link_libraries(horus_sdk_test PRIVATE horus::core horus::sdk GTest::gmock_main)

  list(APPEND horus_targets horus_core_test horus_sdk_test)

  include(GoogleTest)
  gtest_discover_tests(horus_core_test)
  gtest_discover_tests(horus_sdk_test)
endif()

if(HORUS_SDK_BUILD_EXAMPLES)
  set(horus_examples_targets)

  foreach(example print_logs print_events print_health_status print_occupancy_map)
    add_executable(horus_example_${example}
      examples/${example}.cpp
      examples/helpers.cpp
      examples/helpers.h
    )

    target_link_libraries(horus_example_${example} PRIVATE horus::core horus::sdk)
    list(APPEND horus_targets horus_example_${example})
    list(APPEND horus_examples_targets horus_example_${example})
  endforeach()

  add_custom_target(horus_sdk_examples
    DEPENDS ${horus_examples_targets}
  )
endif()


# C++ options
# -----

foreach(horus_target IN LISTS horus_targets)
  target_compile_options(${horus_target} PRIVATE
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -Wconversion
    -Wimplicit-fallthrough
    -Wswitch-default
    -Wswitch-enum
    -Wfloat-equal
    -Wno-unused-parameter
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wdouble-promotion
    -Wformat=2
  )
  set_target_properties(${horus_target} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  if(HORUS_SDK_USE_CLANG AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "16.0.0")
    # Additional checks.
    target_compile_options(${horus_target} PRIVATE
      -Wdocumentation
      -Wdocumentation-pedantic
      -Wshadow-all
      -Wunused-member-function
    )

    # Thread safety annotations.
    if(HORUS_SDK_USE_LIBCXX)
      target_compile_options(${horus_target} PRIVATE
        -Wthread-safety
        -Wthread-safety-verbose
      )
    endif()
  endif()

  if(HORUS_SDK_COVERAGE)
    target_compile_options(${horus_target} PUBLIC -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(${horus_target} PUBLIC -fprofile-instr-generate)
  endif()
endforeach()

set(_horus_sdk_sanitizers)

if(HORUS_SDK_ASAN)
  list(APPEND _horus_sdk_sanitizers address)
endif()
if(HORUS_SDK_TSAN)
  if(HORUS_SDK_COVERAGE)
    message(SEND_ERROR "TSan cannot run with coverage due to false positives")
  endif()
  if(HORUS_SDK_ASAN)
    message(SEND_ERROR "TSan cannot run with ASan")
  endif()
  if(HORUS_SDK_UBSAN)
    message(SEND_ERROR "TSan cannot run with UBSan due to false positives (https://github.com/google/sanitizers/issues/1106)")
  endif()
  list(APPEND _horus_sdk_sanitizers thread)
endif()
if(HORUS_SDK_UBSAN)
  list(APPEND _horus_sdk_sanitizers undefined)
endif()

string(REPLACE ";" "," _horus_sdk_sanitizers "${_horus_sdk_sanitizers}")

foreach(target IN LISTS horus_targets horus_external_targets)
  if(_horus_sdk_sanitizers)
    target_compile_options(${target} PRIVATE -fsanitize=${_horus_sdk_sanitizers})
    target_link_options(${target} PUBLIC -fsanitize=${_horus_sdk_sanitizers})

    if(HORUS_SDK_USE_CLANG)
      set(_horus_sdk_sanitizer_ignorelist "${HORUS_SDK_SOURCE_DIR}/data/sanitizer_ignorelist.txt")
      target_compile_options(${target} PRIVATE "-fsanitize-ignorelist=${_horus_sdk_sanitizer_ignorelist}")
      target_link_options(${target} PUBLIC "-fsanitize-ignorelist=${_horus_sdk_sanitizer_ignorelist}")
    endif()
  endif()

  set_property(TARGET ${target} PROPERTY POSITION_INDEPENDENT_CODE ON)

  if(HORUS_SDK_USE_LIBCXX)
    target_compile_definitions(${target} PUBLIC
      _LIBCPP_HAS_THREAD_SAFETY_ANNOTATIONS=1
      _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE
    )

    set(_kw PUBLIC)
    if(target STREQUAL uv_a)
      set(_kw)
    endif()

    target_compile_options(${target} PUBLIC -fno-strict-aliasing $<$<CONFIG:DEBUG>:-Og> $<$<NOT:$<CONFIG:RELEASE>>:-g>)
    target_link_options(${target} PUBLIC -stdlib=libc++)
    target_link_libraries(${target} ${_kw} c++abi)
  endif()
endforeach()

target_compile_options(ixwebsocket PRIVATE
  -Wno-shorten-64-to-32
  -Wno-unused-const-variable
  -Wno-unused-parameter
  -Wno-unused-private-field
)

# `FetchContent_Declare()` only supports `EXCLUDE_FROM_ALL` starting from CMake 3.28. Manually
# exclude some external targets from ALL here.
set_property(DIRECTORY "${protozero_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL ON)
