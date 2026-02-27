#include "bettergl/GLTypes.hpp"
#include "bettergl/Program.hpp"
#include <GLFW/glfw3.h>
#include <bettergl/Debugging.hpp>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_boilerplate.hpp>
#include <nlohmann/json.hpp>
#include <pfd/portable-file-dialogs.h>

using json = nlohmann::json;

json obj;
std::string selected_index;

#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>

typedef float vec2[2];
typedef float vec3[3];

typedef struct Vertex {
  vec2 pos;
  vec3 col;
} Vertex;

std::string zmiana1;
std::string zmiana2;
std::string awarie;

std::string fpath;

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
      glfwCreateWindow(700, 500, "OpenGL Triangle", NULL, NULL);
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

  iminit(window, true);

  zmiana1.resize(4096);
  zmiana2.resize(4096);
  awarie.resize(4096);

  std::string data("0000-00-00");

  while (!glfwWindowShouldClose(window)) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    imNewFrame();
    setupDocking();

    ImGui::GetStyle().FontScaleMain = 0.75 + 0.2 * (((float)width) / 1200.0);
    if (ImGui::Begin("lista")) {
      ImGui::Text("hello world\n");
      ImGui::InputTextMultiline("Zmiana pierwsza", zmiana1.data(), 4096);
      ImGui::InputTextMultiline("Zmiana dróga", zmiana2.data(), 4096);
      ImGui::InputTextMultiline("awarie", awarie.data(), 4096);
    }
    ImGui::End();

    // ImGui::ShowStyleEditor();
    // ImGui::ShowStyleSelector("hello world");

    if (ImGui::Begin("edytor")) {
      if (ImGui::BeginMenu("plik")) {
        if (ImGui::MenuItem("załaduj")) {
          pfd::open_file f = pfd::open_file(
              "Wybierz plik z modelem 3D", pfd::path::home(),
              {"plik json harmonogramu (.harm.json)", "*.harm.json",
               "plik json (.json)", "*.json", "Wszystkie pliki", "*"});
          obj = json::parse(std::ifstream(f.result()[0]));
          selected_index = std::string();
          fpath = f.result()[0];
        }
        ImGui::EndMenu();
      }

      ImGui::InputText("##data", data.data(), 11);
      if (ImGui::Button("dodaj wpis")) {
        std::cout << "t\n";
        if (!obj.contains(data)) {
          std::cout << "tutaj\n";
          obj[data] = {"", ""};
        }
      }

      if (ImGui::Button("usuń zaznaczony")) {
        if (!selected_index.empty()) {
          obj.erase(selected_index);
          selected_index = "";
        }
      }

      bool showOnlyAbnormal;
      ImGui::Checkbox("filtruj awarie", &showOnlyAbnormal);

      if (ImGui::Button("zapisz")) {
          std::ofstream(fpath) << obj.dump(4);
      }

      if (ImGui::Button("zapisz jako")) {
          pfd::save_file f = pfd::save_file(
              "Wybierz plik z modelem 3D", pfd::path::home(),
              {"plik json harmonogramu (.harm.json)", "*.harm.json",
               "plik json (.json)", "*.json", "Wszystkie pliki", "*"});

          std::ofstream(f.result()) << obj.dump(4);
      }

      ImGui::BeginTable("e", 1);
      ImGui::TableSetupColumn("Zadania");
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
      ImGui::TableSetColumnIndex(0);
      for (auto &[key, value] : obj.items()) {
        if (showOnlyAbnormal && value[2] == "") continue;

        ImGui::Spacing();
        if (ImGui::Selectable(key.c_str())) {
          if (!selected_index.empty()) {
            auto &selected = obj[selected_index];
            selected[0] = zmiana1.c_str();
            selected[1] = zmiana2.c_str();
            selected[2] = awarie.c_str();
          }
          selected_index = key;
          auto &selected = value;

          if (selected[0].empty())
            zmiana1.clear();
          else {
            zmiana1 = selected[0];
          }
          if (zmiana1.capacity() < 4096)
            zmiana1.reserve(4096);

          if (selected[1].empty())
            zmiana2.clear();
          else {
            zmiana2 = selected[1];
          }
          if (zmiana2.capacity() < 4096)
            zmiana2.reserve(4096);

          if (selected[2].empty())
            awarie.clear();
          else {
            awarie = selected[2];
          }
          if (awarie.capacity() < 4096)
            awarie.reserve(4096);
        }
      }
      ImGui::PopStyleVar(1);
      ImGui::EndTable();
    }
    ImGui::End();

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
