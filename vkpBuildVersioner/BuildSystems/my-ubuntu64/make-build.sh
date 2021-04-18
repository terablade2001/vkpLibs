#!/bin/bash
clear
export CURRP=$(pwd)
export BASEP="$CURRP/../.."
cd "$BASEP/C++/build"
make -j
mv UpdateBuildVersion "$BASEP/"
cd "$CURRP"
