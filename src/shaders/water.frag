#version 330 core


struct Material {
   sampler2D diffuse;
   vec3 specular;
   float shininess;
};


struct Light {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   vec3 position;

   float constant;
   float linear;
   float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform vec3 light_pos;
uniform vec3 camera_pos;
uniform vec3 color;

uniform Material material;

uniform Light light;



void calculate_light() {

}




void main() {
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


   vec3 lightDir = normalize(light_pos- FragPos);
   vec3 viewDir = normalize(camera_pos - FragPos);

   // ambience
   // vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).xyz;
   vec3 ambient = light.ambient * color; 
   // diffuse
   float diff =  max(dot(lightDir, Normal), 0.0f);
   //vec3 diffuse = diff * texture(material.diffuse, TexCoord).xyz * light.diffuse;
   vec3 diffuse = diff * color* light.diffuse;

   // specular
   float spec = pow(max(dot(viewDir, reflect(-lightDir, Normal)), 0.0f), material.shininess);
   vec3 specular = material.specular*spec*light.specular;

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;
   vec3 result = ambient+ diffuse+specular;

   FragColor = vec4(result, 1.0);
   // gl_FragColor= vec4(vec3(gl_FragCoord.z), 1.0);
}