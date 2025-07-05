#pragma once
#include "Allocator.h"

#include "Math/Math.h"

#ifndef _WIN32
#include <cstdlib>
#include <stdlib.h>
#endif

template<size_t Alignment>
struct AlignedAllocator final : Allocator {
	static_assert(Math::is_power_of_two(Alignment));

	static AlignedAllocator * instance() {
		static AlignedAllocator allocator = { };
		return &allocator;
	}

private:
	AlignedAllocator() = default;

	NON_COPYABLE(AlignedAllocator);
	NON_MOVEABLE(AlignedAllocator);

	~AlignedAllocator() = default;

	char * alloc(size_t num_bytes) override {
#ifdef _WIN32
		return static_cast<char *>(_aligned_malloc(num_bytes, Alignment));
#else
		void* ptr;
		if (posix_memalign(&ptr, Alignment, num_bytes) == 0) {
			return static_cast<char *>(ptr);
		}
		return nullptr;
#endif
	}

	void free(void * ptr) override {
#ifdef _WIN32
		_aligned_free(ptr);
#else
		std::free(ptr);
#endif
	}
};
