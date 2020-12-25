#include "sc_options.h"

auto sc_options::parse(int argc, char* argv[]) -> void
{
	using namespace boost::program_options;
	options_description description("Allowed sc_options");
	description.add_options()
		(help_switch, "produce help message")
		(com_switch, value<int>()->default_value(static_cast<uint8_t>(4)));
	store(parse_command_line(argc, argv, description), vm_);
	notify(vm_);
}

auto sc_options::check_value(const std::string& key) const -> size_t
{
	return vm_.count(key);
}


auto sc_options::get_value(const std::string& key) const -> boost::program_options::variable_value
{
	return vm_.at(key);
}
