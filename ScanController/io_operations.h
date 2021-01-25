#pragma once

#include "common_usings.h"

namespace sc {

class io_operations {

public:
  static auto write_mesh(std::string path, const surface_mesh &mesh) -> void;
  static auto write_data_points(const data_points &points, std::string filename) -> bool;
  static auto read_data_points(const std::string &filename) -> data_points;
};

} // namespace sc
