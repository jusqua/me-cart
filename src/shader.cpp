#include <shader.hpp>

// Shader constructor
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  int success;
  char log[512];
  // Import shaders
  auto vertexShader = importShader(vertexPath, GL_VERTEX_SHADER);
  auto fragmentShader = importShader(fragmentPath, GL_FRAGMENT_SHADER);

  // Attach shaders to program
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  // Check for link errors
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, log);
    std::cerr << "ERROR: Failed to link shaders\n"
              << log << std::endl;
  }

  // Delete shaders, they are not useful after attaching
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

// Shader destructor
Shader::~Shader() {
  destroy();
}

// Activate shader program
void Shader::activate(void) {
  glUseProgram(ID);
}

// Destroy shader program
void Shader::destroy(void) {
  glDeleteProgram(ID);
}
