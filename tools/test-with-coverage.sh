#!/usr/bin/env -S bash -e

build_dir="${BUILD_DIR:-build/current}"
coverage_dir="$build_dir/coverage"

llvm_profdata="${LLVM_PROFDATA:-llvm-profdata}"
llvm_cov="${LLVM_COV:-llvm-cov}"

mkdir -p "$coverage_dir"
rm -rf "$coverage_dir/*.profraw"

# Run tests, dumping coverage information to `$coverage_dir`.
exit_code=0
object_args=()
for test_file in "$build_dir"/horus_*_test; do
  LLVM_PROFILE_FILE="$coverage_dir/$(basename "$test_file").profraw" "$test_file" \
    || exit_code=$?
  object_args+=(-object "$test_file")
done
cov_args=(-ignore-filename-regex='build/|testing/' "${object_args[@]}")

# Merge coverage information.
"$llvm_profdata" merge -sparse -o "$coverage_dir/all.profdata" \
  "$coverage_dir"/horus_*_test.profraw

# Save coverage information to an lcov.info file.
"$llvm_cov" export -format=lcov -instr-profile="$coverage_dir/all.profdata" "${cov_args[@]}" \
  > "$coverage_dir/lcov.info"

# Print coverage information.
"$llvm_cov" report -instr-profile="$coverage_dir/all.profdata" "${cov_args[@]}"

exit $exit_code
