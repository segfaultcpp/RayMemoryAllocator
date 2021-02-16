#include "pch.hpp"
#include "pool_allocator.hpp"

size_t PoolAllocator::Allocate(size_t size) noexcept
{
	std::lock_guard mutex(_allocatorMutex);

	auto smallestSegmentItIt = _segmentsBySize.lower_bound(size);
	if (smallestSegmentItIt == _segmentsBySize.end())
		return (~0u);
	
	auto smallestSegmentIt = smallestSegmentItIt->second;
	size_t offset = smallestSegmentIt->first;

	size_t newOffset = offset + size;
	size_t newSize = smallestSegmentIt->second.Size - size;

	_segmentsBySize.erase(smallestSegmentItIt);
	_segmentsByOffset.erase(smallestSegmentIt);
	if (newSize > 0)
		AddNewSegment(newOffset, newSize);

	// TODO:
	_maxAvailableSpace = _maxAvailableSpace < newSize ? newSize : _maxAvailableSpace;

	return offset;
}

void PoolAllocator::Free(size_t offset, size_t size) noexcept
{
	std::lock_guard mutex(_allocatorMutex);

	auto nextBlockIt = _segmentsByOffset.upper_bound(offset);
	auto prevBlockIt = nextBlockIt;
	if (prevBlockIt != _segmentsByOffset.begin())
		--prevBlockIt;
	else
		prevBlockIt = _segmentsByOffset.end();

    size_t newSize, newOffset;
    if (prevBlockIt != _segmentsByOffset.end() && offset == prevBlockIt->first + prevBlockIt->second.Size)
    {
        // PrevBlock.offset           offset
        // |                          |
        // |<-----PrevBlock.Size----->|<------Size-------->|
        //
        newSize = prevBlockIt->second.Size + size;
        newOffset = prevBlockIt->first;

        if (nextBlockIt != _segmentsByOffset.end() && offset + size == nextBlockIt->first)
        {
            // PrevBlock.offset           offset               NextBlock.offset 
            // |                          |                    |
            // |<-----PrevBlock.Size----->|<------Size-------->|<-----NextBlock.Size----->|
            //
            newSize += nextBlockIt->second.Size;
            _segmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _segmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
            // Delete the range of two blocks
            ++nextBlockIt;
            _segmentsByOffset.erase(prevBlockIt, nextBlockIt);
        }
        else
        {
            // PrevBlock.offset           offset                       NextBlock.offset 
            // |                          |                            |
            // |<-----PrevBlock.Size----->|<------Size-------->| ~ ~ ~ |<-----NextBlock.Size----->|
            //
            _segmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _segmentsByOffset.erase(prevBlockIt);
        }
    }
    else if (nextBlockIt != _segmentsByOffset.end() && offset + size == nextBlockIt->first)
    {
        // PrevBlock.offset                   offset               NextBlock.offset 
        // |                                  |                    |
        // |<-----PrevBlock.Size----->| ~ ~ ~ |<------Size-------->|<-----NextBlock.Size----->|
        //
        newSize = size + nextBlockIt->second.Size;
        newOffset = offset;
        _segmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
        _segmentsByOffset.erase(nextBlockIt);
    }
    else
    {
        // PrevBlock.offset                   offset                       NextBlock.offset 
        // |                                  |                            |
        // |<-----PrevBlock.Size----->| ~ ~ ~ |<------Size-------->| ~ ~ ~ |<-----NextBlock.Size----->|
        //
        newSize = size;
        newOffset = offset;
    }

    AddNewSegment(newOffset, newSize);
}

void PoolAllocator::AddNewSegment(size_t offset, size_t size) noexcept
{
	auto newSegmentIt = _segmentsByOffset.emplace(offset, size);
	auto orderIt = _segmentsBySize.emplace(size, newSegmentIt.first);
	newSegmentIt.first->second.OrderBySizeIt = orderIt;
}