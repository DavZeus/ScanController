#pragma once

#include <boost/asio.hpp>
#include <string>

class scan_handler {
  using vertical_points = std::vector<std::pair<float, float>>;

  constexpr static int min_step = 1;
  constexpr static int step_count = 200;
  constexpr static int baud_rate = 9600;
  constexpr static int char_size = 8;
  constexpr static unsigned restart_delay = 5;

  std::string result_file_ = "full_result.txt";
  std::string scan_file_ = "afdata1.txt";
  std::string scanner_path_ = "scanner\\PMEXE.exe";

  [[noreturn]] static auto win_error() -> void;

  static auto set_serial_parameters(boost::asio::serial_port &port) -> void;

  template <class T>
  static auto send_to_board(boost::asio::serial_port &port, T &message) -> void;
  static auto check_board_response(boost::asio::serial_port &port) -> bool;

  auto make_scan() const -> void;
  auto remove_scan_file() const -> void;
  auto process_scan_file() const -> vertical_points;

  std::string com_port_;

public:
  explicit scan_handler(std::string com_port);
  [[nodiscard]] auto start() -> std::vector<vertical_points>;
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
