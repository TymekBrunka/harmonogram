#pragma once
#include <bettergl/GLTypes.hpp>
#include <glad/glad.h>
namespace bgl {

Result<Shader> createShader(GLenum shader_type, const char *source);
Result<Program> linkProgram(Program program, const char *name = NULL);
Result<Program> createProgram(const char *name, const char* vertex_shader_source, const char* fragment_shader_source);

} // namespace bgl
