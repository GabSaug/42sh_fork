#! /bin/bash

compiler_options="$1" # in the form of "-O2"

rm -rf build/
mkdir -p build/bin
cd build
if ! cmake -D CMAKE_C_FLAGS="$1 -Wno-dev" .. &> /dev/null ; then
	echo "Error cmake for 42sh"
	exit 1
fi
cd -
