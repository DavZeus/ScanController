#pragma once

#include "program_arguments.h"
#include "scan_handler.h"

namespace sc {
class runner {
  program_arguments options_{};

  auto do_scan_branch() const -> model_profiles<float>;
  // TODO: Wait for rewritten io_operations
  // auto do_data_load_branch() const -> model_profiles;

public:
  auto start(int argc, char *argv[]) -> int;
};
} // namespace sc
