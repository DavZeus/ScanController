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
  std::stringstream time_parse);
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  auto tm = *std::localtime(&time);
  time_parse << std::put_time(&tm, "%FT%T") << std::flush;
  return time_parse.str();
}

auto sc::io::write_mesh(const surface_mesh &mesh,
                        std::filesystem::path filename) -> void {
  if (filename.empty()) {
    filename = "model-" + generate_time_string() + ".stl";
  } else if (filename.extension().empty()) {
    filename += ".stl";
  } else if (filename.extension() != "stl") {
    throw std::exception("Extenstion is not supported");
  }
  std::ofstream file(filename);
  CGAL::write_STL(mesh, file);
  // Force write content to file
  file.flush();
}
