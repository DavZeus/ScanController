#include "dimension_converter.h"
#include "mesh_writer.h"
#include "model_constructor.h"
#include "options.h"
#include "scan_handler.h"

#include <fmt/ostream.h>

int main(int argc, char *argv[]) {
  try {
    sc::options options{};
    options.parse(argc, argv);
    if (options.check_value(sc::options::help_switch)) {
      fmt::print("{}\n", options.get_allowed_options());
      return 0;
    }
    const auto com =
        "COM" + std::to_string(
                    options.get_value(sc::options::com_switch).as<short>());

    sc::scan_handler scanner(com);
    auto points = scanner.start();
    const sc::dimension_converter converter(380.F, 1460.F);
    auto p = converter.convert(std::move(points));
    const sc::model_constructor constructor(
        sc::model_constructor::methods::advancing_front);
    auto mesh = constructor.make_mesh(p);
    sc::mesh_writer::write("model.stl", mesh);
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
}
