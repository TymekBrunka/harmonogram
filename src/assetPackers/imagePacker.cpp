#include "stb_image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  char do_flip = *argv[5];
  stbi_set_flip_vertically_on_load(do_flip == 'y');
  int width, height, nrChannels;
  const char *varname = argv[1];
  const char *input = argv[2];
  std::string output;
  output.assign(argv[3], std::strlen(argv[3]));
  int channels = std::atoi(argv[4]);
  unsigned char *data = stbi_load(input, &width, &height, &nrChannels, channels);

  if (!data) {
    std::cout << "unable to load image: " << stbi_failure_reason() << "\n";
    // throw;
    return 1;
  }

  std::cout << " +----------Image packer-----------\n"
            << " | input file:  \x1b[36m" << input << "\x1b[0m\n"
            << " | output file: \x1b[36m" << output << "\x1b[0m\n"
            << " | width:       \x1b[33m" << width << "\x1b[0m\n"
            << " | height:      \x1b[33m" << height << "\x1b[0m\n"
            << " |---------------------\n"
            << " | number of channels:           \x1b[35m" << nrChannels << "\x1b[0m\n"
            << " | requested amount of channels: \x1b[35m" << channels << "\x1b[0m\n"
            << " +---------------------------------\n";

  int amountOfBytes = (width * height * 4);
  unsigned char *data_end = &data[amountOfBytes];

  char *sb = new char[amountOfBytes * 4];
  char *sbptr = sb;
  for (unsigned char *ptr = data; ptr < data_end; ptr++) {
    snprintf(sbptr, 5, "\\x%0.2x", *ptr);
    sbptr += 4;
  }

  std::string headerpath = output + ".hpp";
  std::string cppath = output + ".cpp";

  std::FILE *header = std::fopen(headerpath.c_str(), "w");
  std::fprintf(header,
               "#pragma once\n"
               "#include \"Texture2d.hpp\"\n"
               "extern RR::image_data %s;",
               varname);
  std::fclose(header);

  std::FILE *cppfile = std::fopen(cppath.c_str(), "w");
  std::fprintf(cppfile,
               "#include \"%s.hpp\"\n"
               "RR::image_data %s = {%d, %d, %d, (unsigned char*)\"%s\"};",
               output.c_str(), varname, width, height, nrChannels, sb);
  std::fclose(cppfile);
  stbi_image_free(data);
  delete[] sb;
}
