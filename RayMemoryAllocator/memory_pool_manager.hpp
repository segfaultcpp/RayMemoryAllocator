#pragma once
#include <vector>
#include <algorithm>

template<typename MemoryPool>
class MemoryPoolManager
{
	using Pools = std::vector<MemoryPool>;

private:
	size_t _preferredPoolSize;
	Pools _listOfPools;

public:
	~MemoryPoolManager()
	{
		Destroy();
	}

	void Initialize(size_t preferredSize) noexcept;
	void Destroy() noexcept;

	MemoryPool& RequestPool(size_t requestedSize) noexcept;

	void SetPreferredPoolSize(size_t size) noexcept
	{
		_preferredPoolSize = size;
	}

private:
	MemoryPool& CreatePool(size_t requestedSize) noexcept;

};

template<typename MemoryPool>
void MemoryPoolManager<MemoryPool>::Initialize(size_t preferredSize) noexcept
{
	_preferredPoolSize = preferredSize;

	_listOfPools.emplace_back(preferredSize, 0);
}

template<typename MemoryPool>
void MemoryPoolManager<MemoryPool>::Destroy() noexcept
{
	if (_listOfPools.size() != 0)
	{
		for (size_t i = 0; i < _listOfPools.size(); ++i)
			_listOfPools[i].Destroy();
	}
}

template<typename MemoryPool>
MemoryPool& MemoryPoolManager<MemoryPool>::RequestPool(size_t requestedSize) noexcept
{
	auto it = std::find_if(_listOfPools.begin(), _listOfPools.end(), [&requestedSize](MemoryPool& pool)
	{
		return pool.IsEnough(requestedSize);
	});

	if (it != _listOfPools.end())
		return *it;

	size_t size = requestedSize > _preferredPoolSize ? requestedSize : _preferredPoolSize;
	return CreatePool(size);
}

template<typename MemoryPool>
MemoryPool& MemoryPoolManager<MemoryPool>::CreatePool(size_t requestedSize) noexcept
{
	_listOfPools.emplace_back(requestedSize, _listOfPools.size());
	return _listOfPools[_listOfPools.size() - 1];
}