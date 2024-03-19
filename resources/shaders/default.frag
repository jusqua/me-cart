#version 330
in vec3 ex_Position;
in vec3 ex_Normal;

out vec4 out_Color;

uniform vec3 camera_Position;
uniform vec3 light_Position;
uniform vec3 light_Color;
uniform vec3 object_Color;

void main(void) {
  float ambient_Strength = 0.2f;
  vec3 ambient_Light = ambient_Strength * light_Color;

  vec3 normal = normalize(ex_Normal);
  vec3 light_Direction = normalize(light_Position - ex_Position);
  float diffuse_Strength = max(dot(light_Direction, normal), 0.0f);
  vec3 diffuse_Light = diffuse_Strength * light_Color;

  float specular_Strength = 0.5f;
  vec3 camera_Direction = normalize(camera_Position - ex_Position);
  vec3 reflect_Direction = reflect(-light_Direction, normal);
  float specular_Gradient = pow(max(dot(camera_Direction, reflect_Direction), 0.0f), 32);
  vec3 specular_Light = specular_Strength * specular_Gradient * light_Color;

  out_Color = vec4((ambient_Light + diffuse_Light + specular_Light) * object_Color, 1.0f);
}

