#pragma once

namespace ray
{
	enum VirtualAllocatorFlags
	{
		eCommit = 1 << 0,
		eReserve = 1 << 1,
		eReset = 1 << 2,
		eRelease = 1 << 3,
		eDecommit = 1 << 4
	};

	// static class
	class VirtualMemoryManager
	{
	public:
		static void* Allocate(size_t allocationSize, u32 allocationFlags, void* baseAddress = nullptr, bool bLargePage = false);
		static void Free(void* baseAddress, u32 flags, size_t allocationSize = 0);

	};

}


