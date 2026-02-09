#include "bettergl/GLTypes.hpp"
#include "bettergl/Program.hpp"
#include <GLFW/glfw3.h>
#include <bettergl/Debugging.hpp>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_boilerplate.hpp>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef float vec2[2];
typedef float vec3[3];

typedef struct Vertex {
  vec2 pos;
  vec3 col;
} Vertex;

static const Vertex vertices[3] = {{{-0.6f, -0.4f}, {1.f, 0.f, 0.f}},
                                   {{0.6f, -0.4f}, {0.f, 1.f, 0.f}},
                                   {{0.f, 0.6f}, {0.f, 0.f, 1.f}}};

static const char *vertex_shader_text =
    "#version 330\n"
    "uniform mat4 MVP;\n"
    "in vec3 vCol;\n"
    "in vec2 vPos;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 330\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    // apply gamma correction\n"
    "    float gamma = 2.2;\n"
    "    fragment.rgb = pow(color.rgb, vec3(1.0/gamma));\n"
    "}\n";

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL(); // only then we can load
  bgl::setUpDebugger();
  glfwSwapInterval(1);

  bgl::SCOPE = "main";

  // NOTE: OpenGL error checks have been omitted for brevity

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  // bgl::catchError("binding vbo");
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // const bgl::Shader vertex_shader =
  //     bgl::createShader(GL_VERTEX_SHADER, vertex_shader_text).unwrapExit();
  //
  // const bgl::Shader fragment_shader =
  //     bgl::createShader(GL_FRAGMENT_SHADER,
  //     fragment_shader_text).unwrapExit();

  // const bgl::Program program = glCreateProgram();
  // glAttachShader(program, vertex_shader);
  // glAttachShader(program, fragment_shader);
  // bgl::linkProgram(program);

  bgl::Program program = bgl::createProgram("main program", vertex_shader_text,
                                            fragment_shader_text)
                             .unwrapExit();

  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, pos));
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, col));

  iminit(window, true);

  while (!glfwWindowShouldClose(window)) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(program);
    // glBindVertexArray(vertex_array);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    imNewFrame();
    setupDocking();

    int old_size = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale *= 0.7 + 0.3 * (((float)width) / 1200.0);

    if (ImGui::Begin("lista")) {
      ImGui::Text("hello world\n");
    }
    ImGui::End();

    if (ImGui::Begin("edytor")) {
      ImGui::Text("hello world\n");
    }
    ImGui::End();

    ImGui::GetFont()->Scale = old_size;

    imrender();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  imdestroy();

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

//! [code]
