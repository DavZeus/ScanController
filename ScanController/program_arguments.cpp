#include "program_arguments.h"

auto sc::program_arguments::get_allowed_options() const
    -> const boost::program_options::options_description & {
  return description_;
}

auto sc::program_arguments::parse(int argc, char *argv[]) -> void {
  const auto desc = get_allowed_options();
  store(parse_command_line(argc, argv, desc), vm_);
  notify(vm_);
}

auto sc::program_arguments::check_value(const std::string_view &key) const -> size_t {
  return vm_.count(key.data());
}

auto sc::program_arguments::get_value(const std::string_view &key) const
    -> boost::program_options::variable_value {
  return vm_.at(key.data());
}

sc::program_arguments::program_arguments() : description_("Allowed program program_arguments") {
  description_.add_options()(help_switch.data(), help_desc.data())(
      com_switch.data(),
      boost::program_options::value<short>()->default_value(
          static_cast<short>(4)),
      com_desc.data())(
      steps_switch.data(),
      boost::program_options::value<unsigned>()->default_value(200U),
      steps_desc.data())(distance_switch.data(),
                         boost::program_options::value<float>(),
                         distance_desc.data())(
      cut_switch.data(),
      boost::program_options::value<float>()->default_value(0.F),
      cut_desc.data())(save_switch.data(), save_desc.data())(
      load_switch.data(), boost::program_options::value<std::string>(),
      load_desc.data());
}
