#pragma once
#include <string>

struct GLFWwindow;

namespace falcon {

class Window {
 public:
  Window();
  ~Window();

  void loop();

 private:
  void initGLFW();
  void initImGUI();

  void exitGLFW();
  void exitImGUI();

  GLFWwindow *m_window = nullptr;
  std::string m_windowTitle;

  double m_lastFrameTime = 0;
};
}  // namespace falcon