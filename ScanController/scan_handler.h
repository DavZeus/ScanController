#pragma once

#include "boost_winver.h"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <string>

namespace sc {

class scan_handler {
  using vertical = std::vector<std::pair<float, float>>;

  constexpr static int min_step = 1;
  constexpr static int step_count = 200;
  constexpr static int baud_rate = 9600;
  constexpr static int char_size = 8;
  constexpr static unsigned restart_delay = 5;

  constexpr static std::string_view data_points_file{"data_points.txt"};
  constexpr static std::string_view scan_file{"afdata1.txt"};
  constexpr static std::string_view scanner_path{"scanner\\PMEXE.exe"};

  [[noreturn]] static auto win_error() -> void;

  static auto set_serial_parameters(boost::asio::serial_port &port) -> void;

  template <class T>
  static auto send_to_board(boost::asio::serial_port &port, T &message) -> void;

  static auto check_board_response(boost::asio::serial_port &port) -> bool;

  static auto make_scan() -> void;
  static auto remove_scan_file() -> void;
  [[nodiscard]] static auto process_scan_file() -> vertical;

  std::string com_port_;

public:
  explicit scan_handler(std::string com_port);
  [[nodiscard]] auto start() -> std::vector<vertical>;
};

template <class T>
auto scan_handler::send_to_board(boost::asio::serial_port &port, T &message)
    -> void {
  // Output buffer
  boost::asio::streambuf o_buf;

  // Stream to write message
  std::ostream os(&o_buf);

  // Write message to buffer
  os << message;

  // Send message to arduino board
  write(port, o_buf.data());
}

} // namespace sc
