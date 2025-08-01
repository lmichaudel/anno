#include "platform/window.hpp"

#include <cstdio>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bx/platform.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#endif
#include "GLFW/glfw3native.h"

#include <GLFW/glfw3native.h>

#include "gfx/imgui/imgui_impl_bgfx.h"
#include "log/log.hpp"

#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

static bgfx::PlatformData get_platform_data(GLFWwindow* window) {
  bgfx::PlatformData pd{};

#if BX_PLATFORM_LINUX
  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    pd.ndt = glfwGetWaylandDisplay();
    pd.nwh = glfwGetWaylandWindow(window);
    pd.type = bgfx::NativeWindowHandleType::Wayland;
  } else {
    pd.ndt = glfwGetX11Display();
    pd.nwh = reinterpret_cast<void*>(glfwGetX11Window(window));
    pd.type = bgfx::NativeWindowHandleType::Default;
  }
#elif BX_PLATFORM_OSX
  pd.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
  pd.nwh = glfwGetWin32Window(window);
#endif

  return pd;
}

namespace lm {

  constexpr uint32_t RESET_FLAGS = BGFX_RESET_VSYNC | BGFX_RESET_SRGB_BACKBUFFER;

  void Window::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    bgfx::reset(width, height, RESET_FLAGS);
  }

  void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    p_window->m_input.key_callback(key, scancode, action, mods);
  }

  void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    const auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    p_window->m_input.mouse_button_callback(button, action, mods);
  }

  void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
    const auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    p_window->m_input.cursor_position_callback(x, y);
  }

  Window::Window(Input& input) : m_input(input) {
#ifdef BX_PLATFORM_LINUX
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if (!glfwInit()) {
      LOG_AND_THROW_FATAL("Could not initialize GLFW.");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(1280, 720, "dev", nullptr, nullptr);

    if (!m_window) {
      LOG_AND_THROW_FATAL("Could not create a window.");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetKeyCallback(m_window, key_callback);

    bgfx::Init init;
    init.type = bgfx::RendererType::Vulkan;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = RESET_FLAGS;
    init.platformData = get_platform_data(m_window);

    if (!bgfx::init(init)) {
      glfwDestroyWindow(m_window);
      glfwTerminate();
      LOG_AND_THROW_FATAL("Could not initialize bgfx.");
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    ImGui::CreateContext();

    ImGui_Implbgfx_Init(255);
    // TODO
    ImGui_ImplGlfw_InitForVulkan(m_window, true);
  }

  Window::~Window() {
    ImGui_Implbgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    bgfx::shutdown();

    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  bool Window::should_close() const { return glfwWindowShouldClose(m_window); }

  void Window::poll() const {
    m_input.poll();
    glfwPollEvents();
  }

  uint16_t Window::width() const {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return width;
  }

  uint16_t Window::height() const {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return height;
  }
} // namespace lm