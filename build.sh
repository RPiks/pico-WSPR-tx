#!/bin/bash

mkdir build
git submodule update
cd build
cmake ..
make
