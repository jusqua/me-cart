#version 330
in vec3 ex_Position;
in vec3 ex_Normal;

out vec4 out_Color;

struct material_t {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct light_t {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
};

uniform vec3 camera_Position;
uniform material_t material;
uniform light_t light;

void main(void) {
  vec3 ambient = light.ambient * material.ambient;

  vec3 normal = normalize(ex_Normal);
  vec3 light_Direction = normalize(-light.direction);
  float diffuse_Strength = max(dot(light_Direction, normal), 0.0f);
  vec3 diffuse = light.diffuse * (diffuse_Strength * material.diffuse);

  vec3 camera_Direction = normalize(camera_Position - ex_Position);
  vec3 reflect_Direction = reflect(-light_Direction, normal);
  float specular_Gradient = pow(max(dot(camera_Direction, reflect_Direction), 0.0f), material.shininess);
  vec3 specular = light.specular * (specular_Gradient * material.specular);

  out_Color = vec4(ambient + diffuse + specular, 1.0f);
}

