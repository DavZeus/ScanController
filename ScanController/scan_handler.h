#pragma once

#include "sc_defs.h"

#include "common_usings.h"

#include "camera_handler.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <fmt/core.h>
#include <string>

namespace sc {

class scan_handler {
  inline constexpr static int min_step = 1;
  inline constexpr static int baud_rate = 9600;
  inline constexpr static int char_size = 8;
  inline constexpr static unsigned restart_delay = 5;

  // Set parameters of serial port
  static auto set_serial_parameters(boost::asio::serial_port &port) -> void;

  // Send message via serial port
  template <class T>
  static auto send_to_board(boost::asio::serial_port &port, T &message) -> void;

  // Check if response acquired via serial port
  static auto check_board_response(boost::asio::serial_port &port) -> void;

  // Chosen serial port
  const std::string com_port_;
  // Number of drive steps
  const unsigned int step_count_;
  // Bottom points cut level (pixels)
  const float cut_level_;
  //
  camera_handler camera_;

  template <std::floating_point T> [[nodiscard]] auto make_scan() -> profile<T>;

public:
  explicit scan_handler(std::string com_port, float cut_level = 0,
                        unsigned int step_count = 200);

  // Performs model scanning
  template <std::floating_point T>
  [[nodiscard]] auto start() -> model_profiles<T>;
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

template <std::floating_point T> auto scan_handler::make_scan() -> profile<T> {
  profile<T> result;
  try {
    result = camera_.get_current_profile<T>();
    if (cut_level_ != 0.F) {
      auto it = std::find_if(
          result.begin(), result.end(),
          [this](const data_point<T> &i) { return cut_level_ > i.x; });
      result.erase(it, result.end());
    }
  } catch (const Pylon::RuntimeException &ex) {
    throw std::exception(ex.GetDescription());
  }
  return result;
}

template <std::floating_point T>
auto scan_handler::start() -> model_profiles<T> {
  model_profiles<T> m_profiles;
  m_profiles.reserve(step_count_);

  // Print status
  fmt::print("Program starting...\n");

  // Initialize serial port
  boost::asio::io_service io_service;
  boost::asio::serial_port port(io_service, com_port_);
  set_serial_parameters(port);

  // Print status
  fmt::print("Connecting to arduino board via {}...\n", com_port_);

  // Wait arduino restart after COM connection
  std::this_thread::sleep_for(std::chrono::seconds(restart_delay));

  // Print status
  fmt::print("Sending settings...\n");

  // Send one turn step count to arduino
  send_to_board(port, step_count_);
  // Check board response
  check_board_response(port);

  // Print status
  fmt::print("Initializing cameras...\n");

  // Init camera connection and settings
  try {
    camera_.initialize();
  } catch (const Pylon::RuntimeException &ex) {
    throw std::exception(ex.GetDescription());
  }
  // Set number of steps left
  auto steps_left = step_count_;

  // Print status
  fmt::print("Scanning starts now.\n");

  // Scan loop
  while (steps_left--) {
    // Print status
    fmt::print("\rStep {} of {}", step_count_ - steps_left, step_count_);

    // Obtain profile
    m_profiles.emplace_back(make_scan<T>());

    // Send signal to make step
    send_to_board(port, min_step);
    // Check board response
    check_board_response(port);
  }

  fmt::print("\n");

  // Close serial port
  if (port.is_open()) {
    port.close();
  }

  return m_profiles;
}
} // namespace sc
