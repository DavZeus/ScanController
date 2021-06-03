#pragma once

#include <concepts>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <pylon/PylonIncludes.h>

#include "common_usings.h"
#include "simple_kalman_filter.hpp"

namespace sc {
class camera_handler {

  static constexpr std::string_view exposure_mode{"Standard"};
  static constexpr std::string_view pixel_format{"Mono8"};
  static constexpr double exposure_time = 10000.;

  static constexpr cv::MorphShapes erosion_type{cv::MORPH_CROSS};
  static constexpr int erosion_size = 2;

  static constexpr uint8_t mono_white = 255;

  Pylon::PylonAutoInitTerm pylon_init_term_;
  Pylon::CInstantCamera camera_;

  //
  auto take_photo() -> cv::Mat;

  // Apply erosion to obtained images
  static auto preprocess_image(cv::Mat &image) -> void;

  template <std::floating_point T = float>
  static auto make_profile(const cv::Mat &image) -> profile<T>;

  template <std::floating_point T = float>
  static auto filter_profile(profile<T> &profile) -> void;

public:
  auto initialize() -> void;

  template <std::floating_point T = float>
  auto get_current_profile() -> profile<T>;
};

template <std::floating_point T>
auto camera_handler::make_profile(const cv::Mat &image) -> profile<T> {
  profile<T> result;
  result.reserve(image.rows);
  constexpr T two = 2;
  for (int row = 0; row < image.rows; ++row) {
    int col_1 = 0;
    while (col_1 < image.cols) {
      if (image.at<uint8_t>(row, col_1) == mono_white) {
        int col_2 = col_1 + 1;
        while (image.at<uint8_t>(row, col_2) == mono_white &&
               col_2 < image.cols) {
          ++col_2;
        }
        result.emplace_back(static_cast<T>(col_1 + col_2 - 1) / two,
                            static_cast<T>(row));
        col_1 = col_2;
      }
      ++col_1;
    }
  }
  result.shrink_to_fit();
  return result;
}

template <std::floating_point T>
auto camera_handler::filter_profile(profile<T> &profile) -> void {
  if (!profile.empty()) {
    constexpr T error = static_cast<T>(0.1), q = static_cast<T>(1.),
                r = static_cast<T>(1.);
    simple_kalman_filter<T> filter(profile.begin()->x, error, q, r);
    for (auto &point : profile) {
      point.x = filter.update(point.x);
    }
  }
}

template <std::floating_point T>
auto camera_handler::get_current_profile() -> profile<T> {
  auto images = take_photo();
  preprocess_image(images);
  auto obtained_profiles = make_profile<T>(images);
  filter_profile<T>(obtained_profiles);
  return obtained_profiles;
}
} // namespace sc
