#version 330
out vec4 out_Color;

uniform vec3 light_Color;
uniform vec3 solid_Color;

void main(void) {
  float ambient_Strength = 0.2f;
  vec3 ambient_Light = ambient_Strength * light_Color;

  out_Color = vec4(ambient_Light * solid_Color, 1.0f);
}

