#pragma once
#include "memory_pool.hpp"
#include "memory_pool_manager.hpp"
#include <Windows.h>

class ExamplePool : public IMemoryPool
{
public:
	ExamplePool(size_t poolSize, size_t index) 
		: IMemoryPool(poolSize, index)
	{
		_pool = VirtualAlloc(nullptr, poolSize, MEM_COMMIT, PAGE_READWRITE);
	}

	~ExamplePool() override 
	{
		Destroy();
	}

public:
	void Destroy() noexcept override
	{
		if (_pool != nullptr)
		{
			VirtualFree(_pool, 0, MEM_RELEASE);
			_pool = nullptr;
		}
	}

};

class ExampleAllocator
{
private:
	MemoryPoolManager<ExamplePool> _manager;
	ExamplePool* _currentPool;

	struct Header
	{
		size_t Size;
		size_t Offset;
		size_t PoolIndex;
	};

public:
	ExampleAllocator() noexcept
	{
		_manager.Initialize(KB(64));
		_currentPool = &_manager.RequestPool(KB(64));
	}

	explicit ExampleAllocator(size_t preferredSize) noexcept
	{
		_manager.Initialize(preferredSize);
		_currentPool = &_manager.RequestPool(preferredSize);
	}

public:
	NODISCARD void* Allocate(size_t size) noexcept
	{
		Timer timer("ExampleAllocator::Allocate");

		constexpr size_t headerSize = sizeof(Header);

		if (!_currentPool->IsEnough(size + headerSize))
		{
			_currentPool = &_manager.RequestPool(size + headerSize);
		}

		void* ret = _currentPool->Allocate(size + headerSize);
		Header header = 
		{
			.Size = size,
			.Offset = static_cast<size_t>(static_cast<u8*>(ret) - static_cast<u8*>(_currentPool->GetPool())),
			.PoolIndex = _currentPool->GetIndex()
		};
		*static_cast<Header*>(ret) = header;

		return static_cast<u8*>(ret) + headerSize;
	}

	void Free(void* mem)
	{
		Timer timer("ExampleAllocator::Free");

		constexpr size_t headerSize = sizeof(Header);
		Header header = *static_cast<Header*>(static_cast<void*>(static_cast<u8*>(mem) - headerSize));
		auto& pool = _manager.RequestPoolByIndex(header.PoolIndex);
		
		pool.Free(header.Offset, header.Size + headerSize);
	}

};
