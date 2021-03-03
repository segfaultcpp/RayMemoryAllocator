#include "pch.hpp"
#include <Windows.h>
#include <iostream>

#include "example_allocator.hpp"
#include "pool_allocator.hpp"
#include "fast_pool_allocator.hpp"

#define WIN_MAIN 1

#if WIN_MAIN

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int bShowCmd)
{
	Timer::Start();

	ExampleAllocator allocator;
#if 0
	
	{
		Timer allocating{ "global_space::main - allocating" };
		s32* a = static_cast<s32*>(allocator.Allocate(sizeof(s32) * 1000000));
		allocator.Free(a);
	}
#else
	constexpr u32 elementsCount = 1000;
	s32* arr[elementsCount];

	// ============================= MALLOC ============================= //
	{
		Timer timer("global_space::main-malloc");

		for (size_t i = 0; i < elementsCount; ++i)
			arr[i] = static_cast<s32*>(malloc(sizeof(s32)));

		for (size_t i = 0; i < elementsCount; ++i)
			free(arr[i]);
	}

	// ============================= EXAMPLE ALLOCATOR ============================= //
	{
		Timer timer("global_space::main-example allocator");

		for (size_t i = 0; i < elementsCount; ++i)
			arr[i] = static_cast<s32*>(allocator.Allocate(sizeof(s32)));

		for (size_t i = 0; i < elementsCount; ++i)
			allocator.Free(arr[i]);
	}

	
#endif
	Timer::Stop();

	return 0;
}

#else

int main()
{
	return 0;
}


#endif