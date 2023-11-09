#pragma once
#include "Gamma.h"

namespace Gamma
{

   class Curve
   {
   private:
      glm::mat3x4 transform;

   public:
      static glm::mat4 Benzie_Weigths;
      Curve(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
      glm::vec3 step(float time_step);
      glm::vec3 first_derivative(float time_step);
   };

   class Spline
   {
   private:
      std::vector<Curve> curves;

   public:
      Spline *add(Curve curve);
      glm::vec3 first_derivative(float time_step);
      glm::vec3 step(float time);
      int max();
   };
}