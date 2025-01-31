#!/usr/bin/env -S sh -e

cd "$(dirname "$(realpath -- "$0")")"

example="${1-print_logs}"

cmake -S . -B build/Debug -G Ninja
cmake --build build/Debug --target "$example"
build/Debug/"$example"
