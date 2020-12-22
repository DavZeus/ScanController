#include "options.h"

auto options::parse_options(int argc, char* argv[])
{
	using namespace boost::program_options;
	options_description description("Allowed options");
	description.add_options()
		(help_switch, "produce help message")
		(com_switch, value<uint8_t>()->default_value(static_cast<uint8_t>(4)));
	store(parse_command_line(argc, argv, description), vm_);
	notify(vm_);
}
