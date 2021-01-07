#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "sc_winver.h"

#include <boost/program_options.hpp>

class sc_options {
  boost::program_options::variables_map vm_;

  constexpr static std::array help_desc = std::to_array("produce help message");
  constexpr static std::array com_desc = std::to_array("COM port number");
  constexpr static std::array distance_desc = std::to_array("camera distance");
  constexpr static std::array cut_desc =
      std::to_array("model cut level in pixels");

public:
  constexpr static std::array help_switch = std::to_array("help");
  constexpr static std::array com_switch = std::to_array("com");
  constexpr static std::array distance_switch = std::to_array("dist");
  constexpr static std::array cut_switch = std::to_array("cut");

  auto parse(int argc, char *argv[]) -> void;
  template <size_t N>
  [[nodiscard]] auto check_value(const std::array<char, N> &key) const
      -> size_t;
  template <size_t N>
  [[nodiscard]] auto get_value(const std::array<char, N> &key) const
      -> boost::program_options::variable_value;
};

template <size_t N>
auto sc_options::check_value(const std::array<char, N> &key) const -> size_t {
  return vm_.count(key.data());
}

template <size_t N>
auto sc_options::get_value(const std::array<char, N> &key) const
    -> boost::program_options::variable_value {
  return vm_.at(key.data());
}
