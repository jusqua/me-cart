#ifndef MECART_UTILS_HPP
#define MECART_UTILS_HPP

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

typedef struct {
  int width, height;
  std::string description;
  int max;
  std::vector<int> content;
} pgm_t;

std::string importSource(const char *path);
pgm_t importPGM(const char *path);
unsigned int importShader(const char *path, GLenum type);

#endif  // !MECART_UTILS_HPP
