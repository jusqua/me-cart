#ifndef MECART_SHADER_HPP
#define MECART_SHADER_HPP

#include <GL/glew.h>
#include <utils.hpp>

class Shader {
 public:
  unsigned int ID;
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  void activate(void);
  void destroy(void);
};

#endif  // !MECART_SHADER_HPP
