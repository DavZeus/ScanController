#include "runner.h"

#include "dimension_converter.h"
#include "io_operations.h"
#include "model_constructor.h"
#include "scan_handler.h"

#include <fmt/ostream.h>

auto sc::runner::start(int argc, char *argv[]) -> int {
  try {
    options_.parse(argc, argv);
    if (options_.check_value(options::help_switch)) {
      fmt::print("{}\n", options_.get_allowed_options());
      return 0;
    }
    if (!options_.check_value(options::distance_switch)) {
      throw std::exception("Distance to camera is not specified");
    }
    const auto dist = options_.get_value(options::distance_switch).as<float>();
    const auto com =
        "COM" +
        std::to_string(options_.get_value(options::com_switch).as<short>());
    scan_handler scanner(com);
    auto points = scanner.start();
    const auto cut = options_.get_value(options::cut_switch).as<float>();
    const dimension_converter converter(dist, cut);
    auto p = converter.convert(std::move(points));
    const model_constructor constructor(
        model_constructor::methods::advancing_front);
    auto mesh = constructor.make_mesh(p);
    io_operations::write_mesh("model.stl", mesh);
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
    return 1;
  }
  return EXIT_SUCCESS;
}
