#pragma once

namespace ray
{
	struct AllocHeader
	{
		u32 AllocatedDataSize;
		u32 MemoryPoolIndex;
	};

	struct MemoryPool
	{
		void* Buffer;
		const u32 MAX_POOL_SIZE = 64 * 1024;
		u32 AvailableSpace = MAX_POOL_SIZE; // atomic?
		u32 FreedSpace; // atomic?
	};

	class MemoryAllocatorManager
	{
	public:
		~MemoryAllocatorManager()
		{
			Destroy();
		}

		void Initialize();
		void Destroy();

		MemoryPool* RequesPool(size_t requestedSize);
		MemoryPool* CreateNewPool();

	private:
		MemoryPool _list;

	};

	class MemoryAllocator
	{
	public:
		MemoryAllocator();

		void* Allocate(size_t size);
		void Free(void* ptr);

	private:
		MemoryPool* _currentPool;

	};
}


