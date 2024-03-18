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

  // Setup OpenGL extensions
  auto glewInitResult = glewInit();

  // Check for importing errors
  if (glewInitResult != GLEW_OK) {
    std::cerr << "ERROR: " << glewGetErrorString(glewInitResult) << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  terrain = importPGM(path);
  camera = Camera(0.0f, 0.0f, 3.0f);
}

// Engine destructor
Engine::~Engine() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

// Engine main loop
void Engine::start(void) {
  while (!glfwWindowShouldClose(window)) {
    pollEvents();
  }
}

// Retrive every event
void Engine::pollEvents(void) {
  pollKeyboardEvents();
  pollFramebufferEvents();
  glfwPollEvents();
}

// Retrive framebuffer reshape events
void Engine::pollFramebufferEvents(void) {
  int width, height;
  // Scale screen based on framebuffer dimensions
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

// Retrive keyboard activation events
void Engine::pollKeyboardEvents(void) {
  // Press Escape to quit
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
