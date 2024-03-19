#version 330
out vec4 out_Color;

uniform vec3 light_Color;
uniform vec3 solid_Color;
uniform float ambient_Strength;

void main(void) {
  out_Color = vec4(ambient_Strength * light_Color * solid_Color, 1.0f);
}

