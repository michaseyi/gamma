#include "Camera.h"

namespace Gamma
{
   /**
    * @brief Adjusts the direction of the camera
    *
    * @param yaw_change measurement of the angle of rotation along the y axis
    * @param pitch_change  measurement of the angle of rotation along the x axis
    * @param roll_change
    */
   void BaseCamera::update_direction(float yaw_change, float pitch_change, float roll_change)
   {
      auto new_direction = glm::vec3(0.0f);

      yaw += yaw_change;
      pitch = clamp_pitch(pitch - pitch_change);

      new_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      new_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      new_direction.y = sin(glm::radians(pitch));
      front = glm::normalize(new_direction);
   }

   float BaseCamera::clamp_pitch(float pitch)
   {
      if (pitch > 80.0f)
         return 80.0f;
      else if (pitch < -80.0f)
         return -80.0f;
      return pitch;
   }

   void BaseCamera::update_position(float x_magnitude, float z_magnitude) {}

   void FPSCam::update_position(float x_magnitude, float z_magnitude)
   {
      glm::vec3 trans(front);
      trans.y = 0.0f;
      pos -= trans * *delta_time * cam_speed * z_magnitude;
      trans = glm::normalize(glm::cross(front, up));
      trans.y = 0.0f;
      pos += trans * *delta_time * cam_speed * x_magnitude;
   }
   void FlyCam::update_position(float x_magnitude, float z_magnitude)
   {
      glm::vec3 trans(front);
      pos -= trans * *delta_time * cam_speed * z_magnitude;
      trans = glm::normalize(glm::cross(front, up));
      pos += trans * *delta_time * cam_speed * x_magnitude;
   }

   void BaseCamera::update_from_joystick(BaseCamera &cam, Joystick &joystick)
   {
      auto hat_info = joystick.poll();
      cam.update_position(round_float(hat_info[0]), round_float(hat_info[1]));
      float degree = 8.0f;
      float xoffset = round_float(hat_info[3]) * degree;
      float yoffset = round_float(hat_info[2]) * degree;
      float sensitivity = 0.3f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;
      cam.update_direction(xoffset, yoffset, 0.0f);
   }
}