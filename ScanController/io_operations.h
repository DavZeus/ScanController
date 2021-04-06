#pragma once

#include "common_usings.h"
#include <filesystem>
#include <fstream>

namespace sc {

namespace io {

constexpr auto data_header = "x-z";
constexpr auto end_part = "-";
constexpr auto end_pair = "^";

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
  for (const auto &pair : points) {
    for (const auto &profile : pair) {
      for (const auto &point : profile) {
        outfile << '\n' << point.x << ' ' << point.z << std::flush;
      }
      outfile << '\n' << end_part << std::flush;
    }
    outfile << '\n' << end_pair << std::flush;
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
    static profile_pair<T> pair;
    static profile_part<T> part;
    static size_t i = 0;
    std::getline(file, buffer, '\n');
    if (buffer == end_part) {
      pair.at(i) = {part.begin(), part.end()};
      i ^= 1;
      part.clear();
      continue;
    }
    if (buffer == end_pair) {
      points.push_back(pair);
    }
    static data_point<T> point;
    std::stringstream stream(buffer);
    stream >> point.x >> point.z;
    part.push_back(point);
  }
  return points;
}
} // namespace io

} // namespace sc
