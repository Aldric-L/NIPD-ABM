#!/bin/bash -e

cmake -B build -DCMAKE_BUILD_TYPE=Debug || exit 1
cmake --build build --config Debug || exit 1
./build/bin/NIPD_ABM
