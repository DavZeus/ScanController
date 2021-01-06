#include <fmt/core.h>

#include "dimension_converter.h"
#include "model_constructor.h"
#include "sc_options.h"
#include "scan_handler.h"

int main(int argc, char *argv[]) {
  std::locale::global(std::locale(""));
  try {
    sc_options options{};
    options.parse(argc, argv);
    const auto com =
        "COM" +
        std::to_string(options.get_value(sc_options::com_switch).as<short>());

    scan_handler scanner(com);
    auto points = scanner.start();
    const dimension_converter converter(500.F);
    auto p = converter.convert(std::move(points));
    const model_constructor constructor(
        model_constructor::methods::advancing_front);
    auto mesh = constructor.make_mesh(p);

  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
}
