#include "Spline.h"
#include "exception"

namespace Gamma
{
   glm::vec3 Spline::step(float time)
   {
      uint index = time;
      float time_step = time - static_cast<float>(index);

      if (this->curves.empty())
      {
         throw std::exception();
      }

      index %= this->curves.size();

      return this->curves[index].step(time_step);
   }

   int Spline::max()
   {
      return this->curves.size();
   }

   Spline *Spline::add(Curve curve)
   {
      this->curves.emplace_back(curve);
      return this;
   }

   glm::vec3 Spline::first_derivative(float time)
   {
      uint index = time;
      float time_step = time - static_cast<float>(index);

      if (this->curves.empty())
      {
         throw std::exception();
      }

      index %= this->curves.size();

      return this->curves[index].first_derivative(time_step);
   }

   glm::mat4 Curve::Benzie_Weigths = glm::mat4(
       glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
       glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
       glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
       glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

   Curve::Curve(glm::vec3 control_point1, glm::vec3 control_point2, glm::vec3 control_point3, glm::vec3 control_point4)
   {
      this->transform = Curve::Benzie_Weigths * glm::transpose(glm::mat4x3(
                                                    control_point1,
                                                    control_point2,
                                                    control_point3,
                                                    control_point4));
   }

   /*
   @param time_step: should range from [0, 1]
   */
   glm::vec3 Curve::step(float time_step)
   {
      return glm::vec4(
                 time_step * time_step * time_step,
                 time_step * time_step,
                 time_step,
                 1) *
             this->transform;
   }

   glm::vec3 Curve::first_derivative(float time_step)
   {
      return glm::vec4(
                 3 * (time_step * time_step),
                 2 * time_step,
                 1,
                 0) *
             this->transform;
   }
}