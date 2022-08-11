#include <GLFW/glfw3.h>
#include <fonts/IconsCodicon.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <logger.h>
#include <window.h>

namespace falcon {

Window::Window() {
  initGLFW();
  initImGUI();
}

Window::~Window() {
  exitGLFW();
  exitImGUI();
}

void Window::loop() {}

void Window::initGLFW() {
  glfwSetErrorCallback([](int error, const char* desc) {
    log::error("GLFW Error [{}] : {}", error, desc);
  });

  if (!glfwInit()) {
    log::fatal("Failed to initalize GLFW");
    std::abort();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  m_windowTitle = "Falcon";

  m_window =
      glfwCreateWindow(1280, 720, m_windowTitle.c_str(), nullptr, nullptr);
  glfwShowWindow(this->m_window);

  glfwSetWindowUserPointer(m_window, this);

  if (!m_window) {
    log::fatal("Failed to create GLFW window");
    std::abort();
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();

  if (monitor) {
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode) {
      int monitorX, monitorY;
      glfwGetWindowPos(m_window, &monitorX, &monitorY);

      int windowHeight, windowWidth;
      glfwGetWindowSize(m_window, &windowWidth, &windowHeight);

      log::info("Window size {} {}", mode->width, mode->height);

      glfwSetWindowPos(m_window, monitorX + (mode->width - windowWidth) / 2,
                       monitorY + (mode->height - windowHeight) / 2);
    }
  }

  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();

    glfwSwapBuffers(m_window);
  }
}

void Window::initImGUI() {}

void Window::exitGLFW() {}

void Window::exitImGUI() {}

}  // namespace falcon