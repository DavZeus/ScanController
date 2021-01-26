#pragma once

#include "options.h"

namespace sc {
class runner {
  options options_{};

public:
  auto start(int argc, char *argv[]) -> int;
};
} // namespace sc
