#version 330 core


in vec4 Color;
out vec4 FragColor;

void main() {
   FragColor = vec4(0.5, 0.5, 0.5, 1.0);

   vec3 lightColor= vec3(1.0, 1.0, 1.0);
   vec3 toyColor = vec3(1.0, 0.5, 0.31);
   vec3 result = lightColor * toyColor;
   FragColor = vec4(result, 1.0);
   FragColor = vec4(Color.xyz, 1.0);
}