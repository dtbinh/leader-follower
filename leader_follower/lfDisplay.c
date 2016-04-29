/*
 * lfDisplay.c
 *
 *  Created on: Apr 21, 2016
 *      Author: user
 */

#include "inc/hw_types.h"
#include "drivers/display96x16x1.h"
#include "utils/ustdlib.h"

#include "lfUtility.h"

#include "lfDisplay.h"

// Output string formats
#define FOLLOW_STR "FOLLOW"
#define SEARCH_STR "SEARCH"
#define WANDER_STR "WANDER"
#define DIST_STR   "DIST"

void lfUpdateSensorDataDisplay(const RobotState state,
                     const int distanceL,
                     const int distanceR)
{
   const char * const stateStr = (state == FOLLOW) ? FOLLOW_STR : SEARCH_STR;
   Display96x16x1StringDrawCentered(stateStr, 0, true);

   // don't display distance if less than 0
   if ((distanceL >= 0) && (distanceR >=0))
   {
      char distStr[32] = { 0 };
      usnprintf(distStr, sizeof(distStr), "%s: %04d %04d", DIST_STR, distanceL, distanceR);
      Display96x16x1StringDrawCentered(distStr, 1, true);
   }
   else
   {
      Display96x16x1ClearLine(1);
   }
}

void lfUpdateDisplay(const RobotState state, const char* msg)
{
   Display96x16x1ClearLine(1);

   char distStr[32] = { 0 };
   usnprintf(distStr, sizeof(distStr), "%s", msg);
   Display96x16x1StringDrawCentered(distStr, 1, true);
}

void lfDisplayInit()
{
   // Initialize OLED Display
   Display96x16x1Init(true);
}
