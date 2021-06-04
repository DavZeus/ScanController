// Suppress the min and max definitions in Windef.h to use numeric_limits max
// function
#define NOMINMAX

#include "camera_handler.h"

#include <fmt/core.h>
#include <opencv2/core.hpp>

auto sc::camera_handler::take_photo() -> cv::Mat {
  Pylon::CGrabResultPtr result;

  // Loop is needed because image can be not grabbed
  do {
    camera_.StartGrabbing(images_number);
    camera_.WaitForFrameTriggerReady(trigger_wait_time);
    camera_.ExecuteSoftwareTrigger();
    camera_.RetrieveResult(INFINITE, result);
  } while (!result->GrabSucceeded());

  cv::Mat img(result->GetHeight(), result->GetWidth(), CV_8U);
  std::memcpy(img.data, result->GetBuffer(),
              static_cast<size_t>(img.cols) * img.rows);
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

  camera_.RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration,
                                Pylon::RegistrationMode_ReplaceAll,
                                Pylon::Cleanup_Delete);

  camera_.Open();
  if (!camera_.IsOpen()) {
    throw std::exception("Can not connect to cameras");
  }

  auto &node_map = camera_.GetNodeMap();
  Pylon::CEnumParameter(node_map, "ExposureMode")
      .SetValue("Timed"); // or Standard
  Pylon::CFloatParameter(node_map, "ExposureTimeAbs").SetValue(exposure_time);
  Pylon::CEnumParameter(node_map, "PixelFormat").SetValue("Mono8");
  Pylon::CIntegerParameter(node_map, "Width").SetToMaximum();
  Pylon::CIntegerParameter(node_map, "Height").SetToMaximum();
  Pylon::CBooleanParameter(node_map, "CenterX").SetValue(true);
  Pylon::CBooleanParameter(node_map, "CenterY").SetValue(true);
}
