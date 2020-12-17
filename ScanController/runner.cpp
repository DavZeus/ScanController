#include "runner.h"

#define _WIN32_WINNT 0x0A00

#include <filesystem>
#include <fmt/format.h>
#include <boost/asio.hpp>

#include <shellapi.h>

#include <fstream>

auto runner::find_switch(const char* switch_string, int argc, char* argv[]) -> char*
{
	const auto switch_length = std::char_traits<char>::length(switch_string);
	for (int i = 1; i < argc; ++i)
	{
		const auto res = std::char_traits<char>::compare(argv[i], switch_string, switch_length);
		if (res == 0)
		{
			return argv[i] + switch_length;
		}
	}
	return nullptr;
}

auto runner::win_error() -> void
{
	auto errorMessageID = GetLastError();
	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	std::string error(messageBuffer, size);
	LocalFree(messageBuffer);
	throw std::exception(error.c_str());
}

auto runner::make_scan() -> void
{
	const std::string full_path = scan_path;

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	char* scanner = new char[scan_path.length() + 1];
	std::char_traits<char>::copy(
		scanner, 
		scan_path.c_str(), 
		scan_path.length() + 1);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessA(
		NULL,
		scanner,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		win_error();
	}
	if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		win_error();
	}
	//TODO: Add security policies to be able to get exit code
	/*DWORD code;
	if (!GetExitCodeProcess(pi.hProcess, &code))
	{
		win_error();
	}
	if (code != 0)
	{
		throw std::exception(fmt::format("Scanner exit code is {}\n", code).c_str());
	}*/
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

auto runner::remove_scan_file() -> void
{
	if (std::filesystem::exists(scan_file))
	{
		std::filesystem::remove(scan_file);
	}
}

auto runner::process_scan_file(std::ofstream& out_file) -> void
{
	//Save scan to result file
	std::ifstream in_file(scan_file, std::ios::in);
	out_file << in_file.rdbuf() << "---\n";
	in_file.close();
	
	//Remove scan file 
	remove_scan_file();
}

auto runner::parse_arguments(int argc, char* argv[]) -> void
{
	auto* res = find_switch(step_count_switch, argc, argv);
	if (res) step_count = std::stoi(res);
	res = find_switch(com_port_switch, argc, argv);
	com_port.append(res ? res : "4");
}

auto runner::start() -> void
{
	using namespace boost::asio;

	//Print status
	fmt::print("Program starting...\n");
	
	//Set step count left
	auto steps_left = step_count;

	//Initialize serial port
	io_service io_service;
	serial_port serial(io_service, com_port);
	serial.set_option(serial_port_base::baud_rate(9600));
	serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
	serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
	serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
	serial.set_option(serial_port_base::character_size(8));

	//Print status
	fmt::print("Connecting to arduino board via {}...\n", com_port);
	
	//Wait arduino restart
	std::this_thread::sleep_for(std::chrono::seconds(5));

	//Output buffer / 
	streambuf o_buf;
	std::ostream os(&o_buf);
	//Input buffer / Response
	streambuf i_buf;

	//Print status
	fmt::print("Sending settings...\n");
	
	//Send one turn step count to arduino 
	os << step_count;
	write(serial, o_buf.data());

	//Remove old scan_file
	remove_scan_file();
	
	//Initialize result file
	std::ofstream out_file(result_file, std::ios::out);
	out_file << "x-z\n";
	
	//Check board response
	if (read(serial, i_buf.prepare(sizeof(uint8_t))) == 0)
	{
		throw std::exception("No arduino response\n");
	}
	i_buf.consume(sizeof(uint8_t));

	//Print status
	fmt::print("Scanning starts now.\n");

	//Scan loop
	while (steps_left--)
	{
		//Print status
		fmt::print("\rStep {} of {}", step_count - steps_left, step_count);
		
		make_scan();

		process_scan_file(out_file);
		
		//Send signal to make step
		os << min_step;
		write(serial, o_buf.data());

		//Check board response
		if (read(serial, i_buf.prepare(sizeof(uint8_t))) == 0)
		{
			throw std::exception("No arduino response\n");
		}
		i_buf.consume(sizeof(uint8_t));
		
	}
	
	fmt::print("\n");

	//Close serial port
	if (serial.is_open())
	{
		serial.close();
	}
}
