#include "converter.h"

#include "arg_parser.h"

auto converter::convert() -> void
{
	auto scan_file_path = arg_parser::switch_values.at(arg_parser::scan_switch);
	std::ifstream scan_file();
}
