#include "program_arguments.h"

auto sc::program_arguments::get_allowed_options() const
    -> const boost::program_options::options_description & {
  return description_;
}

auto sc::program_arguments::parse(int argc, char *argv[]) -> void {
  store(parse_command_line(argc, argv, description_), vm_);
  notify(vm_);
}

auto sc::program_arguments::check_value(const std::string_view &key) const
    -> size_t {
  return vm_.count(key.data());
}

auto sc::program_arguments::get_value(const std::string_view &key) const
    -> boost::program_options::variable_value {
  return vm_.at(key.data());
}

sc::program_arguments::program_arguments()
    : description_("Allowed program program_arguments") {
  description_.add_options()(help_switch.data(), help_desc.data())(
      com_switch.data(),
      boost::program_options::value<decltype(com_value)>()->default_value(
          com_value),
      com_desc.data())(
      steps_switch.data(),
      boost::program_options::value<decltype(steps_value)>()->default_value(
          steps_value),
      steps_desc.data())(distance_switch.data(),
                         boost::program_options::value<float>(),
                         distance_desc.data())(
      cut_switch.data(),
      boost::program_options::value<decltype(cut_value)>()->default_value(
          cut_value),
      cut_desc.data())(save_switch.data(), save_desc.data())(
      load_switch.data(), boost::program_options::value<std::string>(),
      load_desc.data())(
      method_switch.data(),
      boost::program_options::value<decltype(method_value)>()->default_value(
          method_value),
      method_desc.data())(
      additional_switch.data(),
      boost::program_options::value<decltype(method_value)>()->default_value(7),
      additional_desc.data());
}
