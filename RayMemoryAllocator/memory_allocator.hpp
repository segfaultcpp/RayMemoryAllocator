#pragma once

namespace ray
{
#pragma pack(push, 4)
	struct AllocHeader
	{
		u32 AllocatedDataSize;
		u32 MemoryPoolIndex;
		u32 MemoryPageIndex;
	};
#pragma pack(pop)


	struct MemorySegment
	{
		void* Ptr;
		u32 Size;
	};

#pragma pack(push, 2)
	struct MemoryPage
	{
		void* Buffer;
		u32 AvailableSpace; // atomic?
		u32 FreedSpace; // atomic?
		bool bCommitted; // atomic?
	};
#pragma pack(pop)


#pragma pack(push, 4)
	struct MemoryPool
	{
		char PoolName[64];
		MemoryPage ListOfPages;
		MemoryPage ListOfLargePages;
		u32 MaxPageSize; // TODO greater than 4 KB
	};
#pragma pack(pop)

	class MemoryManager
	{
	public:
		~MemoryManager()
		{
			Destroy();
		}

		void Initialize();
		void Destroy();

		MemoryPool* RequestPool(size_t requestedSize);
		MemoryPool* CreateNewPool();

	private:
		MemoryPage _listOfPools;

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


