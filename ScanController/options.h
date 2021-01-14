#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "boost_winver.h"

#include <boost/program_options.hpp>

namespace sc {

class options {
  boost::program_options::options_description description_;
  boost::program_options::variables_map vm_;

  constexpr static std::string_view help_desc{"produce help message"};
  constexpr static std::string_view com_desc{"COM port number"};
  constexpr static std::string_view distance_desc{"camera distance"};
  constexpr static std::string_view cut_desc{"model cut level in pixels"};

public:
  constexpr static std::string_view help_switch{"help"};
  constexpr static std::string_view com_switch{"com"};
  constexpr static std::string_view distance_switch{"dist"};
  constexpr static std::string_view cut_switch{"cut"};
  auto get_allowed_options() const
      -> const boost::program_options::options_description &;
  auto parse(int argc, char *argv[]) -> void;
  [[nodiscard]] auto check_value(const std::string_view &key) const
      -> size_t;
  [[nodiscard]] auto get_value(const std::string_view &key) const
      -> boost::program_options::variable_value;
  options();
};

//template <size_t N>
//auto options::check_value(const std::array<char, N> &key) const -> size_t {
//  return vm_.count(key.data());
//}
//
//template <size_t N>
//auto options::get_value(const std::array<char, N> &key) const
//    -> boost::program_options::variable_value {
//  return vm_.at(key.data());
//}

} // namespace sc
