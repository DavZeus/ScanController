#pragma once

#include "common_usings.h"
#include <numbers>

namespace sc {

template <std::floating_point T> class dimension_converter {
  // Basler ace GigE camera acA1440-73gm
  // Basler Lens C125-0418-5M-P
  inline static constexpr T pixel_size =
      static_cast<T>(3.45) /
      static_cast<T>(1000); // millimeters, original - micrometers, camera
  inline static constexpr T full_angle = static_cast<T>(360);     // degrees
  inline static constexpr T straight_angle = static_cast<T>(180); // degrees
  inline static constexpr T focus_length =
      static_cast<T>(4.09) * static_cast<T>(1.16);
  // millimeters, lens, crop-factor multiplier = lens diagonal / sensor diagonal
  inline static constexpr T camera_width_resolution = static_cast<T>(1456);
  // pixels, camera
  inline static constexpr T camera_height_resolution = static_cast<T>(1088);
  // pixels, camera
  inline static constexpr T half_camera_width =
      camera_width_resolution / static_cast<T>(2);
  // pixels
  inline static constexpr T half_camera_height =
      camera_height_resolution / static_cast<T>(2);
  // pixels

  // Distance from camera to axis of rotation (millimeters)
  const T camera_distance_;

public:
  explicit dimension_converter(T camera_distance);

  // Performs converting of data from scanner to point cloud
  [[nodiscard]] auto convert_circle(model_profiles<T> &&d_points,
                                    const T required_angle = full_angle) const
      -> point_set;

  [[nodiscard]] auto convert_plain(model_profiles<T> &&d_points,
                                   const T step_length) const -> point_set;
};

template <std::floating_point T>
dimension_converter<T>::dimension_converter(T camera_distance)
    : camera_distance_(camera_distance) {}

template <std::floating_point T>
auto dimension_converter<T>::convert_circle(model_profiles<T> &&d_points,
                                            const T required_angle) const
    -> point_set {
  const T angle = full_angle / static_cast<T>(d_points.size());

  const T d = camera_distance_ - focus_length;

  point_set cloud;

  unsigned angle_multiplier = 0;

  for (const auto &profile : d_points) {
    if (angle * static_cast<T>(angle_multiplier) > required_angle) {
      break;
    }

    const T rad = angle * static_cast<T>(angle_multiplier) *
                  std::numbers::pi_v<T> / straight_angle;

    const T cos_rad = std::cos(rad);
    const T sin_rad = std::sin(rad);

    if (!profile.empty()) {
      for (const auto &point : profile) {

        const T hx = (point.x - half_camera_width) * pixel_size;
        const T mx = hx * d;
        const T r = mx / (mx + focus_length);

        const T hz = (point.z - half_camera_height) * pixel_size;
        const T mz = hz * d;
        const T z = hz / (hz + focus_length);

        const T x = cos_rad * r;
        const T y = sin_rad * r;

        cloud.insert({x, y, z});
      }
    }

    ++angle_multiplier;
  }
  return cloud;
}

template <std::floating_point T>
auto dimension_converter<T>::convert_plain(model_profiles<T> &&d_points,
                                           const T step_length) const
    -> point_set {
  const T d = camera_distance_ - focus_length;

  point_set cloud;

  unsigned step_multiplier = 0;

  for (const auto &profile : d_points) {
    if (!profile.empty()) {
      for (const auto &point : profile) {

        const T hx = (point.x - half_camera_width) * pixel_size;
        const T mx = hx * d;
        const T x = mx / (mx + focus_length);

        const T hz = (point.z - half_camera_height) * pixel_size;
        const T mz = hz * d;
        const T z = hz / (hz + focus_length);

        const T y = step_length * step_multiplier;

        cloud.insert({x, y, z});
      }
    }
    ++step_multiplier;
  }
  return cloud;
}
} // namespace sc
