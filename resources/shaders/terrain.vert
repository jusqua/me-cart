#version 330
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_Texture;

out vec3 ex_Position;
out vec3 ex_Normal;
out vec2 ex_Texture;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(void) {
  ex_Position = vec3(model * vec4(in_Position, 1.0f));
  ex_Normal = mat3(transpose(inverse(model))) * in_Normal;
  ex_Texture = in_Texture;
  gl_Position = projection * view * vec4(ex_Position, 1.0f);
}
