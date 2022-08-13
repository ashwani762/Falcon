#pragma once
#include <string>
#include <vector>

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

  bool m_mouseButtonDown;
  std::vector<int> m_pressedKeys;

  double m_lastFrameTime = 0;
};
}  // namespace falcon