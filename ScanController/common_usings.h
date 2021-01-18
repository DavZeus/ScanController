#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Point_set_3.h>
#include <vector>

namespace sc
{
  using point_set =
      CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
  using vertical = std::vector<std::pair<float, float>>;
  using data_points = std::vector<vertical>;
  using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

}
