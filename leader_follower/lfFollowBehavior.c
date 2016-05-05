#include "inc/hw_types.h"
#include "utils/ustdlib.h"

#include "lfMotors.h"
#include "lfSensors.h"
#include "lfUtility.h"

#include "lfFollowBehavior.h"

#define MIN_COURSE_CORRECTION     2          // degrees
#define TURN_THRESHOLD            30         // cm
#define DRIVE_THRESHOLD           3          // inches
#define NOMINAL_DIST_CM           DIST_25    // cm
#define MAX_DIST_TRAVEL_INCHES    6          // inches

void follow(DisplayArgs * const args)
{
   IrDistance leftDist = args->distanceL;
   IrDistance rightDist = args->distanceR;

   if(leftDist < 0 || rightDist < 0)
   {
      return;
   }

   // Compute difference between the actual and goal distances in centimeters.
   int leftDistToNominalCm = leftDist - NOMINAL_DIST_CM;
   // abs() value of leftDistToTravel
//   if(leftDistToNominalCm < 0)
//   {
//      leftDistToNominalCm = leftDistToNominalCm * -1.0;
//   }

   int rightDistToNominalCm = rightDist - NOMINAL_DIST_CM;

   // abs() value of leftDistToTravel
//   if(rightDistToNominalCm < 0)
//   {
//      rightDistToNominalCm = rightDistToNominalCm * -1.0;
//   }

   // Choose the minimum of the two distances to travel
   int travelDistCm = MIN(leftDistToNominalCm, rightDistToNominalCm);

   // Convert the travel distance from cm to inches.
   int travelDistInches = intFloor(cmToInches(travelDistCm));

   // Limit the distance that the robot can travel
   if(travelDistInches > MAX_DIST_TRAVEL_INCHES)
   {
      travelDistInches = MAX_DIST_TRAVEL_INCHES;
   }
   else if(travelDistInches < -MAX_DIST_TRAVEL_INCHES)
   {
      travelDistInches = -MAX_DIST_TRAVEL_INCHES;
   }

   // Decide whether a slight course correction is needed
   if(leftDistToNominalCm > rightDistToNominalCm &&
         (leftDistToNominalCm - rightDistToNominalCm) > TURN_THRESHOLD)
   {
      // Turn right
      turn(-MIN_COURSE_CORRECTION);
      return;
   }
   else if(rightDistToNominalCm > leftDistToNominalCm &&
         (rightDistToNominalCm - leftDistToNominalCm) > TURN_THRESHOLD)
   {
      // Turn left
      turn(MIN_COURSE_CORRECTION);
      return;
   }
   else
   {
      // Don't turn
   }

   // Only move forward/backward if travelDist falls within these ranges:
   // -MAX_DIST_TRAVEL_INCHES <= travelDist <= -DIST_THRESHOLD < 0
   // 0 < DIST_THRESHOLD <= travelDist <= MAX_DIST_TRAVEL_INCHES
   //
   // travelDistInches can be positive or negative, but the moveForward()
   // and moveBackward() methods only expect positive numbers.
   if(travelDistInches > DRIVE_THRESHOLD)
   {
      moveForward(travelDistInches, false);
   }
   else if(travelDistInches < -DRIVE_THRESHOLD)
   {
      moveBackward(-travelDistInches, false);
   }
   else
   {
      // Don't move
   }
}
