#include "Gamma.h"

std::ostream &operator<<(std::ostream &output, glm::vec4 &vec)
{
   output << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]" << std::endl;
   return output;
}

int main()
{
   // input layer;
   glm::vec4 input{192, 0, 255, 172};
   // normalize input layer;
   input /= 255;

   // layer 2;
   // neuron 1;
   glm::vec4 weight_1{2, 0.3,9, 1.2};
   glm::vec4 weight_2{2, 0.3,9, 1.2};

   auto out_1 = glm::sin(glm::dot(weight_1, input));
   auto out_2 = glm::sin(glm::dot(weight_2, input));

   // output layer
   glm::vec2 weight_3{2, 0.8};
   auto result = glm::cos(glm::dot(weight_3, glm::vec2(out_1, out_2)));
   std::cout << result;

   std::cout << input;
   return 0;
}