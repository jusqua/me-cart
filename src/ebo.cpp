#include <ebo.hpp>

EBO::EBO(GLfloat *indices, GLsizeiptr size) {
  glGenBuffers(1, &ID);
  this->bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  this->unbind();
}

EBO::~EBO(void) {
  destroy();
}

void EBO::bind(void) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind(void) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy(void) {
  glDeleteBuffers(1, &ID);
}
