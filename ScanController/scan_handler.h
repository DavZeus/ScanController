#pragma once

#include "boost_winver.h"
#include "common_usings.h"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <string>

namespace sc {

class scan_handler {
  constexpr static int min_step = 1;
  constexpr static int baud_rate = 9600;
  constexpr static int char_size = 8;
  constexpr static unsigned restart_delay = 5;

  constexpr static std::string_view data_points_file{"data_points.txt"};
  constexpr static std::string_view scan_file{"afdata1.txt"};
  constexpr static std::string_view scanner_path{"scanner\\PMEXE.exe"};

  // Form error string by winapi and throw it
  [[noreturn]] static auto win_error() -> void;

  // Set parameters of serial port
  static auto set_serial_parameters(boost::asio::serial_port &port) -> void;

  // Send message via serial port
  template <class T>
  static auto send_to_board(boost::asio::serial_port &port, T &message) -> void;

  // Check if response acquired via serial port
  static auto check_board_response(boost::asio::serial_port &port) -> bool;

  // Perform scan
  static auto make_scan() -> void;

  // Delete scan file
  static auto remove_scan_file() -> void;
  // Read scan file
  [[nodiscard]] auto process_scan_file() -> vertical;

  // Chosen serial port
  const std::string com_port_;
  // Number of drive steps
  const unsigned int step_count_;
  // Bottom points cut level (pixels)
  const float cut_level_;

public:
  explicit scan_handler(std::string com_port, float cut_level = 0,
                        unsigned int step_count = 200);

  // Performs model scanning
  [[nodiscard]] auto start() -> data_points;
};

template <class T>
auto scan_handler::send_to_board(boost::asio::serial_port &port, T &message)
    -> void {
  // Output buffer
  boost::asio::streambuf o_buf;

  // Stream to write_mesh message
  std::ostream os(&o_buf);

  // Write message to buffer
  os << message;

  // Send message to arduino board
  write(port, o_buf.data());
}

} // namespace sc
