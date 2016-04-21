/*
 * lfDisplay.c
 *
 *  Created on: Apr 21, 2016
 *      Author: user
 */

#include <utils/ustdlib.h>

#include "inc/hw_types.h"
#include "drivers/display96x16x1.h"
#include "lfUtility.h"

// Output string formats
#define FOLLOW_STR "FOLLOW"
#define SEARCH_STR "SEARCH"
#define DIST_STR   "DIST"

void lfUpdateDisplay(const FollowerState state, const int distance)
{
   const char * const stateStr = (state == FOLLOW) ? FOLLOW_STR : SEARCH_STR;
   Display96x16x1StringDrawCentered(stateStr, 0, true);

   // don't display distance if less than 0
   if (distance >= 0)
   {
      char distStr[32] = { 0 };
      usnprintf(distStr, sizeof(distStr), "%s: %d", DIST_STR, distance);
      Display96x16x1StringDrawCentered(distStr, 1, true);
   }
   else
   {
      Display96x16x1ClearLine(1);
   }
}

void lfDisplayInit()
{
   // Initialize OLED Display
   Display96x16x1Init(true);
}
