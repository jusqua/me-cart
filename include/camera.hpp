#ifndef MECART_CAMERA_HPP
#define MECART_CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const glm::vec3 DEFAULT_CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = -90.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_FOV = 70.0f;
const float DEFAULT_SENSITIVITY = 0.1f;

const float MIN_FOV = 10.0f;
const float MAX_FOV = 115.0f;

class Camera {
 public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float movementSpeed;
  float cursorSensitivity;
  float fov;

  Camera(
      glm::vec3 position = DEFAULT_CAMERA_POSITION,
      glm::vec3 up = DEFAULT_CAMERA_UP,
      float yaw = DEFAULT_YAW,
      float pitch = DEFAULT_PITCH);

  glm::mat4 getView(void);
  void processKeyboardEvents(CAMERA_MOVEMENT type, float deltaTime);
  void processCursorEvents(float xoffset, float yoffset, bool constrainPitch = true);
  void processScrollEvents(float yoffset);
  void updateVectors(void);
};

#endif  // !MECART_CAMERA_HPP
