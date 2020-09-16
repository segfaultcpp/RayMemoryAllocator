#include "pch.hpp"
#include "memory_allocator.hpp"
#include <Windows.h>

namespace ray
{
	static MemoryAllocatorManager sMallocManager;

	void MemoryAllocatorManager::Initialize()
	{
		
	}

	void MemoryAllocatorManager::Destroy()
	{

	}

	MemoryAllocator::MemoryAllocator()
		: _currentPool(nullptr)
	{
		sMallocManager.Initialize();
	}

	MemoryPool* MemoryAllocatorManager::RequesPool(size_t requestedSize)
	{
		return nullptr;
	}

	MemoryPool* MemoryAllocatorManager::CreateNewPool()
	{
		return nullptr;
	}

	void* MemoryAllocator::Allocate(size_t size)
	{
		return nullptr;
	}

	void MemoryAllocator::Free(void* ptr)
	{

	}
}
