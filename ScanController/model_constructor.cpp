#include "model_constructor.h"

#ifndef CGAL_LINKED_WITH_TBB
#define CGAL_LINKED_WITH_TBB
#endif

#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Scale_space_reconstruction_3/Advancing_front_mesher.h>
#include <CGAL/Scale_space_reconstruction_3/Jet_smoother.h>
#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/estimate_scale.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/mst_orient_normals.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/remove_outliers.h>

auto model_constructor::remove_outliers_from_set(point_set &points,
                                                 size_t k_neighbors) const
    -> void {
  auto outliers_iterator = CGAL::remove_outliers<CGAL::Parallel_tag>(
      points, k_neighbors, points.parameters().threshold_percent(5.0));
  points.remove(outliers_iterator, points.end());
  points.collect_garbage();
}

auto model_constructor::simplify_set(point_set &points,
                                     size_t k_neighbors) const -> void {
  const auto spacing =
      CGAL::compute_average_spacing<CGAL::Parallel_tag>(points, k_neighbors);
  auto simplification_iterator =
      CGAL::grid_simplify_point_set(points, 2. * spacing);
  points.remove(simplification_iterator, points.end());
  std::cout << points.number_of_removed_points()
            << " point(s) removed after simplification." << std::endl;
  points.collect_garbage();
}

auto model_constructor::smooth_set(point_set &points, size_t k_neighbors) const
    -> void {
  CGAL::jet_smooth_point_set<CGAL::Parallel_tag>(points, k_neighbors);
}

auto model_constructor::process_additional(point_set &points) const -> void {
  const auto k_neighbors = CGAL::estimate_global_k_neighbor_scale(points) * 2;
  if (options_ & remove_outliers) {
    remove_outliers_from_set(points, k_neighbors);
  }
  if (options_ & simplification) {
    simplify_set(points, k_neighbors);
  }
  if (options_ & smooth) {
    smooth_set(points, k_neighbors);
  }
}

auto model_constructor::do_advancing_front(point_set &points) const
    -> surface_mesh {
  using facet = std::array<std::size_t, 3>;
  std::vector<facet> facets;
  CGAL::advancing_front_surface_reconstruction(points.points().begin(),
                                               points.points().end(),
                                               std::back_inserter(facets));
  std::vector<CGAL::Epick::Point_3> vertices;
  vertices.reserve(points.size());
  std::copy(points.points().begin(), points.points().end(),
            std::back_inserter(vertices));
  points.clear();
  surface_mesh output_mesh;
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(vertices, facets,
                                                              output_mesh);
  return output_mesh;
}

auto model_constructor::do_scale_space(point_set &points) const
    -> surface_mesh {
  std::vector<CGAL::Epick::Point_3> vertices;
  std::vector<std::array<std::size_t, 3>> facets;
  CGAL::Surface_mesh<CGAL::Epick::Point_3> output_mesh;
  {
    const auto maximum_facet_length = 5.0;
    CGAL::Scale_space_surface_reconstruction_3<CGAL::Epick> reconstruct(
        points.points().begin(), points.points().end());
    reconstruct.increase_scale(
        4, CGAL::Scale_space_reconstruction_3::Jet_smoother<CGAL::Epick>());
    reconstruct.reconstruct_surface(
        CGAL::Scale_space_reconstruction_3::Advancing_front_mesher<CGAL::Epick>(
            maximum_facet_length));
    vertices.reserve(points.points().size());
    std::copy(points.points().begin(), points.points().end(),
              std::back_inserter(vertices));

    facets.reserve(reconstruct.number_of_facets());
    std::copy(reconstruct.facets_begin(), reconstruct.facets_end(),
              std::back_inserter(facets));
  }
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(vertices, facets,
                                                              output_mesh);
  return output_mesh;
}

auto model_constructor::do_poisson(point_set &points) const -> surface_mesh {
  const auto k_neighbors = CGAL::estimate_global_k_neighbor_scale(points) * 2;
  const auto spacing =
      CGAL::compute_average_spacing<CGAL::Parallel_tag>(points, k_neighbors);
  CGAL::jet_estimate_normals<CGAL::Parallel_tag>(points, k_neighbors);
  auto unoriented_points_begin = CGAL::mst_orient_normals(points, k_neighbors);
  points.remove(unoriented_points_begin, points.end());
  points.collect_garbage();
  surface_mesh output_mesh;
  CGAL::poisson_surface_reconstruction_delaunay(
      points.begin(), points.end(), points.point_map(), points.normal_map(),
      output_mesh, 0.25 * spacing);
  return output_mesh;
}

auto model_constructor::make_mesh(point_set &points) const -> surface_mesh {
  process_additional(points);
  switch (method_) {
  case methods::advancing_front:
    return do_advancing_front(points);
  case methods::scale_space:
    return do_scale_space(points);
  case methods::poisson:
    return do_poisson(points);
  default:
    throw std::exception("There is not such model construction methods");
  }
}

model_constructor::model_constructor(methods method, additional_options options)
    : method_(method), options_(options) {}
