#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Surface_mesh.h>

namespace sc {

class model_constructor {
  using point_set =
      CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
  using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

public:
  enum class methods : int8_t { advancing_front, scale_space, poisson };

private:
  methods method_;

public:
  // I do not know how to make this enum better because I can not overload
  // operators of nested enum class

  enum additional_options : unsigned {
    remove_outliers = 1,
    simplification = remove_outliers * 2,
    smooth = simplification * 2,
    all = remove_outliers | simplification | smooth
  };

private:
  additional_options options_;

  auto remove_outliers_from_set(point_set &points, size_t k_neighbors) const
      -> void;
  auto simplify_set(point_set &points, size_t k_neighbors) const -> void;
  auto smooth_set(point_set &points, size_t k_neighbors) const -> void;

  auto process_additional(point_set &points) const -> void;
  auto do_advancing_front(point_set &points) const -> surface_mesh;
  auto do_scale_space(point_set &points) const -> surface_mesh;
  auto do_poisson(point_set &points) const -> surface_mesh;

public:
  auto make_mesh(point_set points) const -> surface_mesh;

  explicit model_constructor(methods method, additional_options options = all);
};

} // namespace sc
