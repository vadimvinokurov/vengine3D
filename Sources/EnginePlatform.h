//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_ENGINEPLATFORM_H
#define VENGINE3D_ENGINEPLATFORM_H

#include <cstdint>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#else
#define ENV32BIT
#endif

#ifdef ENV64BIT
static_assert(sizeof(size_t) == 8, "Platform sizeof(size_t) not as expected");
static_assert(sizeof(uintptr_t) == 8, "Platform sizeof(uintptr_t) not as expected");
static_assert(sizeof(void*) == 8, "Platform sizeof(void*) not as expected");
#else
static_assert(sizeof(size_t) == 4, "Platform sizeof(size_t) not as expected");
static_assert(sizeof(uintptr_t) == 4, "Platform sizeof(uintptr_t) not as expected");
static_assert(sizeof(void*) == 4, "Platform sizeof(void*) not as expected");
#endif
static_assert(sizeof(char) == 1, "Platform sizeof(char) not as expected");

#ifdef ENV64BIT
using int64 = int64_t;
using uint64 = uint64_t;
#else
using int64 = int32_t;
using uint64 = uint32_t;
#endif

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using byte = uint8_t;
using uptr = uintptr_t;
using idtype = size_t;

#endif // VENGINE3D_ENGINEPLATFORM_H
