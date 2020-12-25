#pragma once

#include <boost/program_options.hpp>

class sc_options
{
	boost::program_options::variables_map vm_;
public:
	constexpr static char help_switch[] = "help";
	constexpr static char com_switch[] = "com";

	auto parse(int argc, char* argv[]) -> void;
	[[nodiscard]] auto check_value(const std::string& key) const -> size_t;
	[[nodiscard]] auto get_value(const std::string& key) const -> boost::program_options::variable_value;
};

