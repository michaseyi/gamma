#ifndef __JOYSTICK__
#define __JOYSTICK__
#include "Gamma.h"
namespace Gamma
{
   class Joystick
   {
   public:
      int id;
      Joystick(int id) : id(id){};
      const float *poll();
   };
}

#endif