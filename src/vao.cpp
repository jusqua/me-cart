#include <vao.hpp>

VAO::VAO(void) {
  glGenVertexArrays(1, &ID);
}

VAO::~VAO(void) {
  this->destroy();
}

void VAO::linkAttrib(VBO &buffer, GLuint layout, GLuint components, GLenum type, GLsizeiptr stride, void *offset) {
  this->bind();
  buffer.bind();
  glVertexAttribPointer(layout, components, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  buffer.unbind();
  this->unbind();
}

void VAO::bind(void) {
  glBindVertexArray(ID);
}

void VAO::unbind(void) {
  glBindVertexArray(0);
}

void VAO::destroy(void) {
  glDeleteVertexArrays(1, &ID);
}
