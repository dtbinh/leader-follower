/*
 * lfUtility.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LFUTILITY_H_
#define LFUTILITY_H_

// Define NULL
#ifndef NULL
#define NULL                    ((void *)0)
#endif

// FSM states of the robot
typedef enum RobotState
{
   SEARCH,  // Follower
   FOLLOW,  // Follower
   WANDER   // Leader
} RobotState;

// Returns the current system time, in milliseconds.
unsigned long getSysTicks();

// Busy wait for a time.
// This does not handle overflow.
void sleep(const unsigned int millis);

// Assumes 0 <= min <= max <= RAND_MAX
// Returns in the half-open interval [min, max]
unsigned int randInterval(unsigned int min, unsigned int max);

// Initialize the utility functions.
void lfUtilInit();

#endif /* LFUTILITY_H_ */
