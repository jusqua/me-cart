#ifndef MECART_ENGINE_H
#define MECART_ENGINE_H

#define STB_IMAGE_IMPLEMENTATION

#include <window.hpp>
#include <camera.hpp>
#include <shader.hpp>
#include <ebo.hpp>
#include <vbo.hpp>
#include <vao.hpp>
#include <stb_image.h>

class Engine {
 public:
  Engine(pgm_t terrain);

  void init();
  void framebufferSizeCallback(int width, int height);

  static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto engine = (Engine *)glfwGetWindowUserPointer(window);
    engine->framebufferSizeCallback(width, height);
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
