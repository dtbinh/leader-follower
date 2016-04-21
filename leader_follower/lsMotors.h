/*
 * lsMotors.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LSMOTORS_H_
#define LSMOTORS_H_

#include <stdbool.h>

// Moves forwrad a set distance at either 80% or 40% PWM
void moveForward(const unsigned int inches,
                 const bool fast);

// Moves backwards a set distance at either 80% or 40% PWM
void moveBackward(const unsigned int inches,
                  const bool fast);

// Turns between -90 and 90 degrees at 50% PWM
// The input value is bound internally. 0 is a no-op
void turn(int degrees);

// Initialize the motor functions.
void lsMotorsInit();


#endif /* LSMOTORS_H_ */
