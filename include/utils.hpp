#ifndef MECART_UTILS_HPP
#define MECART_UTILS_HPP

#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

typedef struct {
  int width, height;
  std::string description;
  unsigned char max;
  std::vector<unsigned char> content;
} pgm_t;

std::string importSource(const char *path);
pgm_t importPGM(const char *path);

#endif  // !MECART_UTILS_HPP
