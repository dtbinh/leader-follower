#include "inc/hw_types.h"
#include "lfMotors.h"
#include "lfUtility.h"

#include "lfWanderBehavior.h"

void wander(void)
{
   unsigned int randMovement = randInterval(1,10);

   // If true, then turn a random angle
   if(randMovement > 7)
   {
      // Obtain a random angle to turn
      unsigned int randAngle = randInterval(0, 180);

      // Map the random distance between -90 and 90 degrees
      randAngle -= 90.0;

      turn(randAngle);
   }
   // If true, then move forward a random distance
   else if(randMovement > 2)
   {
      // Obtain a random distance to travel
      unsigned int randDistance = randInterval(1, 10);
      moveForward(randDistance, false);
   }
   // Backup a random distance
   else
   {
      // Obtain a random distance to travel
      unsigned int randDistance = randInterval(1, 10);
      moveBackward(randDistance, false);
   }
}
