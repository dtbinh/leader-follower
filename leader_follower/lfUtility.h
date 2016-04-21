/*
 * lfUtility.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LFUTILITY_H_
#define LFUTILITY_H_

// States of the follower robot
typedef enum FollowerState
{
   SEARCH,
   FOLLOW
} FollowerState;

// Returns the current system time, in milliseconds.
unsigned long getSysTicks();

// Busy wait for a time.
// This does not handle overflow.
void sleep(const unsigned int millis);

// Initialize the utility functions.
void lfUtilInit();

#endif /* LFUTILITY_H_ */
