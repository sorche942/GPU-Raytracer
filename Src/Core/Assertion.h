#pragma once
#include <cstdio>
#include <cstdlib>

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

#define ASSERT(assertion)                                                          \
	do {                                                                           \
		if (!(assertion)) {                                                        \
			printf("%s:%i: ASSERT(" #assertion ") failed!\n", __FILE__, __LINE__); \
			DEBUG_BREAK();                                                         \
		}                                                                          \
	} while(false)

#define ASSERT_UNREACHABLE() abort()
