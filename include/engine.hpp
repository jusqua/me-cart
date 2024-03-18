#ifndef MECART_ENGINE_H
#define MECART_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utils.hpp>
#include <camera.hpp>

const static int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 800;
const static char *DEFAULT_WINDOW_PREFIX = "Me, Cart";
const static char *DEFAULT_TERRAIN_PATH = "resources/terrain/default.pgm";

class Engine {
 public:
  Engine(const char *path = DEFAULT_TERRAIN_PATH);
  ~Engine();

  void start();

 private:
  GLFWwindow *window;
  Camera camera;
  pgm_t terrain;

  void importTerrain(void);
  void pollEvents(void);
  void pollFramebufferEvents(void);
  void pollKeyboardEvents(void);
};

#endif  // !MECART_ENGINE_H
