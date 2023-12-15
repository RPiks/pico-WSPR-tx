#!/bin/bash

mkdir build
git submodule init
git submodule update
git submodule foreach git pull origin main
cd build
cmake ..
make
