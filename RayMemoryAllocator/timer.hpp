#pragma once
#include <chrono>
#include <fstream>
#include <string>

using namespace std::chrono;

class Timer
{
private:
	time_point<system_clock> _start;
	time_point<system_clock> _end;
	static std::ofstream _outFile;
	static std::string _fileName;
	std::string _name;
	static bool _bOpened;

public:
	explicit Timer(const std::string& name) noexcept;
 	~Timer();

	static void Start() noexcept;
	static void Stop() noexcept;

};
