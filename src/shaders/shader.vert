#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 color;

out vec4 outColor;

uniform float time;

void main() {
    mat3 rotate = mat3 (
      cos(time), -sin(time), 0.0,
      sin(time), cos(time), 0.0,
      0.0, 0.0, 1.0
    );

   vec3 n = rotate * vec3(aPos, 1.0);
   gl_Position = vec4(n, 1.0);

  outColor = vec4(n, 1.0);

}
