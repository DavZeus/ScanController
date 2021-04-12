#pragma once

#include <array>
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

  static constexpr std::string_view left_camera_name{"Left"};
  static constexpr std::string_view right_camera_name{"Right"};

  static constexpr cv::MorphShapes erosion_type{cv::MORPH_CROSS};
  static constexpr int erosion_size = 2;

  static constexpr uint8_t mono_white = 255;

  using img_array = std::array<cv::Mat, number_of_cameras>;

  Pylon::PylonAutoInitTerm pylon_init_term_;
  Pylon::CInstantCameraArray cameras_{number_of_cameras};

  size_t img_number_{0};

  //
  auto take_photos() -> img_array;

  // Apply erosion to obtained images
  static auto preprocess_images(img_array &images) -> void;

  template <std::floating_point T = float>
  static auto make_single_profile(const cv::Mat &image) -> profile_part<T>;

  template <std::floating_point T = float>
  static auto make_profiles(const img_array &images) -> profile_pair<T>;

  template <std::floating_point T = float>
  static auto filter_single_profile(profile_part<T> &profile) -> void;

  template <std::floating_point T = float>
  static auto filter_profiles(profile_pair<T> &profiles) -> void;

public:
  auto initialize() -> void;

  template <std::floating_point T = float>
  auto get_current_profiles() -> std::array<profile_part<T>, number_of_cameras>;
};

template <std::floating_point T>
auto camera_handler::make_single_profile(const cv::Mat &image)
    -> profile_part<T> {
  profile_part<T> result;
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
auto camera_handler::make_profiles(const img_array &images) -> profile_pair<T> {
  profile_pair<T> result;
  for (size_t i = 0; i < images.size(); ++i) {
    result.at(i) = make_single_profile(images.at(i));
  }
  return result;
}

template <std::floating_point T>
auto camera_handler::filter_single_profile(profile_part<T> &profile) -> void {
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
auto camera_handler::filter_profiles(profile_pair<T> &profiles) -> void {
  for (auto &profile : profiles) {
    filter_single_profile<T>(profile);
  }
}

template <std::floating_point T>
auto camera_handler::get_current_profiles()
    -> std::array<profile_part<T>, number_of_cameras> {
  auto images = take_photos();
  preprocess_images(images);
  auto obtained_profiles = make_profiles<T>(images);
  filter_profiles<T>(obtained_profiles);
  return obtained_profiles;
}
} // namespace sc
