#define STB_IMAGE_IMPLEMENTATION
#include <utils.hpp>
#include <stb_image.h>

// Block vertex object array
// clang-format off
const float BLOCK_VERTICES[] = {
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
// clang-format on

// Size of block vertex object array
const GLsizeiptr BLOCK_VERTICES_SIZE = sizeof(BLOCK_VERTICES);

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

  // Get file description if exists
  if (source.find("#", 0) != SIZE_MAX) {
    std::getline(ssource, line);
    std::istringstream(line) >> pgm.description;
  }

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
  pgm.content = std::vector<std::vector<int>>(pgm.height);
  for (size_t i = 0; i < pgm.height; i++) {
    std::getline(ssource, line);
    sline = std::istringstream(line);
    pgm.content[i] = std::vector<int>(pgm.width);
    for (size_t j = 0; j < pgm.width; j++) {
      std::getline(sline, token, ' ');
      std::istringstream(token) >> pgm.content[i][j];
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

unsigned int importTexture(char const *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (!data) {
    stbi_image_free(data);
    throw std::runtime_error(ERROR_PREFIX + "Failed to load texture from path");
  }

  GLenum format;
  if (nrComponents == 1)
    format = GL_RED;
  else if (nrComponents == 3)
    format = GL_RGB;
  else if (nrComponents == 4)
    format = GL_RGBA;

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  return textureID;
}
