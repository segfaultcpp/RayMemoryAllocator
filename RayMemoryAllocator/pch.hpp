#pragma once
#include <cstdint>
#include "timer.hpp"

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

#define KB(N) (N * 1024)
#define MB(N) (KB(N) * 1024)
#define GB(N) (MB(N) * 1024)

#ifdef __has_cpp_attribute(nodiscard)
	#define NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(gnu::warn_unused_result)
	#define NODISCARD [[gnu::warn_unused_result]]
#endif

#ifdef __has_cpp_attribute(maybe_unused)
	#define MAYBE_UNUSED [[maybe_unused]]
#elif __has_cpp_attribute(gnu::unused)
	#define MAYBE_UNUSED [[gnu::unused]]
#endif