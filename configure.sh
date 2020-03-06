#!/bin/sh

if [ $# -eq 0 ]; then
	buildType=Debug
else
	buildType=$1
fi

git submodule update --init --recursive && mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=$buildType #-A x64

#conan install --build=missing .. && 
