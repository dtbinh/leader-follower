/*
 * lfSensors.h
 *
 *  Created on: Apr 25, 2016
 *      Author: user
 */

#ifndef LFSENSORS_H_
#define LFSENSORS_H_

// Available sensors
typedef enum IRSensors
{
   IR_LEFT,
   IR_RIGHT
} IRSensors;

// Query a sensor for a new reading, returning 0-1023
void lfSensorsGetReading(const IRSensors sensor,
                         unsigned long * const sVal);

// Initialize the sensor functions.
void lfSensorsInit();

#endif /* LFSENSORS_H_ */
