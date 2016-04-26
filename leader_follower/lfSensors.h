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

// Measurement bins (cm)
// Estimated distance to target, binned for precision
typedef enum IrDistance
{
   DIST_MIN = 0,
   DIST_10  = 10,
   DIST_15  = 15,
   DIST_20  = 20,
   DIST_25  = 25,
   DIST_30  = 30,
   DIST_35  = 35,
   DIST_40  = 40,
   DIST_45  = 45,
   DIST_50  = 50,
   DIST_55  = 55,
   DIST_60  = 60,
   DIST_70  = 70,
   DIST_80  = 80,
   DIST_90  = 90,
   DIST_MAX = 100,
} IrDistance;

// Measurement bins (cm)
// The enum value corresponds to the upper limit
typedef enum IrDistanceBinMax
{
   DIST_BIN_MAX   = 70,
   DIST_BIN_90    = 93,
   DIST_BIN_80    = 120,
   DIST_BIN_70    = 140,
   DIST_BIN_60    = 153,
   DIST_BIN_55    = 166,
   DIST_BIN_50    = 189,
   DIST_BIN_45    = 210,
   DIST_BIN_40    = 240,
   DIST_BIN_35    = 275,
   DIST_BIN_30    = 310,
   DIST_BIN_25    = 375,
   DIST_BIN_20    = 483,
   DIST_BIN_15    = 655,
   DIST_BIN_10    = 780,
   DIST_BIN_MIN,
} IrDistanceBinMax;


// Query a sensor for a new reading corresponding to the estimated distance.
// The return values range from 0-1023
void lfSensorsGetReading(const IRSensors sensor,
                         unsigned long * const sVal);

// Maps the sensor reading to a distance.
void lfSensorsMapDistance(const unsigned long sVal,
                          IrDistance * const distance);

// Initialize the sensor functions.
void lfSensorsInit();

#endif /* LFSENSORS_H_ */
