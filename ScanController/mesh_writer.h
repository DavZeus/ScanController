#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

class mesh_writer {
  using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

public:
  static auto write(std::string path, const surface_mesh &mesh) -> void;
};
