#pragma once
#include <map>
#include <mutex>

/// <summary>
/// Used by memory pool and manages its memory
/// </summary>
/// <remark>
/// 
/// </remark>
class PoolAllocator
{
private:
	struct MemorySegment;
	/// <summary>
	/// Type of the map that keeps memory blocks sorted by their offsets
	/// </summary>
	using MemorySegmentsByOffsetMap = std::map<size_t, MemorySegment>;

	/// <summary>
	/// Type of the map that keeps memory blocks sorted by their sizes
	/// </summary>
	using MemorySegmentsBySizeMap = std::multimap<size_t, MemorySegmentsByOffsetMap::iterator>;

	/// <summary>
	/// Describes memory block that not reserved
	/// </summary>
	struct MemorySegment
	{
		MemorySegment(size_t _Size) : Size(_Size) {}

		size_t Size;
		MemorySegmentsBySizeMap::iterator OrderBySizeIt;
	};

	MemorySegmentsByOffsetMap _segmentsByOffset;
	MemorySegmentsBySizeMap _segmentsBySize;

	size_t _maxAvailableSpace;
	size_t _poolSize;
	std::mutex _allocatorMutex;

public:
	PoolAllocator() noexcept = default;

	explicit PoolAllocator(size_t poolSize) noexcept
		: _poolSize(poolSize)
		, _maxAvailableSpace(poolSize)
	{}

	PoolAllocator(PoolAllocator&& rhs) = default;
	PoolAllocator& operator = (PoolAllocator&& rhs) = default;

public:
	void SetPoolSize(size_t poolSize) noexcept
	{
		_poolSize = poolSize;
	}

	size_t GetPoolSize() const noexcept
	{
		return _poolSize;
	}

	size_t GetMaxAvailableSpace() const noexcept
	{
		return _maxAvailableSpace;
	}

public:
	/// <summary>
	/// Reserves space within pool
	/// </summary>
	/// <param name="size">- Required size</param>
	/// <returns>Offset relative a begining of pool</returns>
	NODISCARD size_t Allocate(size_t size) noexcept;

	/// <summary>
	/// Releases space within pool
	/// </summary>
	/// <param name="offset">- Offset relative a begining of pool</param>
	/// <param name="size">- Size that we need to release</param>
	void Free(size_t offset, size_t size) noexcept;

private:
	void AddNewSegment(size_t offset, size_t size) noexcept;

};