#include <iostream>
#include <fmt/core.h>

#include "scan_handler.h"
#include "sc_options.h"

int main(int argc, char* argv[])
{
	std::locale::global(std::locale(""));
	try
	{
		sc_options options;
		options.parse(argc, argv);

		scan_handler scanner;
		scanner.start();

	}
	catch (const std::exception& ex)
	{
		fmt::print(stderr, "\n{}\n", ex.what());
	}
}
