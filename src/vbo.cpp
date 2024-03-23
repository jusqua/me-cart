#include <vbo.hpp>

VBO::VBO(const GLfloat *vertices, const GLsizeiptr size) {
  glGenBuffers(1, &ID);
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  this->unbind();
}

VBO::~VBO(void) {
  this->destroy();
}

void VBO::bind(void) {
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind(void) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy(void) {
  glDeleteBuffers(1, &ID);
}
