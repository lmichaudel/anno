#include "gfx/program.hpp"

#include "bgfx/bgfx.h"
#include "glm/gtc/type_ptr.hpp"
#include "log/log.hpp"

#include <cstdio>

static bgfx::ShaderHandle load_shader(std::string_view path) {
  FILE* file = fopen(path.data(), "rb");

  if (!file) {
    LOG_AND_THROW_FATAL("Failed to open shader file {}.", path.data());
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (size <= 0) {
    fclose(file);
    LOG_AND_THROW_FATAL("Failed to read shader file {}.", path.data());
  }

  // Allocate memory and read file
  const auto data = new char[size];
  const size_t bytes_read = fread(data, 1, size, file);
  fclose(file);

  if (bytes_read != size) {
    delete[] data;
    LOG_AND_THROW_FATAL("Failed to read shader file {}.", path.data());
  }

  LOG_DEBUG("Loaded shader file {}. ({} bytes)", path, size);

  // Create shader from binary data
  const bgfx::Memory* mem = bgfx::copy(data, size);
  delete[] data;

  const bgfx::ShaderHandle handle = bgfx::createShader(mem);

  if (!bgfx::isValid(handle)) {
    LOG_AND_THROW_FATAL("Failed to create shader from file {}.", path.data());
  }

  return handle;
}

namespace lm {

  static std::string get_shader_prefix() {
    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::OpenGL:
      return "glsl";
    case bgfx::RendererType::OpenGLES:
      return "essl";
    case bgfx::RendererType::Vulkan:
      return "spirv";
    case bgfx::RendererType::Metal:
      return "metal";
    default:
      return ""; // TODO
    }
  }

  Program::Program(const std::string_view name) {
    const bgfx::ShaderHandle vsh = load_shader("res/generated/shaders/" + std::string(name) + "/" + get_shader_prefix() + "/" + std::string(name) + ".vs.sc.bin");
    const bgfx::ShaderHandle fsh = load_shader("res/generated/shaders/" + std::string(name) + "/" + get_shader_prefix() + "/" + std::string(name) + ".fs.sc.bin");
    m_handle = bgfx::createProgram(vsh, fsh, true);
  }

  Program::~Program() {
    if (bgfx::isValid(m_handle)) {
      bgfx::destroy(m_handle);
    }
  }

  Program::operator bgfx::ProgramHandle() const {
    return m_handle;
  }

  Program::Program(Program&& other) noexcept : m_handle(other.m_handle) { other.m_handle = {bgfx::kInvalidHandle}; }

  Program& Program::operator=(Program&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
      }

      m_handle = other.m_handle;
      other.m_handle.idx = bgfx::kInvalidHandle;
    }
    return *this;
  }

  void Program::add_uniform(std::string identifier, bgfx::UniformType::Enum type) {
    Uniform uniform = {identifier, type};
    m_uniforms.emplace(identifier, std::move(uniform));
  }

  void Program::add_sampler(std::string identifier, int stage) {
    Uniform sampler = {identifier, bgfx::UniformType::Sampler};
    m_samplers.emplace(identifier, std::make_pair(std::move(sampler), stage));
  }

  void Program::set_uniform(const std::string_view identifier, const void* data) const {
    auto search = m_uniforms.find(identifier.data());
    if (search != m_uniforms.end()) {
      search->second.set_data(data);
    }
  }

  void Program::set_uniform(const std::string_view identifier, const glm::vec4& data) const {
    set_uniform(identifier, glm::value_ptr(data));
  }

  void Program::set_sampler(const std::string_view identifier, const Texture& texture) const {
    auto search = m_samplers.find(identifier.data());
    if (search != m_samplers.end()) {
      search->second.first.bind_texture(texture, search->second.second);
    }
  }

} // namespace lm