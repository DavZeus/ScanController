// Suppress the min and max definitions in Windef.h to use numeric_limits max
// function
#define NOMINMAX

#include "camera_handler.h"

#include <filesystem>
#include <fmt/format.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

auto sc::camera_handler::take_photos() -> img_array {
  img_array photos;
  for (size_t i = 0; i < cameras_.GetSize(); ++i) {
    unsigned repeat_counter = 0;
    while (true) {
      try {
        Pylon::CGrabResultPtr result;
        cameras_[i].GrabOne(INFINITE, result);
        // Save logic start
        std::string file = std::string(static_cast<sc::camera_position>(i) ==
                                               sc::camera_position::left
                                           ? "left-"
                                           : "right-") +
                           '-' + std::to_string(img_number_);
        Pylon::CPylonImage p_img;
        p_img.AttachGrabResultBuffer(result);
        p_img.Save(Pylon::ImageFileFormat_Bmp, file.c_str());
        p_img.Save(Pylon::ImageFileFormat_Raw, file.c_str());
        p_img.Save(Pylon::ImageFileFormat_Png, file.c_str());
        ++img_number_;
        // Save logic end
        cv::Mat cv_img(result->GetHeight(), result->GetWidth(), CV_8U);
        std::memcpy(cv_img.data, static_cast<uint8_t *>(result->GetBuffer()),
                    sizeof(uint8_t) * cv_img.cols * cv_img.rows);
        photos.at(i) = cv_img;
        break;
      } catch (const Pylon::RuntimeException &ex) {
        fmt::print(stderr, "\n{}\n", ex.GetDescription());
        if (repeat_counter == 10) {
          throw std::exception("Can not retrieve image from one of cameras");
        }
        ++repeat_counter;
      }
    }
  }
  return photos;
}

auto sc::camera_handler::preprocess_images(img_array &images) -> void {
  const cv::Mat structuring_element = cv::getStructuringElement(
      erosion_type, cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
      cv::Point(erosion_size, erosion_size));
  for (size_t i = 0; i < images.size(); ++i) {
    images.at(i).forEach<uint8_t>([](uint8_t &pixel, const int *pos) {
      if (pixel != mono_white) {
        pixel = 0;
      }
    });
    cv::erode(images.at(i), images.at(i), structuring_element);
    if (static_cast<camera_position>(i) == camera_position::right) {
      cv::flip(images.at(i), images.at(i), 1);
    }
  }
}

auto sc::camera_handler::initialize() -> void {

  Pylon::DeviceInfoList device_info_list;
  Pylon::CTlFactory &transport_layer_factory = Pylon::CTlFactory::GetInstance();
  if (transport_layer_factory.EnumerateDevices(device_info_list) == 0) {
    throw std::exception("There is no cameras in network");
  }
  for (const auto &elem : device_info_list) {
    if (elem.GetUserDefinedName() == "Left") {
      cameras_[static_cast<size_t>(camera_position::left)].Attach(
          transport_layer_factory.CreateDevice(elem));
      continue;
    }
    if (elem.GetUserDefinedName() == "Right") {
      cameras_[static_cast<size_t>(camera_position::right)].Attach(
          transport_layer_factory.CreateDevice(elem));
    }
  }
  if (!cameras_.IsPylonDeviceAttached()) {
    throw std::exception(fmt::format(R"(There is no "{}" and/or "{}" cameras)",
                                     left_camera_name, right_camera_name)
                             .data());
  }
  cameras_.Open();
  if (!cameras_.IsOpen()) {
    throw std::exception("Can not connect to cameras");
  }
  for (size_t i = 0; i < cameras_.GetSize(); ++i) {
    auto &node_map = cameras_[i].GetNodeMap();
    Pylon::CEnumParameter(node_map, "ExposureTimeMode")
        .SetValue(exposure_mode.data());
    Pylon::CFloatParameter(node_map, "ExposureTimeAbs").SetValue(exposure_time);
    Pylon::CEnumParameter(node_map, "PixelFormat")
        .SetValue(pixel_format.data());
    Pylon::CIntegerParameter(node_map, "Width").TrySetToMaximum();
    Pylon::CIntegerParameter(node_map, "Height").TrySetToMaximum();
    Pylon::CBooleanParameter(node_map, "CenterX").SetValue(true);
    Pylon::CBooleanParameter(node_map, "CenterY").SetValue(true);
  }
  // Save logic start
}
