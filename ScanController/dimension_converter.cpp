#include "dimension_converter.h"

#include <numbers>

sc::dimension_converter::dimension_converter(const float camera_distance,
                                             const float cut_level)
    : camera_distance_(camera_distance), cut_level_(cut_level) {}

auto sc::dimension_converter::convert(data_points &&verticals) const
    -> point_set {
  point_set point_set;
  const auto angle = full_angle / verticals.size();
  const auto cathetus = camera_distance_ / std::numbers::sqrt2_v<float>;
  auto angle_multiplier = 0U;
  auto rad = 0.F;
  for (const auto &vertical : verticals) {
    const auto cos_rad = std::cos(rad);
    const auto sin_rad = std::sin(rad);
    for (const auto &point : vertical) {
      if (point.second < cut_level_) {
        continue;
      }
      const auto hx = (point.first - half_camera_height) * pixel_size / 1000.F;
      const auto p_x = camera_distance_ +
                       (camera_distance_ - focus_length) * hx / focus_length;
      const auto hz = (point.second - half_camera_width) * pixel_size / 1000.F;
      const auto z = (camera_distance_ - focus_length) * hz / focus_length;

      const auto cos_a = cathetus / p_x;
      const auto sin_a = std::sqrt(1.F - cos_a * cos_a);
      const auto opposing = p_x * sin_a;
      const auto r = cathetus - opposing;

      const auto x = cos_rad * r;
      const auto y = sin_rad * r;

      point_set.insert({x, y, z});
    }
    ++angle_multiplier;
    rad = angle * static_cast<float>(angle_multiplier) *
          std::numbers::pi_v<float> / straight_angle;
  }
  return point_set;
}
