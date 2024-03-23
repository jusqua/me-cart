#version 330
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;

out vec3 ex_Position;
out vec3 ex_Normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(void) {
  ex_Position = vec3(model * vec4(in_Position, 1.0f));
  ex_Normal = mat3(transpose(inverse(model))) * in_Normal;
  gl_Position = projection * view * vec4(ex_Position, 1.0f);
}
