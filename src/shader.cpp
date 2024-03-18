#include <shader.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  int success;
  char log[512];
  auto vertexShader = importShader(vertexPath, GL_VERTEX_SHADER);
  auto fragmentShader = importShader(fragmentPath, GL_FRAGMENT_SHADER);

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, log);
    std::cerr << "ERROR: Failed to link shaders\n"
              << log << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader() {
  destroy();
}

void Shader::activate(void) {
  glUseProgram(ID);
}

void Shader::destroy(void) {
  glDeleteProgram(ID);
}
