#include "sc_options.h"

auto sc_options::parse(int argc, char* argv[]) -> void
{
	using namespace boost::program_options;
	options_description description("Allowed sc_options");
	description.add_options()
		(help_switch, "produce help message")
		(com_switch, value<uint8_t>()->default_value(static_cast<uint8_t>(4)));
	store(parse_command_line(argc, argv, description), vm_);
	notify(vm_);
}

auto sc_options::get_value(const std::string& key) const
{
	return vm_.at(key);
}
