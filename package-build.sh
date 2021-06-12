#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: package-release.sh destdir"
  exit 1
fi

TEST_SRC_DIR=`dirname $(readlink -f $0)`
TEST_BUILD_DIR=$(realpath "$1")

if [ -e "$TEST_BUILD_DIR/build.$1" ]; then
  echo "Build directory $TEST_BUILD_DIR already exists"
  exit 1
fi

function build_arch {
  cd "$TEST_SRC_DIR"

  meson --cross-file "$TEST_SRC_DIR/build-win$1.txt"  \
        --buildtype "release"                         \
        --strip                                       \
        "$TEST_BUILD_DIR/build.$1"

  cd "$TEST_BUILD_DIR/build.$1"
  ninja install

  mv "$TEST_BUILD_DIR/build.$1/nvtt.exe" "$TEST_BUILD_DIR"
  rm -R "$TEST_BUILD_DIR/build.$1"
}

build_arch 32
