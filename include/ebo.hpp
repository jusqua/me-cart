#ifndef MECART_EBO_HPP
#define MECART_EBO_HPP

#include <GL/glew.h>

class EBO {
 public:
  GLuint ID;

  EBO(GLfloat *indices, GLsizeiptr size);
  ~EBO(void);

  void bind(void);
  void unbind(void);
  void destroy(void);
};

#endif  // !MECART_EBO_HPP
