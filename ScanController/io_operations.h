#pragma once

#include "common_usings.h"
#include <filesystem>
#include <fstream>

namespace sc {

namespace io {

// Get current time as string
auto generate_time_string() -> std::string;
// Write mesh to stl file
auto write_mesh(std::string path, const surface_mesh &mesh) -> void;
// Write data camera point data
// TODO: Rewrite
// template <std::floating_point T>
// auto write_data_points(const model_profiles<T> &points,
//                       std::string filename = {}) -> bool {
//  if (filename.empty()) {
//    filename = generate_time_string() + ".txt";
//  } else {
//    filename += ".txt";
//  }
//  std::ofstream outfile(filename);
//  outfile << "x-z" << std::flush;
//  if (!outfile) {
//    return false;
//  }
//  for (const auto &v : points) {
//    for (const auto &p : v) {
//      outfile << '\n' << p.first << " " << p.second << std::flush;
//    }
//    outfile << "\n^" << std::flush;
//  }
//  return true;
//}

// Read data saved camera point data
// TODO: Rewrite
// template <std::floating_point T>
// auto read_data_points(const std::string &filename) -> model_profiles<T> {
//  model_profiles<T> points;
//  if (!std::filesystem::exists(filename)) {
//    throw std::exception("There is no such data file");
//  }
//  std::ifstream file(filename);
//  if (!file) {
//    throw std::exception("Can not open data file");
//  }
//  std::string buffer;
//  std::getline(file, buffer, '\n');
//  if (buffer != "x-z") {
//    throw std::exception("Wrong data file");
//  }
//  while (!file.eof()) {
//    static profile_part vertical_points;
//    std::getline(file, buffer, '\n');
//    if (buffer == "^") {
//      points.push_back(vertical_points);
//      vertical_points.clear();
//      continue;
//    }
//    static std::pair<float, float> point;
//    std::stringstream stream(buffer);
//    stream >> point.first >> point.second;
//    vertical_points.push_back(point);
//  }
//  return points;
//}
} // namespace io

} // namespace sc
