#include "pch.hpp"
#include "virtual_memory_manager.h"
#include <Windows.h>

namespace ray
{
	u32 GetVirtualAllocatorFlags(u32 flags)
	{
		bool multiplier = flags & eCommit;
		u32 result = (MEM_COMMIT * multiplier);

		multiplier = flags & eReserve;
		result |= (MEM_RESERVE * multiplier);

		multiplier = flags & eReset;
		result |= (MEM_RESET * multiplier);

		multiplier = flags & eRelease;
		result |= (MEM_RELEASE * multiplier);

		multiplier = flags & eDecommit;
		result |= (MEM_DECOMMIT * multiplier);
		
		return result;
	}

	void* VirtualMemoryManager::Allocate(size_t allocationSize, u32 allocationFlags, void* baseAddress, bool bLargePage)
	{
		u32 flags = GetVirtualAllocatorFlags(allocationFlags) | (bLargePage ? MEM_LARGE_PAGES : 0);
		return VirtualAlloc(baseAddress, allocationSize, flags, 0);
	}

	void VirtualMemoryManager::Free(void* baseAddress, u32 flags, size_t allocationSize)
	{
		VirtualFree(baseAddress, allocationSize, GetVirtualAllocatorFlags(flags));
	}
	u32 VirtualMemoryManager::GetPageSize()
	{
		static SYSTEM_INFO sSystemInfo;
		static bool s_bFirst = true;
		if (s_bFirst)
		{
			GetSystemInfo(&sSystemInfo);
			s_bFirst = false;
		}

		return sSystemInfo.dwPageSize;
	}
}
