#pragma once

#include "common_usings.h"

namespace sc {

class file_writer {

public:
  static auto write_mesh(std::string path, const surface_mesh &mesh) -> void;
  //static auto write_data_points(const data_points &points);
};

} // namespace sc
