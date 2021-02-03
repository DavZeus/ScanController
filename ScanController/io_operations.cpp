#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "io_operations.h"

#include <CGAL/IO/STL_writer.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

auto sc::io::generate_time_string() -> std::string {
  std::string time_string;
  std::stringstream time_parse(time_string);
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  time_parse << std::put_time(std::localtime(&time), "%FT%T");
  return time_string;
}

auto sc::io::write_mesh(std::string path, const surface_mesh &mesh) -> void {
  std::ofstream file(path);
  CGAL::write_STL(mesh, file);
  // Force write content to file
  file.flush();
}

auto sc::io::write_data_points(const data_points &points, std::string filename)
    -> bool {
  if (filename.empty()) {
    filename = generate_time_string() + ".txt";
  } else {
    filename += ".txt";
  }
  std::ofstream outfile(filename);
  outfile << "x-z" << std::flush;
  if (!outfile) {
    return false;
  }
  for (const auto &v : points) {
    for (const auto &p : v) {
      outfile << '\n' << p.first << " " << p.second << std::flush;
    }
    outfile << "\n^" << std::flush;
  }
  return true;
}

auto sc::io::read_data_points(const std::string &filename) -> data_points {
  data_points points;
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
    static vertical vertical_points;
    std::getline(file, buffer, '\n');
    if (buffer == "^") {
      points.push_back(vertical_points);
      vertical_points.clear();
      continue;
    }
    static std::pair<float, float> point;
    std::stringstream stream(buffer);
    stream >> point.first >> point.second;
    vertical_points.push_back(point);
  }
  return points;
}
