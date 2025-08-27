# CMake 3.20 is required for projects which define targets that transitively depend on `horus::sdk`
# but are not defined in `CMAKE_SOURCE_DIR`: https://gitlab.kitware.com/cmake/cmake/-/issues/18399.
cmake_minimum_required(VERSION 3.20)

project(horus_sdk_examples LANGUAGES CXX)

# Import the Horus SDK.
#
# See https://cmake.org/cmake/help/latest/module/ExternalProject.html#command:externalproject_add
# for alternative options (such as cloning from Git).

include(FetchContent)

FetchContent_Declare(horus_sdk
  # We import from the root of the repository and specify a `SOURCE_SUBDIR` here, since that's
  # likely what clients will want to do (by referring to the root of the repository either with a
  # `GIT_REPOSITORY` or by downloading a zip of the repository with `URL`).
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.."
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/horus"
  SOURCE_SUBDIR cpp

  DOWNLOAD_COMMAND ""
  INSTALL_COMMAND  ""

  FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(horus_sdk)

# Include `..` to `#include "examples/helpers.h"` in the examples.
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

# Create an executable which depends on `horus::sdk`.
#
# Make sure to add `horus::sdk` to `target_link_libraries()` (which will automatically add it as a
# dependency), and its `SOURCE_DIR` to `target_include_directories()` in order to include
# `horus/sdk.h`.

add_executable(print_events
  helpers.cpp
  helpers.h
  print_events.cpp
)
target_link_libraries(print_events horus::sdk)


add_executable(print_health_status
  helpers.cpp
  helpers.h
  print_health_status.cpp
)
target_link_libraries(print_health_status horus::sdk)

add_executable(print_logs
  helpers.cpp
  helpers.h
  print_logs.cpp
)
target_link_libraries(print_logs horus::sdk)

add_executable(print_occupancy_map
  helpers.cpp
  helpers.h
  print_occupancy_map.cpp
)
target_link_libraries(print_occupancy_map horus::sdk)
