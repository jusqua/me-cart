#ifndef MECART_ENGINE_H
#define MECART_ENGINE_H

#include <window.hpp>
#include <camera.hpp>
#include <shader.hpp>
#include <ebo.hpp>
#include <vbo.hpp>
#include <vao.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Engine {
 public:
  Engine(pgm_t terrain);

  void init();
  void framebufferSizeCallback(int width, int height);
  void cursorPosCallback(double xpos, double ypos);
  void scrollCallback(double xoffset, double yoffset);
  void processKeyboardEvents(void);

  static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto engine = (Engine *)glfwGetWindowUserPointer(window);
    engine->framebufferSizeCallback(width, height);
  }
  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    auto engine = (Engine *)glfwGetWindowUserPointer(window);
    engine->cursorPosCallback(xpos, ypos);
  }
  static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    auto engine = (Engine *)glfwGetWindowUserPointer(window);
    engine->scrollCallback(xoffset, yoffset);
  }

 private:
  Window window;
  Camera camera;
  pgm_t terrain;
  float deltaTime;
  float lastTime;
  double lastX;
  double lastY;
  bool firstCursor = true;
};

#endif  // !MECART_ENGINE_H
