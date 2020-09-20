#include "pch.hpp"
#include <Windows.h>
#include <iostream>

#define WIN_MAIN 1


#if WIN_MAIN

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int bShowCmd)
{


	return 0;
}

#else

int main()
{
	return 0;
}


#endif