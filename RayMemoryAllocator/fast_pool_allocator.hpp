#pragma once
#include "pool_allocator.hpp"

/// <summary>
/// Unlike PoolAllocator FastPoolAllocator doesn't look for apropriate memory fragment each allocation call.
/// It keeps vast memory fragment and allocates memory from it.
/// When memory fragment runs out FastPoolAllocator finds max memory block in fragments list
/// </summary>
class FastPoolAllocator final :
    public PoolAllocator
{
private:
    MemoryFragmentsByOffsetMap::iterator _currentFragment;
    size_t _currentFragmentOffset;
    size_t _currentFragmentEnd;

public:
    explicit FastPoolAllocator(size_t poolSize) noexcept
        : PoolAllocator(poolSize)
        , _currentFragment(_fragmentsByOffset.begin())
        , _currentFragmentOffset(_currentFragment->first)
        , _currentFragmentEnd(_currentFragmentOffset + _currentFragment->second.Size)
    {}

    /*FastPoolAllocator(FastPoolAllocator&& rhs) = default;
    FastPoolAllocator& operator = (FastPoolAllocator&& rhs) = default;*/

public:
    NODISCARD size_t Allocate(size_t size) noexcept override;

};

