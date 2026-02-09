#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  const char *varname = argv[1];
  const char *input = argv[2];
  std::string output;
  output.assign(argv[3], std::strlen(argv[3]));

  std::fstream f;
  f.open(input);
  f.seekg(0, std::ios::end);
  int length = f.tellg();
  f.seekg(0, std::ios::beg);
  char *data = (char*)std::calloc(length, sizeof(char));

  f.read(data, length);
  f.close();

  std::cout << " +-----------Byte packer-----------\n"
            << " | input file:  \x1b[36m" << input << "\x1b[0m\n"
            << " | output file: \x1b[36m" << output << "\x1b[0m\n"
            << " | byte length: \x1b[35m" << length << "\x1b[0m\n"
            << " +---------------------------------\n";

  char *sb = new char[length * 4];
  char *data_end = &data[length - 1];
  char *sbptr = sb;
  for (char *ptr = data; ptr < data_end; ptr++) {
    snprintf(sbptr, 5, "\\x%0.2x", *ptr);
    sbptr += 4;
  }

  std::string headerpath = output + ".hpp";
  std::string cppath = output + ".cpp";

  std::FILE *header = std::fopen(headerpath.c_str(), "w");
  std::fprintf(header,
               "#pragma once\n"
               "extern const char* %s;",
               varname);
  std::fclose(header);

  std::FILE *cppfile = std::fopen(cppath.c_str(), "w");
  std::fprintf(cppfile,
               "#include \"%s\"\n"
               "const char* %s = \"%s\";",
               headerpath.c_str(), varname, sb);
  std::fclose(cppfile);
  free(data);
  delete[] sb;
}
