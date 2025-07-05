#pragma once
#include <cuda.h>

#include "Core/IO.h"

// Platform-specific debug break
#ifdef _WIN32
    #include <intrin.h>
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #define DEBUG_BREAK() abort()
#endif

#define CHECK_CUDA_CALLS true

#if CHECK_CUDA_CALLS
#define CUDACALL(result) check_cuda_call(result, __FILE__, __LINE__);

inline void check_cuda_call(CUresult result, const char * file, int line) {
	if (result != CUDA_SUCCESS) {
		const char * error_name;
		const char * error_string;

		cuGetErrorName  (result, &error_name);
		cuGetErrorString(result, &error_string);

		IO::print("{}:{}: CUDA call failed with error {}!\n{}\n"_sv, file, line, error_name, error_string);
		DEBUG_BREAK();
	}
}
#else
#define CUDACALL(result) result
#endif
