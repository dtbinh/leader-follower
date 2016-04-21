/*
 * lsMotors.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LSMOTORS_H_
#define LSMOTORS_H_

#include <stdbool.h>

// Turns 90 degrees at 50% PWM
void turn90(const bool clockwise);


// Initialize the motor functions.
void lsMotorsInit();


#endif /* LSMOTORS_H_ */
