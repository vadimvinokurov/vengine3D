//
// Created by boris on 7/21/2022.
//
#include "EngineLibs.h"
#include "StackAllocator.h"
#include "MemoryUtils.h"

StackAllocator::StackAllocator(void* memory, size_t size)
	: IAllocator(memory, size) {}

void* StackAllocator::allocate(size_t size, uint8 alignment) {
	assert(size > 0 && "allocate called with memSize = 0.");

	union {
		void* asVoid;
		uptr asUptr;
	};
	asVoid = baseAddress_;
	asUptr += usedMemory_;

	auto adjustment = MemoryUtils::AlignAdjustment(asVoid, alignment, sizeof(MetaInfo));
	if (usedMemory_ + size + adjustment > maxSize_) {
		return nullptr;
	}

	asUptr += adjustment;
	MetaInfo* meta = (MetaInfo*) (asUptr - sizeof(MetaInfo));
	meta->adjustment = adjustment;

	usedMemory_ += size + adjustment;

	return asVoid;
}

void StackAllocator::deallocate(void* ptr) {
	assert(usedMemory_ > 0 && "Memory already free.");

	union {
		void* asVoid;
		uptr asUptr;
	};
	asVoid = ptr;
	MetaInfo* meta = (MetaInfo*) (asUptr - sizeof(MetaInfo));

	asUptr -= meta->adjustment;

	auto freedMemorySize = ((uptr) baseAddress_ + usedMemory_) - asUptr;
	usedMemory_ -= freedMemorySize;
}
void StackAllocator::clear() {
	usedMemory_ = 0;
}
