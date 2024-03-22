#ifndef MECART_SHADER_HPP
#define MECART_SHADER_HPP

#include <GL/glew.h>
#include <utils.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
 public:
  unsigned int ID;
  Shader(const char *vertexPath, const char *fragmentPath);
  Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath);
  ~Shader(void);

  void activate(void);
  void destroy(void);

  void setUniform(const char *name, int value);
  void setUniform(const char *name, float value);
  void setUniform(const char *name, glm::vec3 value);
  void setUniform(const char *name, glm::mat4 value);

 private:
  void linkShaders(std::vector<unsigned int> shaders);
};

#endif  // !MECART_SHADER_HPP
