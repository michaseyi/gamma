#include "Joystick.h"

namespace Gamma
{
   const float *Joystick::poll()
   {
      int size;
      auto axis_state = glfwGetJoystickAxes(id, &size);
      if (size == 0)
      {
         std::cerr << "Gamma Engine Error: Faild to load joystick axis state. id: " << id << std::endl;
         exit(2);
      }
      return axis_state;
   }
}