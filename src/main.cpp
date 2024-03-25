#include <engine.hpp>

int main(int argc, char *argv[]) {
  const char *terrainPath = DEFAULT_TERRAIN_PATH;
  const char *terrainTexturePath = DEFAULT_TERRAIN_TEXTURE;
  Engine *engine;
  pgm_t terrain;
  unsigned int terrainTexture;

  if (argc > 3) {
    std::cerr << ERROR_PREFIX << "Too many arguments" << std::endl;
    std::cout << USAGE_PREFIX << "./me-cart [terrain structure path] [terrain texture path]" << std::endl;
    return EXIT_FAILURE;
  }

  if (argc > 1)
    terrainPath = argv[1];

  if (argc > 2)
    terrainTexturePath = argv[2];

  try {
    engine = new Engine(terrainPath, terrainTexturePath);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  engine->init();
  return EXIT_SUCCESS;
}
