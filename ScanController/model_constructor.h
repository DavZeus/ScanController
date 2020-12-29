#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Surface_mesh.h>
#include <type_traits>

class model_constructor {
  using point_set =
      CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
  using surface_mesh = CGAL::Surface_mesh<CGAL::Epick::Point_3>;

public:
  enum class method : int8_t { advancing_front, scale_space };

private:
  method method_;

public:
  // I do not know how to make this enum better because I can not overload
  // operators of nested enum class

  enum additional_options {
    remove_outliers = 1,
    simplification = remove_outliers * 2,
    smooth = simplification * 2,
    all = remove_outliers | simplification | smooth
  };

private:
  additional_options options_;

  auto remove_outliers_from_set(point_set &points) const -> void;
  auto simplify_set(point_set &points) const -> void;
  auto smooth_set(point_set &points) -> void;

  auto process_additional(point_set &points) -> void;

  auto do_advancing_front(point_set points) -> surface_mesh;
  auto do_scale_space(point_set points) -> surface_mesh;

public:
  auto make_mesh(point_set points) -> surface_mesh;

  explicit model_constructor(method method, additional_options options = all);
};
