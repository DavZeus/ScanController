#include "options.h"

auto sc::options::get_allowed_options() const
    -> const boost::program_options::options_description & {
  return description_;
}

auto sc::options::parse(int argc, char *argv[]) -> void {
  const auto desc = get_allowed_options();
  store(parse_command_line(argc, argv, desc), vm_);
  notify(vm_);
}
sc::options::options() : description_("Allowed program options") {
  description_.add_options()(help_switch.data(), help_desc.data())(
      com_switch.data(),
      boost::program_options::value<short>()->default_value(
          static_cast<short>(4)),
      com_desc.data())(distance_switch.data(),
                       boost::program_options::value<float>(),
                       distance_desc.data())(
      cut_switch.data(),
      boost::program_options::value<float>()->default_value(0.F),
      cut_desc.data());
}
