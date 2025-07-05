# Building GPU Raytracer on Linux

## Prerequisites

1. **CUDA Toolkit** (11.0 or later)
   ```bash
   # Ubuntu/Debian
   sudo apt install nvidia-cuda-toolkit
   
   # Or download from NVIDIA website
   ```

2. **Development Dependencies**
   ```bash
   # Ubuntu/Debian
   sudo apt install build-essential cmake git
   sudo apt install libsdl2-dev libglew-dev libgl1-mesa-dev
   sudo apt install libx11-dev libxxf86vm-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev
   ```

3. **NVIDIA GPU Driver** (compatible with your CUDA version)

## Building

1. **Clone the repository** (if not already done)
   ```bash
   git clone <repository-url>
   cd GPU-Raytracer
   ```

2. **Build the project**
   ```bash
   ./build.sh
   ```

   This script will:
   - Check for CUDA installation
   - Create a build directory
   - Run CMake to configure the project
   - Build using all available CPU cores

## Running

```bash
# Basic run with default settings
./pt.sh

# Run with specific scene
./pt.sh -s Data/cornellbox/scene.xml

# Run with custom settings
./pt.sh -W 1920 -H 1080 -N 1000 -b 8

# See all options
./pt.sh --help
```

## Troubleshooting

### CUDA not found
- Ensure CUDA is installed: `nvcc --version`
- Set CUDA_PATH: `export CUDA_PATH=/usr/local/cuda`
- Add to ~/.bashrc for persistence

### Missing dependencies
- SDL2: `sudo apt install libsdl2-dev`
- GLEW: `sudo apt install libglew-dev`
- OpenGL: `sudo apt install libgl1-mesa-dev`

### Build errors
- Check CMake output for missing dependencies
- Ensure your GCC version supports C++17: `g++ --version`
- For CUDA errors, check GPU compute capability in CMakeLists.txt

### Runtime errors
- Ensure NVIDIA driver is properly installed: `nvidia-smi`
- Check that your GPU supports the compute capability specified
- For X11 errors, ensure you're running with a display server

## Performance Tips

- Use Release build (default in build.sh)
- The BVH8 acceleration structure (default) is typically fastest
- Enable compiler optimizations with `-march=native`

## Differences from Windows Version

- Uses CMake instead of Visual Studio
- Dependencies installed via package manager instead of bundled DLLs
- Scripts use .sh instead of .bat
- Some Windows-specific optimizations (like Optimus) are disabled