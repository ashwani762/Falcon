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
    glfwPollEvents();
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

  glfwSetMouseButtonCallback(
      m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS)
          win->m_mouseButtonDown = true;
        else if (action == GLFW_RELEASE)
          win->m_mouseButtonDown = false;
      });

  glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode,
                                  int action, int mods) {
    auto keyName = glfwGetKeyName(key, scanCode);
    if (keyName) key = std::toupper(keyName[0]);

    auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    auto& io = ImGui::GetIO();

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      win->m_pressedKeys.push_back(key);
      io.KeysDown[key] = true;
      io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
      io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
      io.KeyAlt = (mods & GLFW_MOD_ALT) != 0;
    } else if (action == GLFW_RELEASE) {
      io.KeysDown[key] = false;
      io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
      io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
      io.KeyAlt = (mods & GLFW_MOD_ALT) != 0;
    }
  });

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

  //io.ConfigViewportsNoTaskBarIcon = false;
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

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