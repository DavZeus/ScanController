#include "sc_options.h"

auto sc_options::parse(int argc, char *argv[]) -> void {
  boost::program_options::options_description description("Allowed sc_options");
  description.add_options()(help_switch.data(), help_desc.data())(
      com_switch.data(),
      boost::program_options::value<short>()->default_value(
          static_cast<short>(4)),
      com_desc.data())(distance_switch.data(),
                       boost::program_options::value<float>(),
                       distance_desc.data())(
      cut_switch.data(),
      boost::program_options::value<float>()->default_value(0.F),
      cut_desc.data());
  store(parse_command_line(argc, argv, description), vm_);
  notify(vm_);
}
