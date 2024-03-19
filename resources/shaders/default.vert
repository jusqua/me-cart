#version 330
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(void) {
  gl_Position = projection * view * model * vec4(in_Position, 1.0f);
}
