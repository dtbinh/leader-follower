/*
 * lsUtility.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LSUTILITY_H_
#define LSUTILITY_H_

// Returns the current system time, in milliseconds.
unsigned long getSysTicks();

// Busy wait for a time.
// This does not handle overflow.
void sleep(const unsigned int millis);

// Initialize the utility functions.
void lsUtilInit();

#endif /* LSUTILITY_H_ */
