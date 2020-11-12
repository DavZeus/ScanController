#pragma once


#include <string>

class runner
{
	constexpr static char scanner_exe[] = "PMEXE.exe";
	constexpr static char scan_file[]   = "resexe.txt";
	constexpr static char result_file[] = "full_result.txt";

	constexpr static char scan_path_switch[]  = "-scan=";
	constexpr static char save_path_switch[]  = "-save=";
	constexpr static char step_count_switch[] = "-step=";
	constexpr static char com_port_switch[]   = "-com=";

	constexpr static int min_step = 1;

	static auto find_switch(const char* switch_string, int argc, char* argv[]) -> char*;

	static auto make_scan() -> void;

	[[noreturn]] static auto win_error() -> void;

public:
	inline static std::string scan_path{ "scanner\\" };
	inline static std::string save_path{ "" };
	inline static int step_count{ 200 };
	inline static std::string com_port{ "COM" };
	
	static auto parse_arguments(int argc, char* argv[]) -> void;

	static auto start() -> void;
};
