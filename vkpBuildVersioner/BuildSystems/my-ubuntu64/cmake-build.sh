#!/bin/bash
clear
export CURRP=$(pwd)
export BASEP="$CURRP/../.."
cp CMakeLists.txt "$BASEP/C++/CMakeLists.txt"
cd "$BASEP/C++/"
rm -rf build
mkdir build && cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cd "$CURRP"
