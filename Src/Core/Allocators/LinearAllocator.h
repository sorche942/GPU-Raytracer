#pragma once
#include "Allocator.h"
#include "Core/Assertion.h"

// Linear burn-through Allocator
// Every new allocation simply increases the offset within the buffer by the desired number of bytes
// Once a buffer runs out, a new buffer is allocated in a linked-list fashion
// Memory is freed in bulk at the end of the lifetime of the LinearAllocator
template<size_t Size>
struct LinearAllocator final : Allocator {
	char * data   = nullptr;
	size_t offset = 0;
	LinearAllocator<Size> * next = nullptr;

	LinearAllocator() {
		data = new char[Size];
	}

	NON_COPYABLE(LinearAllocator);
	NON_MOVEABLE(LinearAllocator);

	~LinearAllocator() {
		ASSERT(data);
		delete [] data;
		if (next) {
			next->~LinearAllocator();
		}
	}

	void reset() {
		offset = 0;
		if (next) {
			next->reset();
		}
	}

private:
	char * alloc(size_t num_bytes) override {
		if (num_bytes >= Size) {
			return new char[num_bytes]; // Fall back to heap allocation
		}
		if (offset + num_bytes <= Size) {
			char * result = data + offset;
			offset += num_bytes;
			return result;
		} else if (!next) {
			next = new LinearAllocator();
		}
		return next->alloc(num_bytes);
	}

	void free(void * ptr) override {
		if (ptr >= data && ptr < data + Size) {
			// Do nothing, memory will be freed in bulk inside the destructor
		} else if (next) {
			next->free(ptr);
		} else {
			delete [] ptr; // ptr was not in any valid range, must have been a heap allocated pointer
		}
	}
};
