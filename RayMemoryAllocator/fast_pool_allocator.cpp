#include "pch.hpp"
#include "fast_pool_allocator.hpp"
#include <cassert>

size_t FastPoolAllocator::Allocate(size_t size) noexcept
{
	Timer allocate("FastPoolAllocator::Allocate");

	if (_currentFragmentOffset + size > _currentFragmentEnd)
	{
		Timer updatingFragment("FastPoolAllocator::Allocate - updating fragment");

		_fragmentsByOffset.erase(_currentFragment);
		_fragmentsBySize.erase(_currentFragment->second.OrderBySizeIt);

		AddNewFragment(_currentFragmentOffset, _currentFragmentEnd - _currentFragmentOffset);

		_currentFragment = _fragmentsBySize.begin()->second;
		if (_currentFragment == _fragmentsBySize.end()->second)
		{
			return POOL_ALLOCATOR_ERROR;
		}

		_currentFragmentOffset = _currentFragment->first;
		_currentFragmentEnd = _currentFragmentOffset + _currentFragment->second.Size;
	}

	size_t ret = _currentFragmentOffset;
	_currentFragmentOffset += size;
	
	// Since _fragmentsBySize.begin().second == _currentFragment we get size value from second element
	auto it = _fragmentsBySize.begin();
	size_t currentSize = _currentFragmentEnd - _currentFragmentOffset;

	if ((++it) != _fragmentsBySize.end())
	{
		if (it->first > currentSize)
		{
			_maxAvailableSpace = it->first;
		}
		else
		{
			_maxAvailableSpace = currentSize;
		}
	}
	else
	{
		_maxAvailableSpace = currentSize;
	}

	return ret;
}
