#pragma once

#include "common_usings.h"

namespace sc {

class dimension_converter {
  // Camera parameters
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
  // Performs converting of data from scanner to point cloud
  [[nodiscard]] auto convert(data_points &&verticals) const -> point_set;
};

} // namespace sc
