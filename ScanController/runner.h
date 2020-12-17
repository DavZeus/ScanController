#pragma once


#include <string>

class runner
{
	constexpr static char step_count_switch[]  = "-steps=";
	constexpr static char com_port_switch[]    = "-com=";

	constexpr static int min_step = 1;

	static auto find_switch(const char* switch_string, int argc, char* argv[]) -> char*;

	[[noreturn]] static auto win_error() -> void;

	static auto make_scan() -> void;
	static auto remove_scan_file() -> void;
	static auto process_scan_file(std::ofstream& out_file) -> void;
	
public:
	inline const static std::string scan_file = "afdata1.txt";
	inline const static std::string scan_path = "scanner\\PMEXE.exe";

	constexpr static char result_file[] = "full_result.txt";

	inline static int         step_count{ 200 };
	inline static std::string com_port  { "COM" };
	
	static auto parse_arguments(int argc, char* argv[]) -> void;

	static auto start() -> void;
};
