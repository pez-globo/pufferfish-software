#!/bin/bash

CMAKE_BUILD_DIR=`echo "cmake-build-$1" | awk '{print tolower($0)}'`
rm -rf $CMAKE_BUILD_DIR
mkdir $CMAKE_BUILD_DIR
cd $CMAKE_BUILD_DIR
cmake .. \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE="$1" \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake \
  -DTOOLCHAIN_PREFIX="$2"
cd ..
