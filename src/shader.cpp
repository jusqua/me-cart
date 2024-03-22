#include <shader.hpp>

// Shader constructor with vertex and fragment shaders
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // Import shaders
  auto vertexShader = importShader(vertexPath, GL_VERTEX_SHADER);
  auto fragmentShader = importShader(fragmentPath, GL_FRAGMENT_SHADER);

  // Attach shaders to program
  this->linkShaders({vertexShader, fragmentShader});
}

// Shader constructor with vertex, geometry and fragment shaders
Shader::Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath) {
  // Import shaders
  auto vertexShader = importShader(vertexPath, GL_VERTEX_SHADER);
  auto geometryShader = importShader(geometryPath, GL_GEOMETRY_SHADER);
  auto fragmentShader = importShader(fragmentPath, GL_FRAGMENT_SHADER);

  // Attach shaders to program
  this->linkShaders({vertexShader, geometryShader, fragmentShader});
}

// Shader destructor
Shader::~Shader() {
  this->destroy();
}

// Activate shader program
void Shader::activate(void) {
  glUseProgram(this->ID);
}

// Destroy shader program
void Shader::destroy(void) {
  glDeleteProgram(this->ID);
}

void Shader::linkShaders(std::vector<unsigned int> shaders) {
  int success;
  char log[512];

  // Create shader program
  this->ID = glCreateProgram();
  for (auto shader : shaders)
    glAttachShader(this->ID, shader);
  glLinkProgram(this->ID);

  // Check for link errors
  glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->ID, 512, NULL, log);
    std::cerr << "ERROR: Failed to link shaders\n"
              << log << std::endl;
  }

  // Destroy unused shaders
  for (auto shader : shaders)
    glDeleteShader(shader);
}
