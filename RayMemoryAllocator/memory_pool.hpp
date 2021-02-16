#pragma once
#include "pool_allocator.hpp"

class IMemoryPool
{
protected:
	void* _pool;
	size_t _index;
	PoolAllocator _allocator;

public:
	IMemoryPool() = default;
	IMemoryPool(size_t poolSize, size_t index) noexcept
		: _index(index)
		, _allocator(poolSize)
	{}

	virtual ~IMemoryPool() { Destroy(); }

public:
	virtual void Destroy() noexcept {}

public:
	NODISCARD size_t Allocate(size_t size) noexcept
	{
		return _allocator.Allocate(size);
	}

	void Free(size_t oofset, size_t size) noexcept
	{
		_allocator.Free(offset, size);
	}

public:
	bool IsEnough(size_t size) const noexcept
	{
		return size <= _allocator.GetMaxAvailableSpace();
	}

	size_t GetIndex() const noexcept
	{
		return _index;
	}

};