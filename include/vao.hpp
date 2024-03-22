#ifndef MECART_VAO_HPP
#define MECART_VAO_HPP

#include <GL/glew.h>
#include <vbo.hpp>

class VAO {
 public:
  GLuint ID;

  VAO(void);
  ~VAO(void);

  void linkAttrib(VBO &buffer, GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void *offset);
  void bind(void);
  void unbind(void);
  void destroy(void);
};

#endif  // !MECART_VAO_HPP
