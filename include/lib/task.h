#pragma once

#include <functional>
#include <string>

namespace falcon::tasks {
struct Task {
  std::string name;
  std::function<bool()> func;
};
};  // namespace falcon::tasks