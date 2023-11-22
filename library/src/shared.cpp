#include "shared.h"

void *alignPointer(void *ptr, std::size_t alignment) {
    if (alignment & (alignment - 1)) {
        assert(false && "Alignment must be a power of 2.");
    }

    std::uintptr_t ptrValue = reinterpret_cast<std::uintptr_t>(ptr);
    std::uintptr_t alignedValue = (ptrValue + alignment - 1) & ~(alignment - 1);
    return reinterpret_cast<void*>(alignedValue);
}

template<typename T, std::enable_if<std::is_integral<T>::value>::type *>
constexpr T cpow(T base, size_t exponent) {
    return exponent > 1 ? powImpl(base, exponent, T(1)) : base;
}

template<typename T, std::enable_if<std::is_integral<T>::value>::type *>
constexpr T powImpl(T base, size_t exponent, T result) {
    return exponent > 0 ? powImpl(base*base, exponent/2, exponent&1 ? result*base : result) : result;
}
