#!/usr/bin/env bash

mkdir -p build && cd build

# init cmake from CMakeLists in project root
cmake -DCMAKE_BUILD_TYPE=Debug ..

# build
cmake --build .
