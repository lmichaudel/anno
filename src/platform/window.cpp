#include "platform/window.hpp"

#include <cstdio>

#include <bgfx/bgfx.h>
#include <bx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

static bgfx::PlatformData get_platform_data(GLFWwindow* window) {
  bgfx::PlatformData pd{};

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  pd.ndt = glfwGetX11Display();
  pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
  pd.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
  pd.nwh = glfwGetWin32Window(window);
#endif

  return pd;
}

namespace lm {

  constexpr uint32_t RESET_FLAGS = BGFX_RESET_VSYNC | BGFX_RESET_SRGB_BACKBUFFER;

  void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    bgfx::reset(width, height, RESET_FLAGS);
  }

  Window::Window() {
#ifdef BX_PLATFORM_LINUX
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if (!glfwInit()) {
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(1280, 720, "dev", nullptr, nullptr);

    if (!m_window) {
      printf("BIG ERROR");
    }

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = RESET_FLAGS;
    init.platformData = get_platform_data(m_window);

    if (!bgfx::init(init)) {
      glfwDestroyWindow(m_window);
      glfwTerminate();
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
  }

  Window::~Window() {
    bgfx::shutdown();
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  bool Window::should_close() const { return glfwWindowShouldClose(m_window); }
  void Window::poll() const { glfwPollEvents(); }

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

}