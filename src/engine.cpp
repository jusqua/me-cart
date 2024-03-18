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
  // Enable depth simulation
  glEnable(GL_DEPTH_TEST);

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
  windowWidth = DEFAULT_WINDOW_WIDTH;
  windowHeight = DEFAULT_WINDOW_HEIGHT;
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
  // clang-format off
  float vertices[] = {
      0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 3, 2,
    4, 5, 6,
    4, 7, 6,
    0, 1, 5,
    0, 4, 5,
    3, 2, 6,
    3, 7, 6,
    0, 3, 7,
    0, 4, 7,
    1, 2, 6,
    1, 5, 6,
  };
  // clang-format on

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  Shader program("resources/shaders/default.vert",
                 "resources/shaders/default.frag");

  while (!glfwWindowShouldClose(window)) {
    auto currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.activate();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    auto view = camera.getView();
    auto projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    auto model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(program.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

// Retrive framebuffer reshape events
void Engine::framebufferSizeCallback(int width, int height) {
  // Scale screen based on framebuffer dimensions
  windowWidth = width;
  windowHeight = height;
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
  // Process camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboardEvents(CAMERA_MOVEMENT::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboardEvents(CAMERA_MOVEMENT::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboardEvents(CAMERA_MOVEMENT::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboardEvents(CAMERA_MOVEMENT::RIGHT, deltaTime);
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
