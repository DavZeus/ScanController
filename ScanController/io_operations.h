#pragma once

#include "common_usings.h"
#include <filesystem>
#include <fstream>

namespace sc {

namespace io {

constexpr auto data_header = "x-z";
constexpr auto end_profile = "-";

// Get current time as string
auto generate_time_string() -> std::string;
// Write mesh to stl file
auto write_mesh(const surface_mesh &mesh, std::filesystem::path filename = {})
    -> void;
// Write data camera point data
template <std::floating_point T>
auto write_data_points(const model_profiles<T> &points,
                       std::filesystem::path file = {}) -> void {
  if (file.empty()) {
    file = "points-" + generate_time_string() + ".txt";
  } else if (file.extension().empty()) {
    file.append(".txt");
  }
  std::ofstream outfile(file);
  if (!outfile) {
    throw std::exception("Can not open/create data file");
  }
  outfile << data_header << std::flush;
  for (const auto &profile : points) {
    for (const auto &point : profile) {
      outfile << '\n' << point.x << ' ' << point.z << std::flush;
    }
    outfile << '\n' << end_profile << std::flush;
  }
}

// Read data saved camera point data
template <std::floating_point T>
auto read_data_points(const std::string &filename) -> model_profiles<T> {
  model_profiles<T> points;
  if (!std::filesystem::exists(filename)) {
    throw std::exception("There is no such data file");
  }
  std::ifstream file(filename);
  if (!file) {
    throw std::exception("Can not open data file");
  }
  std::string buffer;
  std::getline(file, buffer, '\n');
  if (buffer != "x-z") {
    throw std::exception("Wrong data file");
  }
  while (!file.eof()) {
    static profile<T> profile;
    std::getline(file, buffer, '\n');
    if (buffer == end_profile) {
      points.emplace_back(profile);
      profile.clear();
      continue;
    }
    static data_point<T> point;
    std::stringstream stream(buffer);
    stream >> point.x >> point.z;
    profile.push_back(point);
  }
  return points;
}
} // namespace io

} // namespace sc
