#ifndef MECART_ENGINE_H
#define MECART_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils.hpp>
#include <camera.hpp>
#include <shader.hpp>

const static int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 800;
const static char *DEFAULT_WINDOW_PREFIX = "Me, Cart";
const static char *DEFAULT_TERRAIN_PATH = "resources/terrain/default.pgm";

void framebufferSizeCallbackWrapper(GLFWwindow *window, int width, int height);
void cursorPosCallbackWrapper(GLFWwindow *window, double xpos, double ypos);
void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset);
void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action, int mods);

class Engine {
 public:
  Engine(const char *path = DEFAULT_TERRAIN_PATH);
  ~Engine();

  void init();
  void framebufferSizeCallback(int width, int height);
  void cursorPosCallback(double xpos, double ypos);
  void scrollCallback(double xoffset, double yoffset);
  void keyCallback(int key, int scancode, int action, int mods);

 private:
  GLFWwindow *window;
  Camera camera;
  pgm_t terrain;
  float deltaTime, lastTime;
  double lastX, lastY;
  bool firstCursor;
};

#endif  // !MECART_ENGINE_H
