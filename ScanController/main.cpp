#include <iostream>
#include <fmt/core.h>

#include "runner.h"
#include "sc_options.h"

int main(int argc, char* argv[])
{
	std::locale::global(std::locale(""));
	try
	{
		sc_options options;
		options.parse(argc, argv);

		
		runner::parse_arguments(argc, argv);
		runner::start();
		fmt::print("Scan in file {}\n", runner::result_file);
	}
	catch (const std::exception& ex)
	{
		fmt::print(stderr, "\n{}\n", ex.what());
	}
}
