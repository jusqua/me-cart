#include <utils.hpp>

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

  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR: Failed to source path (" << path << ")\n"
              << e.what() << std::endl;
  }

  return source;
}

pgm_t importPGM(const char *path) {
  auto source = importSource(path);
  std::istringstream ssource(source), sline;
  std::string line, token;
  pgm_t pgm;

  // Get file type
  std::getline(ssource, line);
  if (line.compare("P2")) {
    std::cerr << "ERROR: File format not supported" << std::endl;
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
  pgm.content = std::vector<unsigned char>(pgm.width * pgm.height);
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
