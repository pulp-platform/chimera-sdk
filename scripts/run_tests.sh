#!/bin/bash

# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set -e

# Default configuration
DEFAULT_GVSOC_PATH="/app/install/gvsoc/bin/gvsoc"
DEFAULT_GDB_PATH="gdb-multiarch"
DEFAULT_CMAKE="cmake"
DEFAULT_TOOLCHAIN_DIR="/app/install/llvm-18.1.4-pulp"
DEFAULT_PICOLIBC_DIR="/app/install/picolibc"
DEFAULT_TARGET="chimera-open"
DEFAULT_BACKEND="GVSoC"

GLOBAL_TIMEOUT=10

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Available targets and their tests (based on GitLab CI configuration)

declare -A TARGET_TESTS

# Function to print usage
print_usage() {
    cat <<EOF
Usage: $0 [OPTIONS] [TEST_NAME...]

Run GVSoC tests locally for the Chimera-SDK project.

OPTIONS:
    -g, --gvsoc-path PATH      Path to GVSoC binary (default: $DEFAULT_GVSOC_PATH)
    -G, --gdb-path PATH        Path to GDB binary for ASIC backend (default: $DEFAULT_GDB_PATH)
    -t, --target TARGET        Target platform to build and test (default: $DEFAULT_TARGET)
                               Available targets: chimera-open, chimera-host, chimera-convolve
    -c, --cmake PATH           Path to CMake binary (default: $DEFAULT_CMAKE)
    -T, --toolchain-dir PATH   Path to LLVM toolchain directory (default: $DEFAULT_TOOLCHAIN_DIR)
    -P, --picolibc-dir PATH    Path to picolibc directory (default: $DEFAULT_PICOLIBC_DIR)
    -b, --build-only           Only build tests, don't run them
    -e, --backend BACKEND      Specify the backend to use (default: $DEFAULT_BACKEND)
    -r, --run-only             Only run tests, skip building (requires existing build)
    -l, --list-tests           List available tests for the specified target
    -L, --list-targets         List available targets
    -j, --jobs N               Number of parallel build jobs (default: auto-detected)
    -v, --verbose              Enable verbose output
    -vv, --very-verbose        Enable very verbose output
    -h, --help                 Show this help message

ARGUMENTS:
    TEST_NAME...               Specific test(s) to run. If not specified, all tests for the target will be run.

EXAMPLES:
    # Run all tests for chimera-open target (default)
    $0

    # Run specific test
    $0 test_host_returnZero

    # Run tests for different target
    $0 --target chimera-convolve

    # Use custom GVSoC path
    $0 --gvsoc-path /custom/path/to/gvsoc

    # List available tests for chimera-convolve
    $0 --target chimera-convolve --list-tests

    # Build only, don't run tests
    $0 --build-only

    # Run multiple specific tests
    $0 test_host_returnZero test_snitchCluster_simpleOffload

EOF
}

# Function to print colored output
print_status() {
    local color=$1
    local message=$2
    echo -e "${color}[$(date '+%Y-%m-%d %H:%M:%S')] ${message}${NC}"
}

print_info() { print_status "$BLUE" "INFO: $1"; }
print_success() { print_status "$GREEN" "SUCCESS: $1"; }
print_warning() { print_status "$YELLOW" "WARNING: $1"; }
print_error() { print_status "$RED" "ERROR: $1"; }

# Ensure flags that need a value actually get one
require_value() {
    local flag=$1
    if [[ $# -lt 2 || -z $2 || $2 == -* ]]; then
        print_error "Missing value for $flag"
        exit 1
    fi
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to extract target
extract_targets() {
    # Extract available targets from targets folder
    AVAILABLE_TARGETS=()
    for dir in "$PROJECT_ROOT/targets/"*; do
        if [[ -d "$dir" ]]; then
            AVAILABLE_TARGETS+=("$(basename "$dir")")
        fi
    done
}

# Function to validate target
validate_target() {
    local target=$1
    for t in "${AVAILABLE_TARGETS[@]}"; do
        if [[ "$t" == "$target" ]]; then
            return 0
        fi
    done
    echo "Invalid target: $target"
    echo "Available targets: ${AVAILABLE_TARGETS[*]}"
    exit 1
}

# Function to extract tests for a target
extract_tests() {
    local target=$1
    local backend=$2
    local cmake_cmd=$3
    local toolchain_dir=$4
    local picolibc_dir=$5
    local build_dir=$6

    # Configure build to get CMake targets
    local cmake_args=(
        "-DTARGET_PLATFORM=$target"
        "-DTOOLCHAIN_DIR=$toolchain_dir"
        "-DPICOLIBC_DIR=$picolibc_dir"
        "-DHARDWARE_BACKEND=$backend"
        "-DCHIMERA_UNIFIED_ELF=ON"
        "-B" "$build_dir"
    )

    if ! $cmake_cmd "${cmake_args[@]}" >/dev/null; then
        print_error "CMake configuration failed"
        exit 1
    fi

    # Query CMake for executable targets
    # Filter out intermediate per-domain targets (_host, _cluster, _device suffixes)
    local targets
    targets=$($cmake_cmd --build "$build_dir" --target help 2>/dev/null | grep -E "^\.\.\. test_" | sed 's/\.\.\. //' | grep -v -E "(\.elf|_runtime|_gen_symbols|_gen_placement|_device.*)$" || true)

    # Populate TARGET_TESTS associative array
    TARGET_TESTS["$target"]=""
    for test in $targets; do
        TARGET_TESTS["$target"]+="$test "
    done
}

# Function to extract tests for a target
list_tests() {
    local target=$1

    echo "Available tests for target '$target':"

    if [[ -n "${TARGET_TESTS[$target]}" ]]; then
        for test in ${TARGET_TESTS[$target]}; do
            echo "  - $test"
        done
    else
        print_warning "No tests found for target '$target'."
    fi
}

# List available targets
list_targets() {
    echo "Available targets:"
    for target in "${AVAILABLE_TARGETS[@]}"; do
        echo "  - $target"
    done
}

# Function to build the project
build_project() {
    local target=$1
    local backend=$2
    local cmake_cmd=$3
    local toolchain_dir=$4
    local picolibc_dir=$5
    local jobs=$6
    local verbose=$7
    local build_dir=$8

    print_info "Building target: $target"

    # Configure build
    # CHIMERA_UNIFIED_ELF=ON ensures a merged host ELF suitable for simulation
    # is always produced alongside the individual per-domain binaries.
    local cmake_args=(
        "-DTARGET_PLATFORM=$target"
        "-DTOOLCHAIN_DIR=$toolchain_dir"
        "-DPICOLIBC_DIR=$picolibc_dir"
        "-DHARDWARE_BACKEND=$backend"
        "-DCHIMERA_UNIFIED_ELF=ON"
        "-B" "$build_dir"
    )

    if [[ $verbose -gt 1 ]]; then
        cmake_args+=("-DCMAKE_VERBOSE_MAKEFILE=ON")
    fi

    print_info "Configuring CMake..."
    if [[ $verbose -gt 0 ]]; then
        if ! $cmake_cmd "${cmake_args[@]}"; then
            print_error "CMake configuration failed"
            exit 1
        fi
    else
        if ! $cmake_cmd "${cmake_args[@]}" >/dev/null; then
            print_error "CMake configuration failed"
            exit 1
        fi
    fi

    # Build
    local build_args=("--build" "$build_dir")
    if [[ -n $jobs ]]; then
        build_args+=("-j" "$jobs")
    else
        build_args+=("-j")
    fi

    if [[ $verbose -gt 1 ]]; then
        build_args+=("--verbose")
    fi

    print_info "Building project..."
    if [[ $verbose -gt 0 ]]; then
        if ! $cmake_cmd "${build_args[@]}"; then
            print_error "Build failed"
            exit 1
        fi
    else
        if ! $cmake_cmd "${build_args[@]}" >/dev/null; then
            print_error "Build failed"
            exit 1
        fi
    fi

    print_success "Build completed successfully"
}

# Function to run a single test on GVSoC
run_gvsoc_test() {
    local test_name=$1
    local target=$2
    local gvsoc_path=$3
    local verbose=$4
    local build_dir=$5

    local binary_path="$build_dir/bin/${test_name}_unified.elf"

    # Check if binary exists
    if [[ ! -f "$binary_path" ]]; then
        print_error "Test binary not found: $binary_path"
        print_info "Make sure the project has been built successfully"
        return 1
    fi

    print_info "Running test: $test_name"

    # Change to GVSoC directory (as done in GitLab CI)
    local gvsoc_dir
    gvsoc_dir="$(dirname "$binary_path")"
    gvsoc_dir="$(cd "$gvsoc_dir/.." && pwd)"

    # Prepare GVSoC command
    local gvsoc_cmd=(
        "$gvsoc_path"
        "--target=chimera"
        "--work-dir=$gvsoc_dir"
        "--binary=$binary_path"
        "run"
    )

    if [[ $verbose -gt 0 ]]; then
        gvsoc_cmd+=("--verbose=debug")
        print_info "Working directory: $gvsoc_dir"
        print_info "GVSoC command: ${gvsoc_cmd[*]}"
    fi

    local ret=0
    local output
    output=$(timeout "$GLOBAL_TIMEOUT" "${gvsoc_cmd[@]}" 2>&1) || ret=$?

    if [[ $ret -eq 124 ]]; then
        print_warning "GVSoC output:"
        echo "$output"
        print_error "Test timed out after ${GLOBAL_TIMEOUT}s: $test_name"
        return 1
    fi

    if [[ $verbose -gt 0 ]]; then
        print_info "GVSoC output:"
        echo "$output"
    fi

    # Return value (search for exitcode: N in output)
    local return_value
    return_value=$(echo "$output" | grep -oE "exitcode: [0-9]+" | awk '{print $2}')

    # Check return value from GVSoC output
    if [[ "$ret" == "0" ]]; then
        print_success "Test passed: $test_name"
        return 0
    elif [[ -n "$return_value" ]]; then
        print_error "Test failed: $test_name (exit code: $return_value)"
        return 1
    else
        print_error "Test failed: $test_name"
        return 1
    fi
}

# Function to run a single test on the ASIC
# Assume that openocd exposes GDB port on localhost:3333
run_asic_test() {
    local test_name=$1
    local target=$2
    local gdb_path=$3
    local verbose=$4
    local build_dir=$5

    local binary_path="$build_dir/bin/$test_name"
    local gdb_port="3333"
    local gdb_host="host.docker.internal"

    # Check if binary exists
    if [[ ! -f "$binary_path" ]]; then
        print_error "Test binary not found: $binary_path"
        print_info "Make sure the project has been built successfully"
        return 1
    fi

    # Check if openocd is running (simple check)
    if ! nc -z "$gdb_host" "$gdb_port"; then
        print_error "OpenOCD is not running or GDB port $gdb_port is not accessible"
        print_info "Please start OpenOCD with the appropriate configuration for the target"
        return 1
    fi

    print_info "Running test: $test_name"

    # Create temporary GDB command file
    local gdb_script=$(mktemp)
    cat >"$gdb_script" <<EOF
set tcp connect-timeout 0.5
target extended-remote ${gdb_host}:${gdb_port}
monitor reset halt
load
run
print \$a0
quit
EOF

    if [[ $verbose -gt 0 ]]; then
        print_info "GDB script:"
        cat "$gdb_script"
    fi

    # Run GDB with the script
    local gdb_cmd=(
        "$gdb_path"
        "-nx"
        "$binary_path"
        "-x" "$gdb_script"
        "--batch"
    )

    if [[ $verbose -gt 0 ]]; then
        print_info "GDB command: ${gdb_cmd[*]}"
    fi

    local ret=0
    local output
    output=$(timeout "$GLOBAL_TIMEOUT" "${gdb_cmd[@]}" 2>&1) || ret=$?

    if [[ $ret -eq 124 ]]; then
        print_error "Test timed out after ${GLOBAL_TIMEOUT}s: $test_name"
        rm -f "$gdb_script"
        return 1
    fi

    if [[ $verbose -gt 0 ]]; then
        print_info "GDB output:"
        echo "$output"
    fi

    # Extract return value from GDB output
    local return_value
    return_value=$(echo "$output" | grep "^\$1 =" | awk '{print $3}')

    # Check the output for the return value
    if [[ "$return_value" == "0" ]]; then
        print_success "Test passed: $test_name"
        ret=0
    elif [[ -n "$return_value" ]]; then
        print_error "Test failed: $test_name (exit code: $return_value)"
        ret=1
    else
        print_error "Test failed: $test_name"
        ret=1
    fi

    # Cleanup
    rm -f "$gdb_script"
    return $ret
}

# Function to run tests
run_tests() {
    local target=$1
    local backend=$2
    local gvsoc_path=$3
    local gdb_path=$4
    local verbose=$5
    local build_dir=$6
    shift 6
    local tests=("$@")

    # If no specific tests provided, use all tests for the target
    if [[ ${#tests[@]} -eq 0 ]]; then
        read -ra tests <<<"${TARGET_TESTS[$target]}"
    fi

    # Validate that all specified tests are available for the target
    local available_tests
    read -ra available_tests <<<"${TARGET_TESTS[$target]}"
    for test in "${tests[@]}"; do
        if [[ ! " ${available_tests[*]} " =~ " $test " ]]; then
            print_error "Test '$test' is not available for target '$target'"
            print_info "Available tests: ${available_tests[*]}"
            exit 1
        fi
    done

    # Check if GVSoC binary exists
    if [[ ! -f "$gvsoc_path" && "$backend" == "GVSoC" ]]; then
        print_error "GVSoC binary not found: $gvsoc_path"
        print_info "Please install GVSoC or provide the correct path with --gvsoc-path"
        exit 1
    fi

    # Run tests
    local passed=0
    local failed=0
    local failed_tests=()

    print_info "Running ${#tests[@]} test(s) for target '$target'"
    echo

    for test in "${tests[@]}"; do
        # Temporarily disable exit on error for individual tests
        if [[ $backend == "GVSoC" ]]; then
            test_command="run_gvsoc_test"
            runner_path="$gvsoc_path"
        elif [[ $backend == "ASIC" ]]; then
            test_command="run_asic_test"
            runner_path="$gdb_path"
        else
            print_error "Unsupported backend: $backend"
            exit 1
        fi
        set +e
        if $test_command "$test" "$target" "$runner_path" "$verbose" "$build_dir"; then
            ((passed++))
        else
            ((failed++))
            failed_tests+=("$test")
        fi
        # Re-enable exit on error
        set -e
        echo # Add empty line between tests
    done

    # Print summary
    print_info "Test Summary:"
    print_success "Passed: $passed"
    if [[ $failed -gt 0 ]]; then
        print_error "Failed: $failed"
        print_error "Failed tests: ${failed_tests[*]}"
        exit 1
    else
        print_success "All tests passed!"
    fi
}

# Parse command line arguments
GVSOC_PATH="$DEFAULT_GVSOC_PATH"
GDB_PATH="$DEFAULT_GDB_PATH"
TARGET="$DEFAULT_TARGET"
CMAKE_CMD="$DEFAULT_CMAKE"
TOOLCHAIN_DIR="$DEFAULT_TOOLCHAIN_DIR"
PICOLIBC_DIR="$DEFAULT_PICOLIBC_DIR"
BUILD_ONLY="false"
RUN_ONLY="false"
LIST_TESTS="false"
LIST_TARGETS="false"
JOBS=""
VERBOSE=0
BACKEND="$DEFAULT_BACKEND"
TESTS=()

while [[ $# -gt 0 ]]; do
    case $1 in
    -g | --gvsoc-path)
        require_value "$1" "$2"
        GVSOC_PATH="$2"
        shift 2
        ;;
    -G | --gdb-path)
        require_value "$1" "$2"
        GDB_PATH="$2"
        shift 2
        ;;
    -t | --target)
        require_value "$1" "$2"
        TARGET="$2"
        shift 2
        ;;
    -c | --cmake)
        require_value "$1" "$2"
        CMAKE_CMD="$2"
        shift 2
        ;;
    -T | --toolchain-dir)
        require_value "$1" "$2"
        TOOLCHAIN_DIR="$2"
        shift 2
        ;;
    -P | --picolibc-dir)
        require_value "$1" "$2"
        PICOLIBC_DIR="$2"
        shift 2
        ;;
    -b | --build-only)
        BUILD_ONLY="true"
        shift
        ;;
    -e | --backend)
        require_value "$1" "$2"
        BACKEND="$2"
        shift 2
        ;;
    -r | --run-only)
        RUN_ONLY="true"
        shift
        ;;
    -l | --list-tests)
        LIST_TESTS="true"
        shift
        ;;
    -L | --list-targets)
        LIST_TARGETS="true"
        shift
        ;;
    -j | --jobs)
        require_value "$1" "$2"
        JOBS="$2"
        shift 2
        ;;
    -v | --verbose)
        VERBOSE=1
        shift
        ;;
    -vv | --very-verbose)
        VERBOSE=2
        shift
        ;;
    -h | --help)
        print_usage
        exit 0
        ;;
    -*)
        print_error "Unknown option: $1"
        print_usage
        exit 1
        ;;
    *)
        TESTS+=("$1")
        shift
        ;;
    esac
done

# Check if CMAKE command exists
if ! command_exists "$CMAKE_CMD"; then
    print_error "CMake not found: $CMAKE_CMD"
    print_info "Please install CMake or provide the correct path with --cmake"
    exit 1
fi

if [[ $BUILD_ONLY == "true" && $RUN_ONLY == "true" ]]; then
    print_error "Cannot specify both --build-only and --run-only"
    exit 1
fi

# Validate arguments
extract_targets

# Handle list targets
if [[ $LIST_TARGETS == "true" ]]; then
    list_targets
    exit 0
fi

BUILD_DIR="${PROJECT_ROOT}/build-${TARGET}"

validate_target "$TARGET"

extract_tests "$TARGET" "$BACKEND" "$CMAKE_CMD" "$TOOLCHAIN_DIR" "$PICOLIBC_DIR" "$BUILD_DIR"

# Handle list tests
if [[ $LIST_TESTS == "true" ]]; then
    list_tests "$TARGET"
    exit 0
fi

# Change to project root
cd "$PROJECT_ROOT"

# Main execution
if [[ $RUN_ONLY == "false" ]]; then
    build_project "$TARGET" "$BACKEND" "$CMAKE_CMD" "$TOOLCHAIN_DIR" "$PICOLIBC_DIR" "$JOBS" "$VERBOSE" "$BUILD_DIR"
fi

# WIESEP: Currently only GVSoC and ASIC are supported as backend
if [[ $BUILD_ONLY == "false" ]]; then
    if [[ "$BACKEND" != "GVSoC" && "$BACKEND" != "ASIC" ]]; then
        print_error "Unsupported backend: $BACKEND. Currently only GVSoC and ASIC are supported."
        exit 1
    fi
    run_tests "$TARGET" "$BACKEND" "$GVSOC_PATH" "$GDB_PATH" "$VERBOSE" "$BUILD_DIR" "${TESTS[@]}"
fi
