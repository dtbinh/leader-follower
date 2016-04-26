/*
 * lfDisplay.h
 *
 *  Created on: Apr 21, 2016
 *      Author: user
 */

#ifndef LFDISPLAY_H_
#define LFDISPLAY_H_

#include "lfUtility.h"

/// Display the state and the estimated distance to the leader (left and right sensors).
/// If distance is less than 0 it is not displayed.
void lfUpdateDisplay(const FollowerState state,
                     const int distanceL,
                     const int distanceR);

// Initialize the display functions.
void lfDisplayInit();

#endif /* LFDISPLAY_H_ */
