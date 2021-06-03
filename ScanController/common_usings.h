#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Surface_mesh.h>
#include <concepts>
#include <vector>

namespace sc {
// Stores points for use in CGAL
using point_set =
    CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
template <std::floating_point T = float> struct data_point {
  T x;
  T z;
  bool operator==(const data_point &) const = default;
};
// Stores profile points from one camera
// First is a width, second is a height
template <std::floating_point T = float>
using profile = std::vector<data_point<T>>;
// Stores all scanning points
template <std::floating_point T = float>
using model_profiles = std::vector<profile<T>>;
// Stores reconstructed model
using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

} // namespace sc
