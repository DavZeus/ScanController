#include <fmt/core.h>

#include "sc_options.h"
#include "scan_handler.h"

int main(int argc, char *argv[]) {
  std::locale::global(std::locale(""));
  try {
    sc_options options;
    options.parse(argc, argv);
    const auto com =
        "COM" +
        std::to_string(options.get_value(sc_options::com_switch).as<short>());

    scan_handler scanner(com);
    scanner.start();
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
}
