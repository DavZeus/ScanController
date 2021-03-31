#pragma once

#include "common_usings.h"
#include <numbers>

namespace sc {

template <std::floating_point T> class dimension_converter {
  // Basler ace GigE camera acA1440-73gm
  // Basler Lens C125-0418-5M-P
  inline static constexpr T pixel_size =
      static_cast<T>(3.45); // micrometers, camera
  inline static constexpr T full_angle = static_cast<T>(360.F);     // degrees
  inline static constexpr T straight_angle = static_cast<T>(180.F); // degrees
  inline static constexpr T focus_length = static_cast<T>(4.09F);
  // millimeters, lens
  inline static constexpr T camera_width_resolution = static_cast<T>(1088.F);
  // pixels, camera
  inline static constexpr T camera_height_resolution = static_cast<T>(1456.F);
  // pixels, camera
  inline static constexpr T half_camera_width =
      camera_width_resolution / static_cast<T>(2.F);
  // pixels
  inline static constexpr T half_camera_height =
      camera_height_resolution / static_cast<T>(2.F);
  // pixels

  // Distance from camera to axis of rotation (millimeters)
  const T camera_distance_;

public:
  explicit dimension_converter(T camera_distance);

  // Performs converting of data from scanner to point cloud
  [[nodiscard]] auto convert(model_profiles<T> &&d_points) const -> point_set;
};

template <std::floating_point T>
dimension_converter<T>::dimension_converter(T camera_distance)
    : camera_distance_(camera_distance) {}

template <std::floating_point T>
auto dimension_converter<T>::convert(model_profiles<T> &&d_points) const
    -> point_set {
  const T angle = full_angle / static_cast<T>(d_points.size());
  const T cathetus =
      static_cast<T>(camera_distance_) / std::numbers::sqrt2_v<T>;

  constexpr T micro_multiplier = static_cast<T>(1000.F);

  point_set cloud;
  unsigned angle_multiplier = 0;
  T rad = static_cast<T>(0.F);

  for (const auto &profile_pair : d_points) {
    const T cos_rad = std::cos(rad);
    const T sin_rad = std::sin(rad);
    for (const auto &profile : profile_pair) {
      if (!profile.empty()) {
        for (const auto &point : profile) {
          const T hx =
              (point.x - half_camera_height) * pixel_size / micro_multiplier;
          const T p_x = camera_distance_ +
                        (camera_distance_ - focus_length) * hx / focus_length;
          const T hz =
              (point.z - half_camera_width) * pixel_size / micro_multiplier;
          const T z = (camera_distance_ - focus_length) * hz / focus_length;

          const T cos_a = cathetus / p_x;
          const T sin_a = std::sqrt(static_cast<T>(1.F) - cos_a * cos_a);
          const T opposing = p_x * sin_a;
          const T r = cathetus - opposing;

          const T x = cos_rad * r;
          const T y = sin_rad * r;

          cloud.insert({x, y, z});
        }
      }
    }
    ++angle_multiplier;
    rad = angle * static_cast<T>(angle_multiplier) * std::numbers::pi_v<T> /
          straight_angle;
  }
  return cloud;
}
} // namespace sc
