#pragma once

#include "sc_defs.h"

#include <boost/program_options.hpp>

namespace sc {

class program_arguments {

  boost::program_options::options_description description_;

  boost::program_options::variables_map vm_;

  // Descriptions
  constexpr static std::string_view help_desc{"produce help message"};
  constexpr static std::string_view com_desc{
      "COM port number of Arduino board"};
  constexpr static std::string_view steps_desc{
      "number of steps required for one revolution of drive"};
  constexpr static std::string_view distance_desc{"camera distance"};
  constexpr static std::string_view cut_desc{
      "model bottom cut level in pixels"};
  constexpr static std::string_view save_desc{"save point data in file"};
  constexpr static std::string_view load_desc{
      "use file with point data (one file)"};
  constexpr static std::string_view method_desc{
      "reconstruction method: 0 - advancing front, 1 - scale space, 2 - "
      "Poisson"};
  constexpr static std::string_view additional_desc{
      "additional preprocess point cloud: 1 - simplification, 2 - remove "
      "outliers, 4 - smoothing and A bitwise AND values"};

  // Default values

  inline static float com_value = 4.F;
  inline static unsigned steps_value = 200U;
  inline static float cut_value = 0.F;
  inline static unsigned method_value = 0;
  inline static unsigned additional_value = 0;

public:
  // Switches
  constexpr static std::string_view help_switch{"help"};
  constexpr static std::string_view com_switch{"com"};
  constexpr static std::string_view steps_switch{"steps"};
  constexpr static std::string_view distance_switch{"dist"};
  constexpr static std::string_view cut_switch{"cut"};
  constexpr static std::string_view save_switch{"save"};
  constexpr static std::string_view load_switch{"load"};
  constexpr static std::string_view method_switch{"method"};
  constexpr static std::string_view additional_switch{"add"};

  // Get program program_arguments description (for console output mainly)
  [[nodiscard]] auto get_allowed_options() const
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
