#include <window.hpp>

Window::Window(void) {
  // Initialize GLFW
  if (!glfwInit()) {
    throw std::runtime_error(ERROR_PREFIX + "Failed to initialize GLFW");
  }

  // Setup hint to specify OpenGL context, profile and version
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window with default values and fullscreen mode
  this->context = glfwCreateWindow(DEFAULT_WINDOW_WIDTH,
                                   DEFAULT_WINDOW_HEIGHT,
                                   DEFAULT_WINDOW_PREFIX,
                                   glfwGetPrimaryMonitor(),
                                   NULL);

  // Check for window initialization errors
  if (this->context == NULL) {
    glfwTerminate();
    throw std::runtime_error(ERROR_PREFIX + "Failed to create GLFW window");
  }

  // Attach window to context
  glfwMakeContextCurrent(this->context);

  // Setup OpenGL extensions
  glewExperimental = GL_TRUE;
  auto glewInitResult = glewInit();

  // Check for importing errors
  if (glewInitResult != GLEW_OK) {
    glfwDestroyWindow(this->context);
    glfwTerminate();
    throw std::runtime_error(ERROR_PREFIX + "Failed to import GLEW extensions");
  }

  // Hide cursor and attach to center of screen
  glfwSetInputMode(this->context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // Set user pointer for callbacks
  glfwSetWindowUserPointer(this->context, this);
  // Enable depth simulation
  glEnable(GL_DEPTH_TEST);

  this->width = DEFAULT_WINDOW_WIDTH;
  this->height = DEFAULT_WINDOW_HEIGHT;
}

Window::~Window(void) {
  glfwDestroyWindow(this->context);
  glfwTerminate();
}

bool Window::getKeyBehavior(int key, int action) {
  return glfwGetKey(this->context, key) == action;
}

bool Window::shouldClose(void) {
  return glfwWindowShouldClose(this->context);
}

void Window::close(void) {
  glfwSetWindowShouldClose(this->context, true);
}

void Window::swap(void) {
  glfwSwapBuffers(this->context);
  glfwPollEvents();
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
  glfwSetFramebufferSizeCallback(this->context, callback);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(this->context, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
  glfwSetScrollCallback(this->context, callback);
}
