#include "model_constructor.h"

#ifndef CGAL_LINKED_WITH_TBB
#define CGAL_LINKED_WITH_TBB
#endif

#include <CGAL/compute_average_spacing.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/remove_outliers.h>

auto model_constructor::remove_outliers_from_set(point_set &points) const
    -> void {
  auto outliers_iterator = CGAL::remove_outliers<CGAL::Parallel_tag>(
      points, 24, points.parameters().threshold_percent(5.0));
  points.remove(outliers_iterator, points.end());
  points.collect_garbage();
}

auto model_constructor::simplify_set(point_set &points) const -> void {
  const auto spacing =
      CGAL::compute_average_spacing<CGAL::Parallel_tag>(points, 6);
  auto simplification_iterator =
      CGAL::grid_simplify_point_set(points, 2. * spacing);
  points.remove(simplification_iterator, points.end());
  std::cout << points.number_of_removed_points()
            << " point(s) removed after simplification." << std::endl;
  points.collect_garbage();
}

auto model_constructor::smooth_set(point_set &points) -> void {
  CGAL::jet_smooth_point_set<CGAL::Parallel_tag>(points, 24);
}

auto model_constructor::process_additional(point_set &points) -> void {
  if (options_ & remove_outliers) {
    remove_outliers_from_set(points);
  }
  if (options_ & simplification) {
    simplify_set(points);
  }
  if (options_ & smooth) {
    smooth_set(points);
  }
}

auto model_constructor::make_mesh(point_set points) -> surface_mesh {
  process_additional(points);
  std::function<surface_mesh(point_set)> method_function;
  switch (method_) {
  case method::advancing_front:
    method_function = std::bind_front(&do_advancing_front, this);
    break;
  case method::scale_space:
    method_function = std::bind_front(&do_scale_space, this);
    break;
  default:
    throw std::exception("There is not such model construction method");
  }
  return method_function(points);
}

model_constructor::model_constructor(const method method,
                                     const additional_options options)
    : method_(method), options_(options) {}
