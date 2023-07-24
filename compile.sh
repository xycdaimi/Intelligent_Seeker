#!/bin/bash

if [ -d "./build" ]; then
  echo "Removing existing build directory..."
  rm -r -d "./build"
fi

echo "Creating build directory..."
mkdir "./build"

echo "Build directory created successfully!"

cd ./build

cmake ..

make -j 7

make install