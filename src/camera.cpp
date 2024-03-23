#include <camera.hpp>

// Camera constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->movementSpeed = DEFAULT_SPEED;
  this->cursorSensitivity = DEFAULT_SENSITIVITY;
  this->fov = DEFAULT_FOV;
  this->updateVectors();
}

// Return camera lookAt matrix
glm::mat4 Camera::getView(void) {
  return glm::lookAt(this->position, this->position + this->front, this->up);
}

// Process events from keyboard
void Camera::processKeyboardEvents(CAMERA_MOVEMENT type, float deltaTime) {
  const float velocity = this->movementSpeed * deltaTime;
  switch (type) {
    case FORWARD:
      this->position += this->front * velocity;
      break;
    case BACKWARD:
      this->position -= this->front * velocity;
      break;
    case LEFT:
      this->position -= this->right * velocity;
      break;
    case RIGHT:
      this->position += this->right * velocity;
      break;
  }
}

// Process events from mouse cursor
void Camera::processCursorEvents(float xoffset, float yoffset, bool constrainPitch) {
  xoffset *= this->cursorSensitivity;
  yoffset *= this->cursorSensitivity;

  this->yaw = xoffset;
  this->pitch = yoffset;

  if (constrainPitch) {
    if (this->pitch > 89.0f)
      this->pitch = 89.0f;
    if (this->pitch < -89.0f)
      this->pitch = -89.0f;
  }

  this->updateVectors();
}

// Process events from mouse scroll
void Camera::processScrollEvents(float yoffset) {
  this->fov -= yoffset;
  if (this->fov > MAX_FOV)
    this->fov = MAX_FOV;
  if (this->fov < MIN_FOV)
    this->fov = MIN_FOV;
}

// Update camera main vectors
void Camera::updateVectors(void) {
  glm::vec3 front;
  front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  front.y = sin(glm::radians(this->pitch));
  front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->front = glm::normalize(front);
  this->right = glm::normalize(glm::cross(this->front, this->worldUp));
  this->up = glm::normalize(glm::cross(this->right, this->front));
}
