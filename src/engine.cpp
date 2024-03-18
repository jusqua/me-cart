#include <engine.hpp>

// Engine constructor
Engine::Engine(const char *path) {
  // Initialize GLFW
  if (!glfwInit()) exit(EXIT_FAILURE);

  // Setup hint to specify OpenGL context, profile and version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window with default values and fullscreen mode
  window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH,
                            DEFAULT_WINDOW_HEIGHT,
                            DEFAULT_WINDOW_PREFIX,
                            glfwGetPrimaryMonitor(),
                            NULL);

  // Check for window initialization errors
  if (window == NULL) {
    std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Attach window to context
  glfwMakeContextCurrent(window);
  // Hide cursor and attach to center of screen
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // Set user pointer for callbacks
  glfwSetWindowUserPointer(window, this);

  // Setup OpenGL extensions
  auto glewInitResult = glewInit();

  // Check for importing errors
  if (glewInitResult != GLEW_OK) {
    std::cerr << "ERROR: " << glewGetErrorString(glewInitResult) << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackWrapper);
  glfwSetCursorPosCallback(window, cursorPosCallbackWrapper);
  glfwSetScrollCallback(window, scrollCallbackWrapper);
  glfwSetKeyCallback(window, keyCallbackWrapper);

  terrain = importPGM(path);
  camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  lastX = DEFAULT_WINDOW_WIDTH / 2.0f;
  lastY = DEFAULT_WINDOW_HEIGHT / 2.0f;
  deltaTime = glfwGetTime();
  lastTime = glfwGetTime();
  firstCursor = true;
}

// Engine destructor
Engine::~Engine() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

// Engine main loop
void Engine::init(void) {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

// Retrive framebuffer reshape events
void Engine::framebufferSizeCallback(int width, int height) {
  // Scale screen based on framebuffer dimensions
  glViewport(0, 0, width, height);
}

// Retrive mouse cursor events
void Engine::cursorPosCallback(double xpos, double ypos) {
  if (firstCursor) {
    lastX = xpos;
    lastY = ypos;
    firstCursor = false;
  }

  float xoffset = (xpos - lastX);
  float yoffset = (lastY - ypos);

  camera.processCursorEvents(xoffset, yoffset);
}

// Retrive mouse scroll events
void Engine::scrollCallback(double xoffset, double yoffset) {
  camera.processScrollEvents(yoffset);
}

// Retrive keyboard activation events
void Engine::keyCallback(int key, int scancode, int action, int mods) {
  // Press Escape to quit
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Wrappers
void framebufferSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  auto engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->framebufferSizeCallback(width, height);
}
void cursorPosCallbackWrapper(GLFWwindow *window, double xpos, double ypos) {
  auto engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->cursorPosCallback(xpos, ypos);
}
void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  auto engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->scrollCallback(xoffset, yoffset);
}
void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->keyCallback(key, scancode, action, mods);
}
