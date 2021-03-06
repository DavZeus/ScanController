#pragma once

#include "boost_winver.h"

#include <boost/program_options.hpp>

namespace sc {

class program_arguments {

  boost::program_options::options_description description_;

  boost::program_options::variables_map vm_;

  // Program program_arguments description
  constexpr static std::string_view help_desc{"produce help message"};
  constexpr static std::string_view com_desc{"COM port number"};
  constexpr static std::string_view steps_desc{
      "number of steps required for one revolution of drive"};
  constexpr static std::string_view distance_desc{"camera distance"};
  constexpr static std::string_view cut_desc{
      "model bottom cut level in pixels"};
  constexpr static std::string_view save_desc{"save point data in file"};
  constexpr static std::string_view load_desc{
      "use file with point data (one file)"};

public:
  // Program option switches
  constexpr static std::string_view help_switch{"help"};
  constexpr static std::string_view com_switch{"com"};
  constexpr static std::string_view steps_switch{"steps"};
  constexpr static std::string_view distance_switch{"dist"};
  constexpr static std::string_view cut_switch{"cut"};
  constexpr static std::string_view save_switch{"save"};
  constexpr static std::string_view load_switch{"load"};

  // Get program program_arguments description (for console output mainly)
  auto get_allowed_options() const
      -> const boost::program_options::options_description &;
  // Process program program_arguments
  auto parse(int argc, char *argv[]) -> void;
  // Check if value present by switch
  [[nodiscard]] auto check_value(const std::string_view &key) const -> size_t;
  // Get value by switch
  [[nodiscard]] auto get_value(const std::string_view &key) const
      -> boost::program_options::variable_value;
  program_arguments();
};

} // namespace sc
