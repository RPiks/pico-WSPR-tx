#!/bin/bash

mkdir build
git submodule init
git submodule update
cd build
cmake ..
make
