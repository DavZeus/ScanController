#include "runner.h"

#include "dimension_converter.h"
#include "io_operations.h"
#include "model_constructor.h"

#include <fmt/ostream.h>

auto sc::runner::do_scan_branch() -> data_points {
  const auto com =
      "COM" +
      std::to_string(options_.get_value(options::com_switch).as<short>());
  const auto cut_level = options_.get_value(options::cut_switch).as<float>();
  scan_handler scanner(com, cut_level);
  return scanner.start();
}

auto sc::runner::do_data_load_branch() -> data_points {
  if (options_.check_value(options::load_switch) > 1) {
    throw std::exception("Only one file can be passed");
  }
  const std::string file =
      options_.get_value(options::load_switch).as<std::string>();
  return io::read_data_points(file);
}

auto sc::runner::start(int argc, char *argv[]) -> int {
  try {
    options_.parse(argc, argv);
    if (options_.check_value(options::help_switch)) {
      fmt::print("{}\n", options_.get_allowed_options());
      return EXIT_SUCCESS;
    }
    if (!options_.check_value(options::distance_switch)) {
      throw std::exception("Distance to camera is not specified");
    }
    data_points d_points;
    if (options_.check_value(options::load_switch)) {
      d_points = do_data_load_branch();
    } else {
      d_points = do_scan_branch();
    }
    if (options_.check_value(options::save_switch) &&
        !options_.check_value(options::load_switch)) {
      io::write_data_points(d_points);
    }
    const auto camera_distance =
        options_.get_value(options::distance_switch).as<float>();
    const dimension_converter converter(camera_distance);
    const auto points = converter.convert(std::move(d_points));
    const model_constructor constructor(
        model_constructor::methods::advancing_front);
    const auto mesh = constructor.make_mesh(points);
    io::write_mesh("model.stl", mesh);
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
