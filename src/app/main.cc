#include <window.h>

#include <iostream>

auto main(int argc, char **argv) -> int {
  falcon::Window window;
  window.loop();

  return EXIT_SUCCESS;
}