#pragma once

#include "common_usings.h"

namespace sc {

namespace io {

// Write mesh to stl file
auto write_mesh(std::string path, const surface_mesh &mesh) -> void;
// Write data camera point data
auto write_data_points(const data_points &points, std::string filename) -> bool;
// Read data saved camera point data
auto read_data_points(const std::string &filename) -> data_points;

} // namespace io

} // namespace sc
