#pragma once

#include "program_arguments.h"
#include "scan_handler.h"

namespace sc {
class runner {
  program_arguments options_{};

  auto do_scan_branch() -> data_points;
  auto do_data_load_branch() -> data_points;

public:
  auto start(int argc, char *argv[]) -> int;
};
} // namespace sc
