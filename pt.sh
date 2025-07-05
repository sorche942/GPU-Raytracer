#!/bin/bash

# Run script for GPU Raytracer on Linux

# Check if executable exists
if [ ! -f "build/pathtracer" ]; then
    echo "Error: pathtracer executable not found!"
    echo "Please run ./build.sh first"
    exit 1
fi

# Run pathtracer with all arguments passed to this script
./build/pathtracer "$@"