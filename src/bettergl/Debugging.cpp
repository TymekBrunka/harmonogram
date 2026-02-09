#include <bettergl/Debugging.hpp>
namespace bgl {

const char *SCOPE = "";

inline void APIENTRY GLDEBUGFUNC(GLenum source_, GLenum type_, GLuint id,
                          GLenum severity_, GLsizei length,
                          const GLchar *message, const void *userParam) {
  switch (id) {
  case 131185: // glBufferData
    return;
  }

  const char *type;
  const char *severity;
  const char *source;

  switch (type_) {
  case GL_DEBUG_TYPE_ERROR_ARB:
    type = "\x1b[31mE\x1b[0m";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
    type = "\x1b[33mDB\x1b[0m";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
    type = "\x1b[31mUB\x1b[0m";
    break;
  case GL_DEBUG_TYPE_PORTABILITY_ARB:
    type = "\x1b[34mPORT\x1b[0m";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE_ARB:
    type = "\x1b[34mPERF\x1b[0m";
    break;
  }

  switch (severity_) {
  case GL_DEBUG_SEVERITY_LOW_ARB:
    severity = "\x1b[34mL\x1b[0m";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM_ARB:
    severity = "\x1b[33mM\x1b[0m";
    break;
  case GL_DEBUG_SEVERITY_HIGH_ARB:
    severity = "\x1b[31mH\x1b[0m";
    break;
  }

  switch (source_) {
  case GL_DEBUG_SOURCE_API_ARB:
    source = "API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
    source = "Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
    source = "Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
    source = "Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION_ARB:
    source = "Application";
    break;
  case GL_DEBUG_SOURCE_OTHER_ARB:
    source = "Other";
    break;
  }

  std::cout << type << "/" << severity << " \x1b[32m[\x1b[33m" << SCOPE
            << "\x1b[32m]\x1b[0m OpenGL source: \x1b[33m" << source
            << "\x1b[0m message: " << message << "\n";
}

bool setUpDebugger() {
  if (GLAD_GL_ARB_debug_output) {
    glDebugMessageCallbackARB(GLDEBUGFUNC, NULL);
    return true;
  }
  return false;
}

#ifndef BGL_REL_NO_LTO
template void catchError<>(const char *call_description,
                           std::source_location l);
#endif
} // namespace bgl
