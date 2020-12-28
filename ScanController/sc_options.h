#pragma once

#include <boost/program_options.hpp>

class sc_options {
  boost::program_options::variables_map vm_;

  constexpr static char help_desc[] = "produce help message";
  constexpr static char com_desc[] = "COM port number";
  constexpr static char distance_desc[] = "camera distance";
  constexpr static char cut_desc[] = "model cut level in pixels";

public:
  constexpr static char help_switch[] = "help";
  constexpr static char com_switch[] = "com";
  constexpr static char distance_switch[] = "dist";
  constexpr static char cut_switch[] = "cut";

  auto parse(int argc, char *argv[]) -> void;
  [[nodiscard]] auto check_value(const std::string &key) const -> size_t;
  [[nodiscard]] auto get_value(const std::string &key) const
      -> boost::program_options::variable_value;
};
