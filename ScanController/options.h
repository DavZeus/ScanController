#pragma once

#include <boost/program_options.hpp>

class options
{
	boost::program_options::variables_map vm_;


	constexpr static char help_switch[] = "help";
	constexpr static char com_switch[] = "com";
public:
	auto parse_options(int argc, char* argv[]);
};

