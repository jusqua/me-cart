#ifndef MECART_UTILS_HPP
#define MECART_UTILS_HPP

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

const static std::string ERROR_PREFIX = "ERROR: ";
const static std::string USAGE_PREFIX = "USAGE: ";
const static char *DEFAULT_TERRAIN_PATH = "resources/terrain/default.pgm";

extern const float BLOCK_VERTICES[];
extern const GLsizeiptr BLOCK_VERTICES_SIZE;

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
