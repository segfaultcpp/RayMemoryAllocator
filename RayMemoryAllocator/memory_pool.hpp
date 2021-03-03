#pragma once
#include "fast_pool_allocator.hpp"

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

	/*IMemoryPool(IMemoryPool&& rhs) = default;
	IMemoryPool& operator = (IMemoryPool&& rhs) = default;

	IMemoryPool(const IMemoryPool& lhs) = default;
	IMemoryPool& operator = (const IMemoryPool& lhs) = default;*/

	virtual ~IMemoryPool() {}

public:
	virtual void Destroy() noexcept {}

public:
	NODISCARD void* Allocate(size_t size) noexcept
	{
		return static_cast<u8*>(_pool) + _allocator.Allocate(size);
	}

	void Free(size_t offset, size_t size) noexcept
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

	void* GetPool() const noexcept
	{
		return _pool;
	}

};