#ifndef MECART_WINDOW_HPP
#define MECART_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utils.hpp>

const static int DEFAULT_WINDOW_WIDTH = 1200, DEFAULT_WINDOW_HEIGHT = 800;
const static char *DEFAULT_WINDOW_PREFIX = "Me, Cart";

class Window {
 public:
  int width;
  int height;

  Window();
  ~Window(void);

  bool getKeyBehavior(int key, int action);

  bool shouldClose(void);
  void close(void);
  void swap(void);
  void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
  void setCursorPosCallback(GLFWcursorposfun callback);
  void setScrollCallback(GLFWscrollfun callback);

 private:
  GLFWwindow *context;
};

#endif  // !MECART_WINDOW_HPP
