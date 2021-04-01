#pragma once

#include "program_arguments.h"
#include "scan_handler.h"

namespace sc {
class runner {
  program_arguments options_{};

  auto do_scan_branch() const -> model_profiles<float>;
  auto do_data_load_branch() const -> model_profiles<float>;

public:
  auto start(int argc, char *argv[]) -> int;
};
} // namespace sc
