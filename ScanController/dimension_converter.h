#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <vector>

namespace sc {

class dimension_converter {
  using point_set =
      CGAL::Point_set_3<CGAL::Epick::Point_3, CGAL::Epick::Vector_3>;
  using vertical = std::vector<std::pair<float, float>>;

  constexpr static float min_angle = 1.8F;                  // degrees
  constexpr static float pixel_size = 1.67F;                // micrometers
  constexpr static float straight_angle = 180.F;            // degrees
  constexpr static float focus_length = 16.F;               // millimeters
  constexpr static float camera_width_resolution = 3840.F;  // pixels
  constexpr static float camera_height_resolution = 2748.F; // pixels
  constexpr static float half_camera_width =
      camera_width_resolution / 2.F; // pixels
  constexpr static float half_camera_height =
      camera_height_resolution / 2.F; // pixels

  float camera_distance_; // millimeters
  float cut_level_;       // pixels

public:
  explicit dimension_converter(float camera_distance, float cut_level = 0.F);
  [[nodiscard]] auto convert(std::vector<vertical> &&verticals) const
      -> point_set;
};

} // namespace sc
