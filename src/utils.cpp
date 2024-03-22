#include <utils.hpp>

// Import source file from path
std::string importSource(const char *path) {
  std::string source;
  std::ifstream file;

  // Make file throw exceptions
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // Open file stream
    file.open(path);
    std::stringstream stream;
    // Throw file to string stream
    stream << file.rdbuf();
    // Close file
    file.close();
    // Attach string from string
    source = stream.str();
  } catch (const std::ifstream::failure &_) {
    throw std::runtime_error(ERROR_PREFIX + "File not found or failed to load from source");
  }

  return source;
}

// Import PGM from path
pgm_t importPGM(const char *path) {
  auto source = importSource(path);
  std::istringstream ssource(source), sline;
  std::string line, token;
  pgm_t pgm;

  // Get file type
  std::getline(ssource, line);
  if (line.compare("P2")) {
    throw std::runtime_error(ERROR_PREFIX + "File does not match PGM format");
  }

  // Ignore file description
  std::getline(ssource, line);
  std::istringstream(line) >> pgm.description;

  // Get content dimensions
  std::getline(ssource, line);
  sline = std::istringstream(line);
  std::getline(sline, token, ' ');
  std::istringstream(token) >> pgm.width;
  std::getline(sline, token, ' ');
  std::istringstream(token) >> pgm.height;

  // Get max content value
  std::getline(ssource, line);
  std::istringstream(line) >> pgm.max;

  // Get content
  pgm.content = std::vector<int>(pgm.width * pgm.height);
  for (size_t i = 0; i < pgm.height; i++) {
    std::getline(ssource, line);
    sline = std::istringstream(line);
    for (size_t j = 0; j < pgm.width; j++) {
      std::getline(sline, token, ' ');
      std::istringstream(token) >> pgm.content[(pgm.width * i) + j];
    }
  }

  return pgm;
}

// Import shaders from path
unsigned int importShader(const char *path, GLenum type) {
  int success;
  char log[512];
  unsigned int shader;
  // Import source
  auto source = importSource(path);
  auto code = source.c_str();

  // Create shader and compile
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &code, NULL);
  glCompileShader(shader);

  // Check for compile errors
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, log);
    throw std::runtime_error(ERROR_PREFIX + log);
  }

  return shader;
}
