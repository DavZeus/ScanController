#include "scan_handler.h"

#include <boost/asio/read.hpp>

auto sc::scan_handler::set_serial_parameters(boost::asio::serial_port &port)
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

auto sc::scan_handler::check_board_response(boost::asio::serial_port &port)
    -> void {
  // Input/response buffer
  boost::asio::streambuf i_buf;

  if (read(port, i_buf.prepare(sizeof(uint8_t))) == 0) {
    throw std::exception("No arduino response");
  }
  i_buf.consume(sizeof(uint8_t));
}

sc::scan_handler::scan_handler(std::string com_port, float cut_level,
                               unsigned int step_count)
    : com_port_(std::move(com_port)), step_count_(step_count),
      cut_level_(cut_level) {}
