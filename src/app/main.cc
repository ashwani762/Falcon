#include <preRunTasks.h>
#include <window.h>
#include <logger.h>

#include <iostream>

auto main(int argc, char **argv) -> int {
  for (const auto &[preRunStr, action] : falcon::preTasks::tasks()) {
    falcon::log::info(preRunStr);
    action();
  }
  falcon::Window window;
  window.loop();

  return EXIT_SUCCESS;
}