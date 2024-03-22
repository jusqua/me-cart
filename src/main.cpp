#include <engine.hpp>

int main(int argc, char *argv[]) {
  const char *terrainPath = DEFAULT_TERRAIN_PATH;
  Engine *engine;
  pgm_t terrain;

  if (argc > 2) {
    std::cerr << ERROR_PREFIX << "Too many arguments" << std::endl;
    std::cout << USAGE_PREFIX << "./me-cart [terrain file path]" << std::endl;
    return EXIT_FAILURE;
  }

  if (argc == 2)
    terrainPath = argv[1];

  try {
    terrain = importPGM(terrainPath);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    terrain = importPGM(terrainPath);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    engine = new Engine(terrain);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  engine->init();
  return EXIT_SUCCESS;
}
