#include "inc/hw_types.h"
#include "lfMotors.h"
#include "lfSensors.h"
#include "lfUtility.h"

#include "lfSearchBehavior.h"

void search(DisplayArgs * const args)
{
   IrDistance leftDist = args->distanceL;
   IrDistance rightDist = args->distanceR;

   if(leftDist < 0 || rightDist < 0)
   {
      return;
   }

   turn(15);
}
