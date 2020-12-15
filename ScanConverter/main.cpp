//#include <exception>
#include <fmt/core.h>


#include "arg_parser.h"
#include "converter.h"

int main(int argc, char* argv[])
{
	try
	{
		arg_parser::parse_arguments(argc, argv);
	}
	catch (const std::exception& ex)
	{
		fmt::print(ex.what());
	}
}
