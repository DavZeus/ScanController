#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Surface_mesh.h>
#include <vector>

namespace sc {
// Stores points for use in CGAL
using point_set =
    CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
// Stores points from one vertical scan
using vertical = std::vector<std::pair<float, float>>;
// Stores all scanning points
using data_points = std::vector<vertical>;
// Stores reconstructed model
using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

} // namespace sc
