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

# Check for compatible GCC versions for CUDA
echo -e "${YELLOW}Checking for CUDA-compatible GCC versions...${NC}"
COMPATIBLE_GCC_VERSIONS=("gcc-14" "gcc-13" "gcc-12" "gcc-11")
FOUND_COMPATIBLE_GCC=false

for GCC_VERSION in "${COMPATIBLE_GCC_VERSIONS[@]}"; do
    if command -v $GCC_VERSION &> /dev/null; then
        echo -e "${GREEN}Found compatible GCC: $GCC_VERSION${NC}"
        FOUND_COMPATIBLE_GCC=true
        break
    fi
done

if [ "$FOUND_COMPATIBLE_GCC" = false ]; then
    echo -e "${YELLOW}Warning: No CUDA-compatible GCC found (gcc-11 through gcc-14).${NC}"
    echo -e "${YELLOW}Your system GCC version:${NC}"
    gcc --version | head -1
    echo -e "${YELLOW}CUDA 12.9 supports GCC versions 11-14. You may encounter compilation errors.${NC}"
    echo -e "${YELLOW}To install GCC 14 on Arch Linux: sudo pacman -S gcc14${NC}"
    echo -e "${YELLOW}Continuing with system GCC...${NC}"
fi

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