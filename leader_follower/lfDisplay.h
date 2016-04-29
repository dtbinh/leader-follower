/*
 * lfDisplay.h
 *
 *  Created on: Apr 21, 2016
 *      Author: user
 */

#ifndef LFDISPLAY_H_
#define LFDISPLAY_H_

#include "lfUtility.h"

// Display the state and the estimated distance to the leader (left and right sensors).
// If distance is less than 0 it is not displayed. This function should not be called
// by the follower robot autonomy software.
void lfUpdateSensorDataDisplay(const RobotState state,
                     const int distanceL,
                     const int distanceR);

// Write a message to the display.
void lfUpdateDisplay(const RobotState state,
                     const char* msg);

// Initialize the display functions.
void lfDisplayInit();

#endif /* LFDISPLAY_H_ */
