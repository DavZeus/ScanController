#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <vector>

class dimension_converter {
  using point_set =
      CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
  using vertical = std::vector<std::pair<float, float>>;

  constexpr static float min_angle = 1.8F;
  constexpr static float straight_angle = 180.F;
  constexpr static float focus_length = 16.F;
  constexpr static float camera_width_resolution = 3840.F;
  constexpr static float camera_height_resolution = 2748.F;
  constexpr static float half_camera_width = camera_width_resolution / 2.F;
  constexpr static float half_camera_height = camera_height_resolution / 2.F;

  float camera_distance_;
  float cut_level_;

public:
  explicit dimension_converter(float camera_distance, float cut_level = 0.F);
  [[nodiscard]] auto convert(const std::vector<vertical> &verticals) const
      -> point_set;
};
