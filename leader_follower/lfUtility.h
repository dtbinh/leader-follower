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

// Define min/max operations
#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))

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

// Converts inches to centimeters
float inchesToCm(float inches);

// Converts centimeters to inches
float cmToInches(float cm);

// Performs a floor() operation on the given double.
// This function supports positive and negative numbers
// between +/-100,000
int intFloor(double x);

// Initialize the utility functions.
void lfUtilInit();

#endif /* LFUTILITY_H_ */
