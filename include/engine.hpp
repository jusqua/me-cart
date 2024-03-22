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
#include <ebo.hpp>
#include <vbo.hpp>
#include <vao.hpp>

const static int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 800;
const static char *DEFAULT_WINDOW_PREFIX = "Me, Cart";
const static char *DEFAULT_TERRAIN_PATH = "resources/terrain/default.pgm";

void framebufferSizeCallbackWrapper(GLFWwindow *window, int width, int height);
void cursorPosCallbackWrapper(GLFWwindow *window, double xpos, double ypos);
void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset);

class Engine {
 public:
  Engine(pgm_t terrain);
  ~Engine(void);

  void init();
  void framebufferSizeCallback(int width, int height);
  void cursorPosCallback(double xpos, double ypos);
  void scrollCallback(double xoffset, double yoffset);
  void processKeyboardEvents(void);

 private:
  GLFWwindow *window;
  int windowWidth;
  int windowHeight;
  Camera camera;
  pgm_t terrain;
  float deltaTime;
  float lastTime;
  double lastX;
  double lastY;
  bool firstCursor;
};

#endif  // !MECART_ENGINE_H
