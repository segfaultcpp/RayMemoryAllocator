#include "pch.hpp"
#include "timer.hpp"

std::ofstream Timer::_outFile{};
std::string Timer::_fileName = "timer.txt";
bool Timer::_bOpened = false;

Timer::Timer(const std::string& name) noexcept
#ifndef _DEBUG
{}
#else
	: _name(name)
{
	_start = system_clock::now();
}
#endif

Timer::~Timer()
#ifndef _DEBUG
{}
#else
{
	_end = system_clock::now();

	auto result = _end - _start;
	_outFile << _name << ": " << duration_cast<milliseconds>(result).count() << " ms\t" << duration_cast<nanoseconds>(result).count() << " ns\n";
}
#endif

void Timer::Start() noexcept
{
	_outFile.open(_fileName, std::ios_base::out);
	_outFile.clear();
}

void Timer::Stop() noexcept
{
	_outFile.close();
}
