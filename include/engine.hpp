#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

const static int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 800;
const static char *DEFAULT_WINDOW_PREFIX = "Me, Cart";

class Engine {
 public:
  Engine();
  ~Engine();

  void start(void);

 private:
  GLFWwindow *window;

  void pollEvents(void);
  void pollFramebufferEvents(void);
  void pollKeyboardEvents(void);
};
