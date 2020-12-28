

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include "scan_handler.h"

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <shellapi.h>
#include <utility>

auto scan_handler::win_error() -> void {
  const auto error_message_id = GetLastError();
  LPSTR message_buffer = nullptr;
  const size_t size = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, error_message_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      LPSTR(&message_buffer), 0, nullptr);
  const std::string error(message_buffer, size);
  LocalFree(message_buffer);
  throw std::exception(error.c_str());
}

auto scan_handler::set_serial_parameters(boost::asio::serial_port &port)
    -> void {
  using namespace boost::asio;

  port.set_option(serial_port_base::baud_rate(baud_rate));
  port.set_option(
      serial_port_base::flow_control(serial_port_base::flow_control::none));
  port.set_option(serial_port_base::parity(serial_port_base::parity::none));
  port.set_option(
      serial_port_base::stop_bits(serial_port_base::stop_bits::one));
  port.set_option(serial_port_base::character_size(char_size));
}

auto scan_handler::check_board_response(boost::asio::serial_port &port)
    -> bool {
  // Input buffer / Response
  boost::asio::streambuf i_buf;

  if (read(port, i_buf.prepare(sizeof(uint8_t))) == 0) {
    return false;
  }
  i_buf.consume(sizeof(uint8_t));

  return true;
}

auto scan_handler::make_scan() -> void {
  STARTUPINFOA si{};
  PROCESS_INFORMATION pi{};
  std::string sc_path = scanner_path.data();
  // ZeroMemory(&si, sizeof si);
  si.cb = sizeof si;
  // ZeroMemory(&pi, sizeof pi);
  if (!CreateProcessA(nullptr, sc_path.data(), nullptr, nullptr, FALSE, 0,
                      nullptr, nullptr, &si, &pi)) {
    win_error();
  }
  if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_OBJECT_0) {
    win_error();
  }
  // TODO: Add security policies to be able to get exit code
  /*DWORD code;
  if (!GetExitCodeProcess(pi.hProcess, &code))
  {
                  win_error();
  }
  if (code != 0)
  {
                  throw std::exception(fmt::format("Scanner exit code is {}\n",
  code).c_str());
  }*/
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void scan_handler::remove_scan_file() {
  if (std::filesystem::exists(scan_file.data())) {
    std::filesystem::remove(scan_file.data());
  }
}

auto scan_handler::process_scan_file() -> vertical {
  vertical points;

  // Open scan file
  std::ifstream in_file(scan_file.data(), std::ios::in);
  if (!in_file) {
    throw std::exception("There is no scan file!");
  }

  while (!in_file.eof()) {
    static float p_x;
    static float p_z;
    in_file >> p_x >> p_z;
    points.emplace_back(p_x, p_z);
  }

  in_file.close();

  // Remove scan file
  remove_scan_file();

  return points;
}

scan_handler::scan_handler(std::string com_port)
    : com_port_(std::move(com_port)) {}

auto scan_handler::start() -> std::vector<vertical> {
  std::vector<vertical> verticals;

  // Print status
  fmt::print("Program starting...\n");

  // Initialize serial port
  boost::asio::io_service io_service;
  boost::asio::serial_port port(io_service, com_port_);
  set_serial_parameters(port);

  // Print status
  fmt::print("Connecting to arduino board via {}...\n", com_port_);

  // Wait arduino restart
  std::this_thread::sleep_for(std::chrono::seconds(restart_delay));

  // Print status
  fmt::print("Sending settings...\n");

  // Send one turn step count to arduino
  send_to_board(port, step_count);

  // Remove old scan_file
  remove_scan_file();

  // Check board response
  if (!check_board_response(port)) {
    throw std::exception("No arduino response");
  }

  // Print status
  fmt::print("Scanning starts now.\n");

  // Set step count left
  auto steps_left = step_count;

  // Scan loop
  while (steps_left--) {
    // Print status
    fmt::print("\rStep {} of {}", step_count - steps_left, step_count);

    make_scan();

    verticals.emplace_back(process_scan_file());

    // Send signal to make step
    send_to_board(port, min_step);

    // Check board response
    if (!check_board_response(port)) {
      throw std::exception("No arduino response");
    }
  }

  fmt::print("\n");

  // Close serial port
  if (port.is_open()) {
    port.close();
  }

  return verticals;
}
