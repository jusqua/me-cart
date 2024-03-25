#include <engine.hpp>
#include <glm/common.hpp>

// Engine constructor
Engine::Engine(pgm_t terrain) {
  this->terrain = terrain;

  this->window.setFramebufferSizeCallback(Engine::framebufferSizeCallback);
  this->window.setCursorPosCallback(Engine::cursorPosCallback);
  this->window.setScrollCallback(Engine::scrollCallback);
}

// Engine main loop
void Engine::init(void) {
  auto objectVBO = VBO(BLOCK_VERTICES, BLOCK_VERTICES_SIZE);

  auto terrainVAO = VAO();
  terrainVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  terrainVAO.linkAttrib(objectVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  material_t terrainMaterial = {glm::vec3(1.0f, 0.5f, 0.31f),
                                glm::vec3(1.0f, 0.5f, 0.31f),
                                glm::vec3(0.5f, 0.5f, 0.5f),
                                32.0f};

  auto lightVAO = VAO();
  lightVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  directional_light_t light = {glm::vec3(0.5f),
                               glm::vec3(0.8f),
                               glm::vec3(1.0f),
                               glm::vec3(-0.2f, -1.0f, -0.3f)};

  auto cartVAO = VAO();
  cartVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), 0);
  cartVAO.linkAttrib(objectVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  auto cartScaleFactor = 0.25f;
  // Green plastic material
  material_t cartBodyworkMaterial = {glm::vec3(0.0f),
                                     glm::vec3(0.1f, 0.35f, 0.1f),
                                     glm::vec3(0.45f, 0.55f, 0.45f),
                                     0.25f};
  // Black plastic material
  material_t cartWheelMaterial = {glm::vec3(0.0f),
                                  glm::vec3(0.01f),
                                  glm::vec3(0.5f),
                                  0.25f};

  float cartWheelYaw = 0.0f;
  float cartWheelPitch = 0.0f;
  float cartYaw = 0.0f;
  glm::vec3 cartFront(1.0f, 0.0f, 0.0f);
  glm::vec3 cartUp(0.0f, 1.0f, 0.0f);
  glm::vec3 cartPosition(0.0f, terrain.content[terrain.height / 2][terrain.width / 2], 0.0f);
  glm::vec3 cameraOffset(-6.0f, 4.0f, 0.0f);
  auto movementSpeed = 15.0f;
  auto radialSpeed = 5.0f;
  auto maxWheelTurn = 30.0f;
  camera.yaw = cartYaw;
  camera.pitch = -15.0f;
  camera.updateVectors();

  Shader terrainProgram("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");
  Shader lightProgram("resources/shaders/light.vert", "resources/shaders/light.frag");
  Shader cartProgram("resources/shaders/cart.vert", "resources/shaders/cart.frag");

  while (!window.shouldClose()) {
    auto currentTime = glfwGetTime();
    this->deltaTime = currentTime - this->lastTime;
    this->lastTime = currentTime;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.position = cartPosition + cameraOffset;
    auto view = this->camera.getView();
    auto projection = glm::perspective(glm::radians(camera.fov),
                                       (float)this->window.width / this->window.height,
                                       0.1f,
                                       80.0f);

    // Terrain Logic
    terrainProgram.activate();
    terrainVAO.bind();

    terrainProgram.setUniform("camera_Position", camera.position);

    terrainProgram.setUniform("light.ambient", light.ambient);
    terrainProgram.setUniform("light.diffuse", light.diffuse);
    terrainProgram.setUniform("light.specular", light.specular);
    terrainProgram.setUniform("light.direction", light.direction);

    terrainProgram.setUniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    terrainProgram.setUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    terrainProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    terrainProgram.setUniform("material.shininess", 32.0f);

    terrainProgram.setUniform("view", view);
    terrainProgram.setUniform("projection", projection);

    auto start = glm::translate(glm::mat4(1.0f), -glm::vec3(this->terrain.height / 2.0f, 0.0f, this->terrain.width / 2.0f));
    for (int i = 0; i < this->terrain.height; i++) {
      auto right = glm::translate(start, glm::vec3((float)i, 0.0f, 0.0f));

      for (int j = 0; j < this->terrain.width; j++) {
        int minHeight = terrain.max;
        if (i + 1 < terrain.height)
          minHeight = std::min(minHeight, terrain.content[i + 1][j]);
        if (i - 1 >= 0)
          minHeight = std::min(minHeight, terrain.content[i - 1][j]);
        if (j + 1 < terrain.width)
          minHeight = std::min(minHeight, terrain.content[i][j + 1]);
        if (j - 1 >= 0)
          minHeight = std::min(minHeight, terrain.content[i][j - 1]);

        auto delta = terrain.content[i][j] - minHeight;
        auto model = glm::translate(right, glm::vec3(0.0f, (float)this->terrain.content[i][j], (float)j));

        for (int k = delta; k >= 0; k--) {
          terrainProgram.setUniform("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);
          model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        }
      }
    }

    // Light Logic
    lightProgram.activate();
    lightVAO.bind();
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 500.0f, 0.0f));
    model = glm::scale(model, glm::vec3(50.0f));
    lightProgram.setUniform("view", view);
    lightProgram.setUniform("projection", projection);
    lightProgram.setUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cart Logic
    cartProgram.activate();
    cartVAO.bind();

    cartProgram.setUniform("camera_Position", camera.position);

    cartProgram.setUniform("light.ambient", light.ambient);
    cartProgram.setUniform("light.diffuse", light.diffuse);
    cartProgram.setUniform("light.specular", light.specular);
    cartProgram.setUniform("light.direction", light.direction);

    cartProgram.setUniform("view", view);
    cartProgram.setUniform("projection", projection);

    // Cart bodywork
    cartProgram.setUniform("material.ambient", cartBodyworkMaterial.ambient);
    cartProgram.setUniform("material.diffuse", cartBodyworkMaterial.diffuse);
    cartProgram.setUniform("material.specular", cartBodyworkMaterial.specular);
    cartProgram.setUniform("material.shininess", cartBodyworkMaterial.shininess);

    model = glm::translate(glm::mat4(1.0f), cartPosition);
    model = glm::rotate(model, -glm::radians(cartYaw), cartUp);
    model = glm::translate(model, cartScaleFactor * glm::vec3(-5.0f, 4.0f, 0.0f));
    model = glm::scale(model, cartScaleFactor * glm::vec3(-10.0f, 2.0f, 8.0f));
    cartProgram.setUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::translate(glm::mat4(1.0f), cartPosition);
    model = glm::rotate(model, -glm::radians(cartYaw), cartUp);
    model = glm::translate(model, cartScaleFactor * glm::vec3(0.0f, 6.0f, 0.0f));
    model = glm::scale(model, cartScaleFactor * glm::vec3(-6.0f, 6.0f, 8.0f));
    cartProgram.setUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cart wheels
    cartProgram.setUniform("material.ambient", cartWheelMaterial.ambient);
    cartProgram.setUniform("material.diffuse", cartWheelMaterial.diffuse);
    cartProgram.setUniform("material.specular", cartWheelMaterial.specular);
    cartProgram.setUniform("material.shininess", cartWheelMaterial.shininess);

    model = glm::translate(glm::mat4(1.0f), cartPosition);
    model = glm::rotate(model, -glm::radians(cartYaw), cartUp);
    model = glm::translate(model, cartScaleFactor * glm::vec3(-4.0f, 3.0f, 0.0f));
    for (int i = -1; i < 2; i += 2) {
      auto modelX = glm::translate(model, cartScaleFactor * glm::vec3(i * -4.5f, 0.0f, 0.0f));
      for (int k = -1; k < 2; k += 2) {
        auto modelXZ = glm::translate(modelX, cartScaleFactor * glm::vec3(0.0f, 0.0f, k * 4.5f));
        if (i < 0)
          modelXZ = glm::rotate(modelXZ, -glm::degrees(cartWheelYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        modelXZ = glm::rotate(modelXZ, -glm::degrees(cartWheelPitch), glm::vec3(0.0f, 0.0f, 1.0f));
        auto _model = glm::scale(modelXZ, cartScaleFactor * glm::vec3(-2.0f, 2.0f, 0.5f));
        cartProgram.setUniform("model", _model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    window.swap();
    this->processKeyboardEvents();

    // Cart keyboard controls
    auto velocity = movementSpeed * deltaTime;

    auto WPressed = window.getKeyBehavior(GLFW_KEY_W, GLFW_PRESS);
    auto SPressed = window.getKeyBehavior(GLFW_KEY_S, GLFW_PRESS);
    auto APressed = window.getKeyBehavior(GLFW_KEY_A, GLFW_PRESS);
    auto DPressed = window.getKeyBehavior(GLFW_KEY_D, GLFW_PRESS);

    cartWheelYaw = (APressed * -maxWheelTurn) + (DPressed * maxWheelTurn);
    auto cartDirection = WPressed * 1.0f + SPressed * -1.0f;

    cartYaw += cartDirection * radialSpeed * cartWheelYaw / maxWheelTurn;

    if (cartYaw > 360.0)
      cartYaw -= 360.0;
    if (cartYaw < 0.0)
      cartYaw += 360.0;

    cartFront = glm::normalize(glm::vec3(cos(glm::radians(cartYaw)), 0.0f, sin(glm::radians(cartYaw))));

    auto cartNextPosition = cartPosition + cartDirection * cartFront * velocity;
    auto cartTerrainX = (int)cartNextPosition.x + terrain.height / 2;
    auto cartTerrainZ = (int)cartNextPosition.z + terrain.width / 2;
    if (cartTerrainX >= 0 && cartTerrainX < terrain.height && cartTerrainZ >= 0 && cartTerrainZ < terrain.width) {
      cartPosition = cartNextPosition;
      cartPosition.y = terrain.content[cartTerrainX][cartTerrainZ];
      cartWheelPitch += cartDirection;
    }

    model = glm::rotate(glm::mat4(1.0f), -glm::radians(cartYaw), cartUp);
    model = glm::translate(model, cartPosition);
    cameraOffset = glm::vec3(model * glm::vec4(-6.0f, 4.0f, 0.0f, 0.0f));
    camera.yaw = cartYaw;
    camera.updateVectors();
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

  // this->camera.processCursorEvents(xoffset, yoffset);
}

// Retrive mouse scroll events
void Engine::scrollCallback(double xoffset, double yoffset) {
  // this->camera.processScrollEvents(yoffset);
}

// Retrive keyboard activation events
void Engine::processKeyboardEvents(void) {
  // Press Escape to quit
  if (window.getKeyBehavior(GLFW_KEY_ESCAPE, GLFW_PRESS))
    window.close();

  // Process camera movement
  // if (window.getKeyBehavior(GLFW_KEY_W, GLFW_PRESS))
  //   this->camera.processKeyboardEvents(CAMERA_MOVEMENT::FORWARD, this->deltaTime);
  // if (window.getKeyBehavior(GLFW_KEY_S, GLFW_PRESS))
  //   this->camera.processKeyboardEvents(CAMERA_MOVEMENT::BACKWARD, this->deltaTime);
  // if (window.getKeyBehavior(GLFW_KEY_A, GLFW_PRESS))
  //   this->camera.processKeyboardEvents(CAMERA_MOVEMENT::LEFT, this->deltaTime);
  // if (window.getKeyBehavior(GLFW_KEY_D, GLFW_PRESS))
  //   this->camera.processKeyboardEvents(CAMERA_MOVEMENT::RIGHT, this->deltaTime);

  // Change camera moviment speed
  // if (window.getKeyBehavior(GLFW_KEY_UP, GLFW_PRESS))
  //   this->camera.movementSpeed = std::min(DEFAULT_SPEED * 10, this->camera.movementSpeed + DEFAULT_SPEED);
  // if (window.getKeyBehavior(GLFW_KEY_DOWN, GLFW_PRESS))
  //   this->camera.movementSpeed = std::max(DEFAULT_SPEED, this->camera.movementSpeed - DEFAULT_SPEED);
}
