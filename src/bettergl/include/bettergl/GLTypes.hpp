#pragma once
#include "bettergl/Debugging.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
namespace bgl {

// vertex buffer object
typedef unsigned int VBO;
// index buffer object
typedef unsigned int EBO;
// vertex array object
typedef unsigned int VAO;
// framebuffer object
typedef unsigned int FBO;
// renderbuffer object
typedef unsigned int RBO;
// uniform buffer object
typedef unsigned int UBO;

typedef unsigned int Texture2D;
typedef unsigned int CubeMap;

typedef unsigned int Program;
typedef unsigned int Shader;

typedef int Location;

// Result represents type that can hold either openGL object index or a error
// message
template <typename T> struct Result {
  T self;
  std::string error_msg;

  // returns stored object and prints error message if it is present;
  T unwrapPrint(std::source_location l = std::source_location::current()) {
    if (error_msg.empty())
      return self;

    std::cout << "\x1b[31mE\x1b[0m OpenGL \x1b[32m[\x1b[33m" << SCOPE
              << "\x1b[32m]\x1b[0m at \x1b[33m" << l.file_name()
              << "\x1b[31m:" << l.line() << ":" << l.column()
              << "\x1b[0m: " << error_msg << "\n";
    return self;
  }

  // returns stored object and exits program if error message is present;
  T unwrapExit(std::source_location l = std::source_location::current()) {
    if (error_msg.empty())
      return self;
    std::cout << "\x1b[31mE\x1b[0m OpenGL \x1b[32m[\x1b[33m" << SCOPE
              << "\x1b[32m]\x1b[0m at \x1b[33m" << l.file_name()
              << "\x1b[31m:" << l.line() << ":" << l.column()
              << "\x1b[0m: " << error_msg << "\n";
    exit(-1);
  }
};

} // namespace bgl
