# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a GPU-accelerated pathtracer built with CUDA that implements state-of-the-art rendering techniques. The project focuses on performance through various BVH optimizations and wavefront rendering patterns.

## Build Commands

### Linux
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install nvidia-cuda-toolkit build-essential cmake git
sudo apt install libsdl2-dev libglew-dev libgl1-mesa-dev
sudo apt install libx11-dev libxxf86vm-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev

# Build
./build.sh

# Run
./pt.sh                                    # Default settings
./pt.sh -s Data/cornellbox/scene.xml      # Specific scene
./pt.sh -W 1920 -H 1080 -N 1000 -b 8     # Custom resolution/samples
```

### Windows
Use Visual Studio solution. Run from `x64\Release\pathtracer.exe` or use `pt.bat`.

## Architecture

### Core Components

1. **Renderer System** (`/Src/Renderer/`)
   - `Renderer` class: Main orchestrator, manages CUDA/OpenGL interop
   - `Scene` class: Scene graph, material management, BVH construction
   - `Camera` class: View matrices, interactive movement

2. **CUDA Integration** (`/Src/CUDA/`)
   - Wavefront rendering pattern for divergence reduction
   - Separate kernels for different ray types (primary, shadow, etc.)
   - Hot-reload system via `CudaModule` class

3. **BVH Acceleration** (`/Src/BVH/`)
   - Multiple implementations: BVH2, BVH4, BVH8
   - SAH and SBVH builders
   - Specialized CUDA traversal kernels per BVH type

4. **Material System** (`/Src/Renderer/Materials/`)
   - PBR materials with energy conservation
   - Importance sampling for each BSDF
   - Texture support via `TextureManager`

### Key Design Patterns

1. **Memory Management**
   - Custom allocators for GPU memory (`GpuMemoryAllocator`)
   - Aligned allocations for SIMD operations
   - Resource lifetime tied to `Scene` object

2. **Wavefront Rendering**
   - Work queues for different ray types
   - Compaction to maintain GPU occupancy
   - Separate shading and traversal phases

3. **Hot Reload System**
   - Press F5 to recompile CUDA kernels at runtime
   - Preserves scene state during recompilation
   - Essential for shader development workflow

## Code Standards

### Naming Conventions
- Classes: `PascalCase` (e.g., `PathTracer`, `BvhNode`)
- Methods: `camelCase` (e.g., `buildBvh`, `tracePath`)
- Member variables: `m_` prefix (e.g., `m_vertices`, `m_triangles`)
- CUDA kernels: `__global__` functions with descriptive names

### CUDA Kernel Organization
- Kernels grouped by functionality in separate `.cu` files
- Device functions in corresponding `.cuh` headers
- Shared memory declarations at kernel start
- Early exit patterns for divergence reduction

## Performance Considerations

1. **BVH Selection**
   - BVH8 (default): Best performance on modern GPUs
   - BVH4: Good balance for older hardware
   - SBVH: Higher quality but slower build times

2. **Optimization Flags**
   - Release builds use `-O3 -march=native -ffast-math`
   - CUDA architectures: 60, 61, 70, 75, 80, 86
   - Adjust via CMake for specific GPU

3. **Memory Patterns**
   - Coalesced memory access in CUDA kernels
   - Structure-of-arrays for vertex data
   - Texture memory for read-only data

## Scene and Materials

### Scene Format
- Primary: Mitsuba XML format
- Secondary: OBJ, PLY for individual meshes
- Examples in `/Data/` directory

### Material Properties
- Base color (albedo)
- Metallic/roughness (PBR workflow)
- Emission for area lights
- Normal maps supported

## GUI Integration

The project uses Dear ImGui for the interface:
- Scene hierarchy editor
- Material property tweaking
- Camera controls
- Renderer settings
- Performance statistics

Access via mouse interaction in render window.

## Debugging

1. **CUDA Errors**
   - Check `cuda_error_check()` calls
   - Use `cuda-gdb` for kernel debugging
   - Monitor with `nvidia-smi`

2. **Build Issues**
   - GCC version compatibility (11-14 for CUDA)
   - Check CUDA_PATH environment variable
   - Verify compute capability matches GPU

3. **Runtime Performance**
   - Use built-in profiler (`PerfTest` class)
   - Monitor GPU utilization
   - Check for warp divergence in kernels

## Extending the Codebase

### Adding New Integrators
1. Create class inheriting from `Integrator`
2. Implement `trace()` method
3. Add to `IntegratorType` enum
4. Register in `Renderer::createIntegrator()`

### Adding New Materials
1. Extend `Material` class
2. Implement BSDF evaluation/sampling
3. Add serialization support
4. Update material UI in `SceneEditor`

### Adding BVH Variants
1. Create builder in `/Src/BVH/`
2. Implement CUDA traversal kernel
3. Add to `BVHType` enum
4. Benchmark against existing implementations

## Common Pitfalls

1. **Memory Alignment**: Always use aligned allocators for SIMD types
2. **Kernel Launches**: Check grid/block dimensions for GPU limits
3. **Texture Coordinates**: Ensure proper UV mapping in assets
4. **Hot Reload**: Some changes require full rebuild (headers, class layout)
5. **Scene Scale**: Keep scenes in reasonable units for numerical stability

## Key Academic References

The implementation follows techniques from:
- "Megakernels Considered Harmful" (Laine et al.) - Wavefront design
- "Spatiotemporal Variance-Guided Filtering" (Schied et al.) - Denoising
- "Microfacet Models for Refraction" (Walter et al.) - BSDF models
- "Efficient BVH Construction via Approximate Agglomerative Clustering" (Gu et al.) - SBVH