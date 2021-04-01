#include "runner.h"

#include "dimension_converter.hpp"
#include "io_operations.h"
#include "model_constructor.h"

#include <fmt/ostream.h>

auto sc::runner::do_scan_branch() const -> model_profiles<float> {
  const auto com =
      "COM" +
      std::to_string(
          options_.get_value(program_arguments::com_switch).as<short>());
  const auto cut_level =
      options_.get_value(program_arguments::cut_switch).as<float>();
  const auto step_count =
      options_.get_value(program_arguments::steps_switch).as<unsigned>();
  scan_handler scanner(com, cut_level, step_count);
  return scanner.start<float>();
}

auto sc::runner::do_data_load_branch() const -> model_profiles<float> {
  if (options_.check_value(program_arguments::load_switch) > 1) {
    throw std::exception("Only one file can be passed");
  }
  const std::string file =
      options_.get_value(program_arguments::load_switch).as<std::string>();
  return io::read_data_points<float>(file);
}

auto sc::runner::start(int argc, char *argv[]) -> int {
  try {
    options_.parse(argc, argv);
    if (options_.check_value(program_arguments::help_switch)) {
      fmt::print("{}\n", options_.get_allowed_options());
      return EXIT_SUCCESS;
    }
    if (!options_.check_value(program_arguments::distance_switch)) {
      throw std::exception("Distance to camera is not specified");
    }
    model_profiles<float> d_points;
    if (options_.check_value(program_arguments::load_switch)) {
      d_points = do_data_load_branch();
    } else {
      d_points = do_scan_branch();
    }
    if (options_.check_value(program_arguments::save_switch) &&
        !options_.check_value(program_arguments::load_switch)) {
      io::write_data_points(d_points);
    }
    const auto camera_distance =
        options_.get_value(program_arguments::distance_switch).as<float>();
    const dimension_converter<float> converter(camera_distance);
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
