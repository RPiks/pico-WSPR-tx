#!/bin/bash

mkdir build
git submodule foreach git pull origin main
cd build
cmake ..
make
