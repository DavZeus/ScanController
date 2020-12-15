#include "arg_parser.h"


auto arg_parser::find_switch(const std::string& switch_string, int argc, char* argv[]) -> char*
{
	for (int i = 1; i < argc; ++i)
	{
		const auto res = std::char_traits<char>::compare(argv[i], switch_string.c_str(), switch_string.length());
		if (res == 0 && sizeof(argv[i]) > switch_string.length())
		{
			return argv[i] + switch_string.length();
		}
	}
	return nullptr;
}

auto arg_parser::parse_arguments(int argc, char* argv[]) -> void
{
	for (const auto& element : switches)
	{
		const std::string res = find_switch(element, argc, argv);
		if (!res.empty())
		{
			switch_values.insert(element, res);
		}
	}
}