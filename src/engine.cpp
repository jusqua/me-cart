#include <engine.hpp>

// Engine constructor
Engine::Engine(pgm_t _terrain) {
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
    glfwTerminate();
    throw std::runtime_error(ERROR_PREFIX + "Failed to create GLFW window");
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
    glfwDestroyWindow(window);
    glfwTerminate();
    throw std::runtime_error(ERROR_PREFIX + "Failed to import GLEW extensions");
  }

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackWrapper);
  glfwSetCursorPosCallback(window, cursorPosCallbackWrapper);
  glfwSetScrollCallback(window, scrollCallbackWrapper);

  terrain = _terrain;
  camera = Camera(glm::vec3(3.0f, 7.0f, 3.0f));
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
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };
  // clang-format on

  unsigned int lightSourceVAO, terrainVAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &lightSourceVAO);
  glBindVertexArray(lightSourceVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glGenVertexArrays(1, &terrainVAO);
  glBindVertexArray(terrainVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader lightProgram("resources/shaders/light.vert",
                      "resources/shaders/light.frag");

  Shader terrainProgram("resources/shaders/default.vert",
                        "resources/shaders/default.frag");

  while (!glfwWindowShouldClose(window)) {
    auto currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = camera.getView();
    auto projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, 0.1f, 330.0f);

    terrainProgram.activate();
    glBindVertexArray(terrainVAO);

    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "camera_Position"), 1, glm::value_ptr(camera.position));

    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "light.direction"), 1, glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.5f)));
    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f)));
    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(terrainProgram.ID, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    glUniform1f(glGetUniformLocation(terrainProgram.ID, "material.shininess"), 32.0f);

    glUniformMatrix4fv(glGetUniformLocation(terrainProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(terrainProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    auto modelLocation = glGetUniformLocation(terrainProgram.ID, "model");
    for (int i = 0; i < terrain.height; i++) {
      auto right = glm::translate(glm::mat4(1.0f), glm::vec3(i * 1.0f, 0.0f, 0.0f));
      for (int j = 0; j < terrain.width; j++) {
        auto relief = terrain.content[terrain.width * i + j];
        auto model = glm::translate(right, glm::vec3(0.0f, relief * 1.0f, j * 1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    lightProgram.activate();
    glBindVertexArray(lightSourceVAO);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 330.0f, 0.0f));
    model = glm::scale(model, glm::vec3(25.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
    processKeyboardEvents();
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
void Engine::processKeyboardEvents(void) {
  // Press Escape to quit
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camera.movementSpeed = std::min(DEFAULT_SPEED * 10, camera.movementSpeed + DEFAULT_SPEED);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera.movementSpeed = std::max(DEFAULT_SPEED, camera.movementSpeed - DEFAULT_SPEED);
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
