#include <camera.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->movementSpeed = DEFAULT_SPEED;
  this->cursorSensitivity = DEFAULT_SENSITIVITY;
  this->fov = DEFAULT_FOV;
  updateVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
  this->position = glm::vec3(posX, posY, posZ);
  this->worldUp = glm::vec3(upX, upY, upZ);
  this->yaw = yaw;
  this->pitch = pitch;
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->movementSpeed = DEFAULT_SPEED;
  this->cursorSensitivity = DEFAULT_SENSITIVITY;
  this->fov = DEFAULT_FOV;
  updateVectors();
}

glm::mat4 Camera::getView(void) {
  return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardEvents(CAMERA_MOVEMENT type, float deltaTime) {
  const float velocity = movementSpeed * deltaTime;
  switch (type) {
    case FORWARD:
      position += front * velocity;
      break;
    case BACKWARD:
      position -= front * velocity;
      break;
    case LEFT:
      position -= right * velocity;
      break;
    case RIGHT:
      position += right * velocity;
      break;
  }
}

void Camera::processCursorEvents(float xoffset, float yoffset, bool constrainPitch) {
  xoffset *= cursorSensitivity;
  yoffset *= cursorSensitivity;

  yaw = xoffset;
  pitch = yoffset;

  if (constrainPitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  updateVectors();
}

void Camera::processScrollEvents(float yoffset) {
  fov -= yoffset;
  if (fov > MAX_FOV)
    fov = MAX_FOV;
  if (fov < MIN_FOV)
    fov = MIN_FOV;
}

void Camera::updateVectors(void) {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
