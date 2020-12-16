#include <iostream>
#include <fmt/core.h>

#include "runner.h"


int main(int argc, char* argv[])
{
	std::cout.imbue(std::locale(""));
	try
	{
		runner::parse_arguments(argc, argv);
		runner::start();
		fmt::print("Scan result in file {}\n", runner::save_path);
	}
	catch (const std::exception& ex)
	{
		fmt::print(ex.what());
	}
	
}
