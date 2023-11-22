#include "shared.h"

void *alignPointer(void *ptr, std::size_t alignment) {
    if (alignment & (alignment - 1)) {
        assert(false && "Alignment must be a power of 2.");
    }

    std::uintptr_t ptrValue = reinterpret_cast<std::uintptr_t>(ptr);
    std::uintptr_t alignedValue = (ptrValue + alignment - 1) & ~(alignment - 1);
    return reinterpret_cast<void*>(alignedValue);
}