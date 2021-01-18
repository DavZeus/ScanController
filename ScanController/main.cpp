#include "dimension_converter.h"
#include "file_writer.h"
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
    if (!options.check_value(sc::options::distance_switch)) {
      throw std::exception("Distance to camera is not specified");
    }
    const auto dist = options.get_value(sc::options::distance_switch).as<float>(); 
    const auto com =
        "COM" +
        std::to_string(options.get_value(sc::options::com_switch).as<short>());
    sc::scan_handler scanner(com);
    auto points = scanner.start();
    const auto cut = options.get_value(sc::options::cut_switch).as<float>();
    const sc::dimension_converter converter(dist, cut);
    auto p = converter.convert(std::move(points));
    const sc::model_constructor constructor(
        sc::model_constructor::methods::advancing_front);
    auto mesh = constructor.make_mesh(p);
    sc::file_writer::write_mesh("model.stl", mesh);
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
}
