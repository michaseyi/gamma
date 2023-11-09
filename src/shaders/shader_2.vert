#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec4 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main() {
   vec4 model_space  = model * vec4(aPos, 1.0f);
   gl_Position = projection * view * model_space; 
   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
   Color = vec4(color, 1.0f);
   FragPos = model_space.xyz;
   TexCoord = texCoord;
}