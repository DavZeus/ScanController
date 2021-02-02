#pragma once

#include "common_usings.h"

namespace sc {

namespace io {

// Write mesh to stl file
static auto write_mesh(std::string path, const surface_mesh &mesh) -> void;
// Write data camera point data
static auto write_data_points(const data_points &points, std::string filename)
    -> bool;
// Read data saved camera point data
static auto read_data_points(const std::string &filename) -> data_points;

} // namespace io

} // namespace sc
