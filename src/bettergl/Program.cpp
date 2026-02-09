#include "bettergl/Debugging.hpp"
#include "bettergl/GLTypes.hpp"
#include <bettergl/Program.hpp>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <string>
#include <string_view>
namespace bgl {

Result<Shader> createShader(GLenum shader_type, const char *source) {

  if (shader_type != GL_VERTEX_SHADER && shader_type != GL_FRAGMENT_SHADER &&
      shader_type != GL_GEOMETRY_SHADER) {
    return {0, std::string("shader creation error: invalid enum")};
  }

  Shader shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int compilation_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);

  std::stringstream error_msg;
  if (!compilation_status) {
    const char *shader_type_s;

    switch (shader_type) {
    case GL_VERTEX_SHADER:
      shader_type_s = "vertex";
      break;
    case GL_FRAGMENT_SHADER:
      shader_type_s = "fragment";
      break;
    case GL_GEOMETRY_SHADER:
      shader_type_s = "geometry";
      break;
    }
    error_msg << shader_type_s << " shader linking error: ";
    std::string shader_error_msg;
    shader_error_msg.resize(512);
    glGetShaderInfoLog(shader, 512, NULL, shader_error_msg.data());
    error_msg << shader_error_msg;
    glDeleteShader(shader);
  }

  return {shader, error_msg.str()};
}

Result<Program> linkProgram(Program program, const char *name) {
  glLinkProgram(program);
  int linking_status;
  glGetProgramiv(program, GL_LINK_STATUS, &linking_status);

  std::string error_msg;
  if (!linking_status) {
    std::string shader_type_s;

    error_msg = std::string("program '") + (name ? name : "") +
                std::string("' linking error: ");
    int msg_size = error_msg.size();
    error_msg.resize(590);
    glGetProgramInfoLog(program, 512, NULL, error_msg.data() + msg_size);
    glDeleteProgram(program);
  }

  return {program, error_msg};
}

// may not result program if shaders fail to compile
Result<Program> createProgram(const char *name,
                              const char *vertex_shader_source,
                              const char *fragment_shader_source) {

  Result<Shader> vertex_shader_ =
      createShader(GL_VERTEX_SHADER, vertex_shader_source);

  if (!vertex_shader_.error_msg.empty()) {
    vertex_shader_.error_msg =
        std::string("On program '") + name + "' " + vertex_shader_.error_msg;
    return vertex_shader_;
  }

  Result<Shader> fragment_shader_ =
      createShader(GL_FRAGMENT_SHADER, fragment_shader_source);

  if (!fragment_shader_.error_msg.empty()) {
    fragment_shader_.error_msg =
        std::string("On program '") + name + "' " + fragment_shader_.error_msg;
    return fragment_shader_;
  }

  Program program = glCreateProgram();
  glAttachShader(program, vertex_shader_.self);
  glAttachShader(program, fragment_shader_.self);

  return linkProgram(program, name);
}

} // namespace bgl
