#ifndef __CAMERA__
#define __CAMERA__
#include "glm/glm.hpp"
#include "Gamma.h"
#include "Joystick.h"

namespace Gamma
{
   class BaseCamera
   {
   public:
      glm::vec3 pos;
      glm::vec3 front;
      glm::vec3 up;
      float pitch = 0.0f;
      float yaw = -90.0f;
      float *delta_time;
      float cam_speed;

      BaseCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float cam_speed) : pos(pos), front(front), up(up), cam_speed(cam_speed){};

      void update_direction(float yaw_change, float pitch_change, float roll_change);
      virtual void update_position(float x_magnitude, float z_magnitude);
      float clamp_pitch(float pitch);
      static void update_from_joystick(BaseCamera &cam, Joystick &joystick);
   };

   class FlyCam : public BaseCamera
   {
   public:
      FlyCam(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float cam_speed) : BaseCamera(pos, front, up, cam_speed) {}
      void update_position(float x_magnitude, float z_magnitude) override;
   };

   class FPSCam : public BaseCamera
   {
   public:
      FPSCam(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float cam_speed) : BaseCamera(pos, front, up, cam_speed) {}
      void update_position(float x_magnitude, float z_magnitude) override;
   };
};

#endif