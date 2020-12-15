#pragma once

#include <fstream>

class converter
{
	std::ifstream scan_file;
	std::ofstream save_file;

	
public:
	auto convert() -> void;
};

