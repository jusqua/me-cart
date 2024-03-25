#include <engine.hpp>
#include <glm/common.hpp>

// Engine constructor
Engine::Engine(const char *terrainPath, const char *terrainTexturePath) {
  this->terrain = importPGM(terrainPath);
  this->terrainTexture = importTexture(terrainTexturePath);

  this->window.setFramebufferSizeCallback(Engine::framebufferSizeCallback);
}

// Engine main loop
void Engine::init(void) {
  auto objectVBO = VBO(BLOCK_VERTICES, BLOCK_VERTICES_SIZE);

  // INFO: Terrain object
  auto terrainVAO = VAO();
  terrainVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  terrainVAO.linkAttrib(objectVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  terrainVAO.linkAttrib(objectVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  material_t terrainMaterial = {glm::vec3(1.0f, 0.5f, 0.31f),
                                glm::vec3(1.0f, 0.5f, 0.31f),
                                glm::vec3(0.5f, 0.5f, 0.5f),
                                32.0f};

  // INFO: Light object
  auto lightVAO = VAO();
  lightVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  directional_light_t light = {glm::vec3(0.5f),
                               glm::vec3(0.8f),
                               glm::vec3(1.0f),
                               glm::vec3(-0.2f, -1.0f, -0.3f)};

  // INFO: Cart object
  auto cartVAO = VAO();
  cartVAO.linkAttrib(objectVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
  cartVAO.linkAttrib(objectVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  auto cartScaleFactor = 0.25f;
  material_t cartBodyworkMaterial = {glm::vec3(0.0f),
                                     glm::vec3(0.1f, 0.35f, 0.1f),
                                     glm::vec3(0.45f, 0.55f, 0.45f),
                                     0.25f};
  material_t cartWheelMaterial = {glm::vec3(0.0f),
                                  glm::vec3(0.01f),
                                  glm::vec3(0.5f),
                                  0.25f};

  // Cart and camera behavior
  float cartWheelYaw = 0.0f;
  float cartWheelPitch = 0.0f;
  float cartYaw = 0.0f;
  glm::vec3 cartFront(1.0f, 0.0f, 0.0f);
  glm::vec3 cartUp(0.0f, 1.0f, 0.0f);
  glm::vec3 cartPosition(0.0f, terrain.content[terrain.height / 2][terrain.width / 2], 0.0f);
  glm::vec3 cameraOffset(-6.0f, 4.0f, 0.0f);
  auto movementSpeed = 15.0f;
  auto wheelTurnSpeed = 1.5f;
  auto maxWheelTurn = 45.0f;
  camera.yaw = cartYaw;
  camera.pitch = -15.0f;
  camera.updateVectors();

  // INFO: Computing shaders
  Shader terrainProgram("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");
  Shader lightProgram("resources/shaders/light.vert", "resources/shaders/light.frag");
  Shader cartProgram("resources/shaders/cart.vert", "resources/shaders/cart.frag");

  while (!window.shouldClose()) {
    // INFO: Time logic
    auto currentTime = glfwGetTime();
    this->deltaTime = currentTime - this->lastTime;
    this->lastTime = currentTime;

    // INFO: Clear background to next frame
    glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // INFO: Camera position and projection into world
    camera.position = cartPosition + cameraOffset;
    auto view = this->camera.getView();
    auto projection = glm::perspective(glm::radians(camera.fov),
                                       (float)this->window.width / this->window.height,
                                       0.1f,
                                       80.0f);

    // INFO: Terrain assembling
    terrainProgram.activate();
    terrainVAO.bind();

    terrainProgram.setUniform("camera_Position", camera.position);

    terrainProgram.setUniform("light.ambient", light.ambient);
    terrainProgram.setUniform("light.diffuse", light.diffuse);
    terrainProgram.setUniform("light.specular", light.specular);
    terrainProgram.setUniform("light.direction", light.direction);

    terrainProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    terrainProgram.setUniform("material.shininess", 32.0f);
    cartProgram.setUniform("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);

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

    // INFO: Light assembling
    lightProgram.activate();
    lightVAO.bind();
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 500.0f, 0.0f));
    model = glm::scale(model, glm::vec3(50.0f));
    lightProgram.setUniform("view", view);
    lightProgram.setUniform("projection", projection);
    lightProgram.setUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // INFO: Cart assembling
    cartProgram.activate();
    cartVAO.bind();

    cartProgram.setUniform("camera_Position", camera.position);

    cartProgram.setUniform("light.ambient", light.ambient);
    cartProgram.setUniform("light.diffuse", light.diffuse);
    cartProgram.setUniform("light.specular", light.specular);
    cartProgram.setUniform("light.direction", light.direction);

    cartProgram.setUniform("view", view);
    cartProgram.setUniform("projection", projection);

    // Cart bodywork assembling
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

    // Cart wheels assembling
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
          modelXZ = glm::rotate(modelXZ, -glm::radians(cartWheelYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        modelXZ = glm::rotate(modelXZ, -glm::radians(cartWheelPitch), glm::vec3(0.0f, 0.0f, 1.0f));
        auto _model = glm::scale(modelXZ, cartScaleFactor * glm::vec3(-2.0f, 2.0f, 0.5f));
        cartProgram.setUniform("model", _model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    // INFO: User control logic
    auto velocity = movementSpeed * deltaTime;

    // Get controls
    auto WPressed = window.getKeyBehavior(GLFW_KEY_W, GLFW_PRESS);
    auto SPressed = window.getKeyBehavior(GLFW_KEY_S, GLFW_PRESS);
    auto APressed = window.getKeyBehavior(GLFW_KEY_A, GLFW_PRESS);
    auto DPressed = window.getKeyBehavior(GLFW_KEY_D, GLFW_PRESS);

    // Change cart wheel yaw rotation axis
    cartWheelYaw = glm::clamp(cartWheelYaw + (APressed * -wheelTurnSpeed) + (DPressed * wheelTurnSpeed), -maxWheelTurn, maxWheelTurn);

    // Based on key which direction the cart will go, forward or backward
    auto cartDirection = WPressed * 1.0f + SPressed * -1.0f;

    // Change cart yaw rotation axis
    cartYaw += cartDirection * cartWheelYaw / maxWheelTurn * 2.0f;
    if (cartYaw > 360.0)
      cartYaw -= 360.0;
    if (cartYaw < 0.0)
      cartYaw += 360.0;

    // Change cart front
    cartFront = glm::normalize(glm::vec3(cos(glm::radians(cartYaw)), 0.0f, sin(glm::radians(cartYaw))));

    // Check if cart next position is possible and handle terrain "physics"
    auto cartNextPosition = cartPosition + cartDirection * cartFront * velocity;
    auto cartTerrainX = (int)cartNextPosition.x + terrain.height / 2;
    auto cartTerrainZ = (int)cartNextPosition.z + terrain.width / 2;
    if ((cartTerrainX >= 0 && cartTerrainX < terrain.height) &&
        (cartTerrainZ >= 0 && cartTerrainZ < terrain.width) &&
        (terrain.content[cartTerrainX][cartTerrainZ] - cartPosition.y < 2)) {
      cartPosition = cartNextPosition;
      cartPosition.y = terrain.content[cartTerrainX][cartTerrainZ];
      cartWheelPitch += cartDirection * 100;
    }

    // Change camera position and rotation to look cart's back
    model = glm::rotate(glm::mat4(1.0f), -glm::radians(cartYaw), cartUp);
    model = glm::translate(model, cartPosition);
    cameraOffset = glm::vec3(model * glm::vec4(-6.0f, 4.0f, 0.0f, 0.0f));
    camera.yaw = cartYaw;
    camera.updateVectors();

    // Handle window close
    if (window.getKeyBehavior(GLFW_KEY_ESCAPE, GLFW_PRESS))
      window.close();

    // Swap buffer and window events
    window.swap();
  }
}

// Retrive framebuffer reshape events
void Engine::framebufferSizeCallback(int width, int height) {
  // Scale screen based on framebuffer dimensions
  this->window.width = width;
  this->window.height = height;
  glViewport(0, 0, width, height);
}
