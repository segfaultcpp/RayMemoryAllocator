#include "pch.hpp"
#include "pool_allocator.hpp"

size_t PoolAllocator::Allocate(size_t size) noexcept
{
	//std::lock_guard mutex(_allocatorMutex);
    Timer timer{ "PoolAllocator::Allocate" };

    MemoryFragmentsBySizeMap::iterator smallestFragmentItIt;
    {
        Timer searchingOfFragment{ "PoolAllocator::Allocate - searching of fragment" };
        smallestFragmentItIt = _fragmentsBySize.lower_bound(size);
        if (smallestFragmentItIt == _fragmentsBySize.end())
            return (~0u);
    }

	auto smallestFragmentIt = smallestFragmentItIt->second;
	size_t offset = smallestFragmentIt->first;

	size_t newOffset = offset + size;
	size_t newSize = smallestFragmentIt->second.Size - size;

    {
        Timer deletingFragments{ "PoolAllocator::Allocate - deleting fragments" };
        _fragmentsBySize.erase(smallestFragmentItIt);
        _fragmentsByOffset.erase(smallestFragmentIt);
    }

    if (newSize > 0)
		AddNewFragment(newOffset, newSize);

    auto lastElement = _fragmentsBySize.end();
    _maxAvailableSpace = _fragmentsBySize.size() != 0 ? 
       (--lastElement)->first : 0;

	return offset;
}

void PoolAllocator::Free(size_t offset, size_t size) noexcept
{
	//std::lock_guard mutex(_allocatorMutex);
    Timer timer{ "PoolAllocator::Free" };

    MemoryFragmentsByOffsetMap::iterator nextBlockIt, prevBlockIt;
    {
        Timer searchingOfFragments{ "PoolAllocator::Free - searching of fragments" };
        nextBlockIt = _fragmentsByOffset.upper_bound(offset);
        prevBlockIt = nextBlockIt;
        if (prevBlockIt != _fragmentsByOffset.begin())
            --prevBlockIt;
        else
            prevBlockIt = _fragmentsByOffset.end(); 
    }

    size_t newSize, newOffset;
    if (prevBlockIt != _fragmentsByOffset.end() && offset == prevBlockIt->first + prevBlockIt->second.Size)
    {
        // PrevBlock.offset           offset
        // |                          |
        // |<-----PrevBlock.Size----->|<------Size-------->|
        //
        newSize = prevBlockIt->second.Size + size;
        newOffset = prevBlockIt->first;

        if (nextBlockIt != _fragmentsByOffset.end() && offset + size == nextBlockIt->first)
        {
            // PrevBlock.offset           offset               NextBlock.offset 
            // |                          |                    |
            // |<-----PrevBlock.Size----->|<------Size-------->|<-----NextBlock.Size----->|
            //
            newSize += nextBlockIt->second.Size;
            _fragmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _fragmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
            // Delete the range of two blocks
            ++nextBlockIt;
            _fragmentsByOffset.erase(prevBlockIt, nextBlockIt);
        }
        else
        {
            // PrevBlock.offset           offset                       NextBlock.offset 
            // |                          |                            |
            // |<-----PrevBlock.Size----->|<------Size-------->| ~ ~ ~ |<-----NextBlock.Size----->|
            //
            _fragmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _fragmentsByOffset.erase(prevBlockIt);
        }
    }
    else if (nextBlockIt != _fragmentsByOffset.end() && offset + size == nextBlockIt->first)
    {
        // PrevBlock.offset                   offset               NextBlock.offset 
        // |                                  |                    |
        // |<-----PrevBlock.Size----->| ~ ~ ~ |<------Size-------->|<-----NextBlock.Size----->|
        //
        newSize = size + nextBlockIt->second.Size;
        newOffset = offset;
        _fragmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
        _fragmentsByOffset.erase(nextBlockIt);
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

    AddNewFragment(newOffset, newSize);

    auto lastElement = _fragmentsBySize.end();
    _maxAvailableSpace = (--lastElement)->first;
}

void PoolAllocator::AddNewFragment(size_t offset, size_t size) noexcept
{
    Timer timer("PoolAllocator::AddNewFragment");

	auto newfragmentIt = _fragmentsByOffset.emplace(offset, size);
	auto orderIt = _fragmentsBySize.emplace(size, newfragmentIt.first);
	newfragmentIt.first->second.OrderBySizeIt = orderIt;
}