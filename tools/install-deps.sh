#!/usr/bin/env -S bash -eo pipefail

host="$(uname --machine)"
deps_dir="$(pwd)/.cache/deps"

# Parse arguments.
apt_packages=(ca-certificates)
apt_llvm=0
install_cmake=0
install_go=0
install_protoc=0

while [[ $# -gt 0 ]]; do
  case $1 in
    clang*18)
      apt_llvm=1
      apt_packages+=(curl "$1")
      ;;
    cmake)
      apt_packages+=(curl gzip)
      install_cmake=1
      ;;
    go)
      install_go=1
      ;;
    ninja)
      apt_packages+=(ninja-build)
      ;;
    protoc)
      apt_packages+=(unzip)
      install_protoc=1
      ;;
    *)
      echo "unknown argument: $1"
      exit 1
      ;;
  esac
  shift
done

# Note: `curl -fsSL` = `curl --fail --silent --show-error --location`.

# Install LLVM.
if test "$apt_llvm" -eq 1; then
  curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -

  sudo add-apt-repository --yes 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-18 main'
  sudo apt-get update
fi

# Install system packages.
sudo apt-get install --yes --no-install-recommends "${apt_packages[@]}"

# Install local packages.
add_to_path() {
  if test -n "$GITHUB_PATH"; then
    echo "$1" >> "$GITHUB_PATH"
  else
    echo "Please add $1 to the PATH."
  fi
}

# Install `cmake`.
if test "$install_cmake" -eq 1; then
  if ! test -x "$deps_dir/cmake/bin/cmake"; then
    mkdir -p "$deps_dir"
    curl -fsSL "https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-$host.tar.gz" \
      --output "$deps_dir/cmake.tar.gz"
    tar -xzf "$deps_dir/cmake.tar.gz" -C "$deps_dir"
    mv "$deps_dir/cmake-3.30.0-linux-$host" "$deps_dir/cmake"
    rm "$deps_dir/cmake.tar.gz"
  fi

  add_to_path "$deps_dir/cmake/bin"
fi

# Install `go`.
if test "$install_go" -eq 1; then
  if ! test -x "$deps_dir/go/bin/go"; then
    go_host="$(test "$host" = "aarch64" && echo "arm64" || echo "amd64")"

    mkdir -p "$deps_dir"
    curl -fsSL "https://go.dev/dl/go1.23.4.linux-$go_host.tar.gz" --output "$deps_dir/go.tar.gz"
    tar -xzf "$deps_dir/go.tar.gz" -C "$deps_dir"
    rm "$deps_dir/go.tar.gz"
  fi

  add_to_path "$deps_dir/go/bin"
fi

# Install `protoc`.
if test "$install_protoc" -eq 1; then
  if ! test -x "$deps_dir/protobuf/bin/protoc"; then
    pb_host="$(test "$host" = "aarch64" && echo "aarch_64" || echo "$host")"

    mkdir -p "$deps_dir"
    curl -fsSL "https://github.com/protocolbuffers/protobuf/releases/download/v28.3/protoc-28.3-linux-$pb_host.zip" \
      --output "$deps_dir/protobuf.zip"
    mkdir -p "$deps_dir/protobuf"
    unzip "$deps_dir/protobuf.zip" -d "$deps_dir/protobuf"
    rm "$deps_dir/protobuf.zip"
  fi

  add_to_path "$deps_dir/protobuf/bin"
fi
