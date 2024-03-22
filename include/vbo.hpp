#ifndef MECART_VBO_HPP
#define MECART_VBO_HPP

#include <GL/glew.h>

class VBO {
 public:
  GLuint ID;

  VBO(GLfloat *vertices, GLsizeiptr size);
  ~VBO(void);

  void bind(void);
  void unbind(void);
  void destroy(void);
};

#endif  // !MECART_VBO_HPP
