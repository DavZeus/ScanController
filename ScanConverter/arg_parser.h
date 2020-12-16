#pragma once

#include <map>
#include <string>
#include <vector>

class arg_parser
{
public:
	//Named variables of switches
	constexpr static char scan_switch[]   = "-scan_file=";
	constexpr static char save_switch[]   = "-save_file=";
	constexpr static char method_switch[] = "-method=";
	//Switches container
private:
	inline const static std::vector<std::string> switches {
		save_switch,
		save_switch,
		method_switch
	};

	//TODO: Add list of supported file types
public:
	//Parsed switch values
	inline static std::map<std::string, std::string> switch_values;
private:
	//Switch search in program arguments
	static auto find_switch(const std::string& switch_string, int argc, char* argv[]) -> std::string;

public:
	//Default values
	inline static std::string default_scan_file{ "full result.txt" };
	inline static std::string default_save_file{ "model.stl" };

	//Parse of program arguments
	static auto parse_arguments(int argc, char* argv[]) -> void;
};

