#include <engine.hpp>

// Engine constructor
Engine::Engine(pgm_t terrain) {
  this->terrain = terrain;
  this->camera = Camera(glm::vec3(0.0f, terrain.content[terrain.height / 2 + terrain.width / 2], 0.0f));

  this->window.setFramebufferSizeCallback(Engine::framebufferSizeCallback);
  this->window.setCursorPosCallback(Engine::cursorPosCallback);
  this->window.setScrollCallback(Engine::scrollCallback);
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

  auto objectVBO = VBO(vertices, sizeof(vertices));

  auto terrainVAO = VAO();
  terrainVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  terrainVAO.linkAttrib(objectVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));

  auto lightVAO = VAO();
  lightVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);

  Shader lightProgram("resources/shaders/light.vert", "resources/shaders/light.frag");
  Shader terrainProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

  while (!window.shouldClose()) {
    auto currentTime = glfwGetTime();
    this->deltaTime = currentTime - this->lastTime;
    this->lastTime = currentTime;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = this->camera.getView();
    auto projection = glm::perspective(glm::radians(camera.fov),
                                       (float)this->window.width / this->window.height,
                                       0.1f,
                                       800.0f);

    terrainProgram.activate();
    terrainVAO.bind();

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
    for (int i = 0; i < this->terrain.height; i++) {
      auto start = glm::translate(glm::mat4(1.0f),
                                  glm::vec3(-this->terrain.height / 2.0f + i, 0.0f, -this->terrain.width / 2.0f));
      for (int j = 0; j < this->terrain.width; j++) {
        auto relief = this->terrain.content[this->terrain.width * i + j];
        auto model = glm::translate(start, glm::vec3(0.0f, relief * 1.0f, j * 1.0f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    lightProgram.activate();
    lightVAO.bind();
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 500.0f, 0.0f));
    model = glm::scale(model, glm::vec3(50.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(lightProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    window.swap();
    this->processKeyboardEvents();
  }
}

// Retrive framebuffer reshape events
void Engine::framebufferSizeCallback(int width, int height) {
  // Scale screen based on framebuffer dimensions
  this->window.width = width;
  this->window.height = height;
  glViewport(0, 0, width, height);
}

// Retrive mouse cursor events
void Engine::cursorPosCallback(double xpos, double ypos) {
  if (this->firstCursor) {
    this->lastX = xpos;
    this->lastY = ypos;
    this->firstCursor = false;
  }

  float xoffset = (xpos - this->lastX);
  float yoffset = (this->lastY - ypos);

  this->camera.processCursorEvents(xoffset, yoffset);
}

// Retrive mouse scroll events
void Engine::scrollCallback(double xoffset, double yoffset) {
  this->camera.processScrollEvents(yoffset);
}

// Retrive keyboard activation events
void Engine::processKeyboardEvents(void) {
  // Press Escape to quit
  if (window.getKeyBehavior(GLFW_KEY_ESCAPE, GLFW_PRESS))
    window.close();

  // Process camera movement
  if (window.getKeyBehavior(GLFW_KEY_W, GLFW_PRESS))
    this->camera.processKeyboardEvents(CAMERA_MOVEMENT::FORWARD, this->deltaTime);
  if (window.getKeyBehavior(GLFW_KEY_S, GLFW_PRESS))
    this->camera.processKeyboardEvents(CAMERA_MOVEMENT::BACKWARD, this->deltaTime);
  if (window.getKeyBehavior(GLFW_KEY_A, GLFW_PRESS))
    this->camera.processKeyboardEvents(CAMERA_MOVEMENT::LEFT, this->deltaTime);
  if (window.getKeyBehavior(GLFW_KEY_D, GLFW_PRESS))
    this->camera.processKeyboardEvents(CAMERA_MOVEMENT::RIGHT, this->deltaTime);

  // Change camera moviment speed
  if (window.getKeyBehavior(GLFW_KEY_UP, GLFW_PRESS))
    this->camera.movementSpeed = std::min(DEFAULT_SPEED * 10, this->camera.movementSpeed + DEFAULT_SPEED);
  if (window.getKeyBehavior(GLFW_KEY_DOWN, GLFW_PRESS))
    this->camera.movementSpeed = std::max(DEFAULT_SPEED, this->camera.movementSpeed - DEFAULT_SPEED);
}
