// Suppress the min and max definitions in Windef.h to use numeric_limits max
// function
#define NOMINMAX

#include "camera_handler.h"

#include <fmt/format.h>
#include <opencv2/core.hpp>

auto sc::camera_handler::take_photo() -> cv::Mat {
  Pylon::CGrabResultPtr result;

  while (!camera_.GrabOne(INFINITE, result, Pylon::TimeoutHandling_Return)) {
    fmt::print("\n{}\n", result->GetErrorDescription().c_str());
  }
  cv::Mat img(result->GetHeight(), result->GetWidth(), CV_8U);
  std::memcpy(img.data, static_cast<uint8_t *>(result->GetBuffer()),
              sizeof(uint8_t) * img.cols * img.rows);
  return img;
}

auto sc::camera_handler::preprocess_image(cv::Mat &image) -> void {
  const cv::Mat structuring_element = cv::getStructuringElement(
      erosion_type, cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
      cv::Point(erosion_size, erosion_size));
  image.forEach<uint8_t>([](uint8_t &pixel, const int *pos) {
    if (pixel != mono_white) {
      pixel = 0;
    }
  });
  cv::erode(image, image, structuring_element);
}

auto sc::camera_handler::initialize() -> void {
  camera_.Attach(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
  if (!camera_.IsPylonDeviceAttached()) {
    throw std::exception("There is no camera");
  }

  camera_.Open();
  if (!camera_.IsOpen()) {
    throw std::exception("Can not connect to cameras");
  }

  auto &node_map = camera_.GetNodeMap();
  Pylon::CEnumParameter(node_map, "ExposureMode").SetValue("Timed");
  Pylon::CFloatParameter(node_map, "ExposureTimeAbs").SetValue(exposure_time);
  Pylon::CEnumParameter(node_map, "PixelFormat").SetValue(pixel_format.data());
  Pylon::CIntegerParameter(node_map, "Width").TrySetToMaximum();
  Pylon::CIntegerParameter(node_map, "Height").TrySetToMaximum();
  Pylon::CBooleanParameter(node_map, "CenterX").SetValue(true);
  Pylon::CBooleanParameter(node_map, "CenterY").SetValue(true);
}
