#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "io_operations.h"

#include <CGAL/IO/STL_writer.h>
#include <chrono>
#include <ctime>
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
