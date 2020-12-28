#include "sc_options.h"

auto sc_options::parse(int argc, char *argv[]) -> void {
  boost::program_options::options_description description("Allowed sc_options");
  description.add_options()(help_switch, help_desc)(
      com_switch,
      boost::program_options::value<short>()->default_value(
          static_cast<short>(4)),
      com_desc)(distance_switch, boost::program_options::value<float>(),
                distance_desc)(
      cut_switch, boost::program_options::value<float>()->default_value(0.F),
      cut_desc);
  store(parse_command_line(argc, argv, description), vm_);
  notify(vm_);
}

auto sc_options::check_value(const std::string &key) const -> size_t {
  return vm_.count(key);
}

auto sc_options::get_value(const std::string &key) const
    -> boost::program_options::variable_value {
  return vm_.at(key);
}
