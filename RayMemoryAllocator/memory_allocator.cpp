#include "pch.hpp"
#include "memory_allocator.hpp"
#include "virtual_memory_manager.h"
#include <Windows.h>

namespace ray
{
	static MemoryManager sMallocManager;

	void MemoryManager::Initialize()
	{
		
	}

	void MemoryManager::Destroy()
	{
		
	}

	MemoryAllocator::MemoryAllocator()
		: _currentPool(nullptr)
	{
		sMallocManager.Initialize();
	}

	MemoryPool* MemoryManager::RequestPool(size_t requestedSize)
	{
		return nullptr;
	}

	MemoryPool* MemoryManager::CreateNewPool()
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
