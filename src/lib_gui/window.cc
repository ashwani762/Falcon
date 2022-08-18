#include <GLFW/glfw3.h>
#include <fonts/IconsCodicon.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <logger.h>
#include <views.h>
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

void Window::loop() {
  while (!glfwWindowShouldClose(m_window)) {
    glfwWaitEvents();
  }
}

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
      glfwCreateWindow(1366, 768, m_windowTitle.c_str(), nullptr, nullptr);

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

      glfwSetWindowPos(m_window, monitorX + (mode->width - windowWidth) / 2,
                       monitorY + (mode->height - windowHeight) / 2);
    }
  }

  glfwShowWindow(m_window);
}

void Window::initImGUI() {
  auto fonts = views::View::getFontAtlas();

  GImGui = ImGui::CreateContext(fonts);

  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 1.0f;
  style.WindowRounding = 1.0f;

  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;

    for (auto& entry : fonts->ConfigData) io.Fonts->ConfigData.push_back(entry);

  {
    GLsizei width, height;
    unsigned char* fontData;

    io.Fonts->GetTexDataAsRGBA32(&fontData, &width, &height);

    // Create new font atlas
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8,
                 GL_UNSIGNED_INT, fontData);
    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(tex));
  }

  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.IndentSpacing = 10.0F;

    ImGui_ImplGlfw_InitForOpenGL(this->m_window, true);

#if defined(OS_MACOS)
  ImGui_ImplOpenGL3_Init("#version 150");
#else
  ImGui_ImplOpenGL3_Init("#version 130");
#endif

}

void Window::exitGLFW() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::exitImGUI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace falcon