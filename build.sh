#!/bin/bash

# Build script for GPU Raytracer on Linux

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if CUDA is installed
if [ -z "$CUDA_PATH" ]; then
    if [ -d "/usr/local/cuda" ]; then
        export CUDA_PATH=/usr/local/cuda
    elif [ -d "/opt/cuda" ]; then
        export CUDA_PATH=/opt/cuda
    else
        echo -e "${RED}Error: CUDA not found. Please install CUDA and set CUDA_PATH environment variable.${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Using CUDA from: $CUDA_PATH${NC}"

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo -e "${YELLOW}Configuring project...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Build successful!${NC}"
echo -e "${GREEN}Executable: build/pathtracer${NC}"