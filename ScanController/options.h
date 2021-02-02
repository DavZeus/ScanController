#pragma once

#include "boost_winver.h"

#include <boost/program_options.hpp>

namespace sc {

class options {

  boost::program_options::options_description description_;

  boost::program_options::variables_map vm_;

  // Program options description
  constexpr static std::string_view help_desc{"produce help message"};
  constexpr static std::string_view com_desc{"COM port number"};
  constexpr static std::string_view distance_desc{"camera distance"};
  constexpr static std::string_view cut_desc{"model cut level in pixels"};

public:
  // Program option switches
  constexpr static std::string_view help_switch{"help"};
  constexpr static std::string_view com_switch{"com"};
  constexpr static std::string_view distance_switch{"dist"};
  constexpr static std::string_view cut_switch{"cut"};

  // Get program options description (for console output mainly)
  auto get_allowed_options() const
      -> const boost::program_options::options_description &;
  // Process program options
  auto parse(int argc, char *argv[]) -> void;
  // Check if value present by switch
  [[nodiscard]] auto check_value(const std::string_view &key) const -> size_t;
  // Get value by switch
  [[nodiscard]] auto get_value(const std::string_view &key) const
      -> boost::program_options::variable_value;
  options();
};

} // namespace sc
