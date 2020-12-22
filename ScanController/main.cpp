#include <iostream>
#include <fmt/core.h>

#include "runner.h"


int main(int argc, char* argv[])
{
	std::locale::global(std::locale(""));
	try
	{
		runner::parse_arguments(argc, argv);
		runner::start();
		fmt::print("Scan in file {}\n", runner::result_file);
	}
	catch (const std::exception& ex)
	{
		fmt::print("\n{}\n", ex.what());
	}
}
