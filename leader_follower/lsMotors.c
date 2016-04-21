/*
 * lsMotors.c
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#include "inc/hw_types.h"
#include "drivers/motor.h"
#include "drivers/sensors.h"
#include "lsMotors.h"

// Number of encoder ticks per wheel rotation
#define ENTICKS_PER_REV 8

// Wheel radius (side of wheel)
#define WHEEL_RADIUS 0.4375

// Wheel diameter (between wheels)
#define INTER_WHEEL_DIAMETER 3.21

// PI
#define M_PI     3.14159265358979323846
#define M_PI_2   1.57079632679489661923
#define M_PI_4   0.78539816339744830962

// The current state of a motor
typedef enum MotorState
{
   MS_STOPPED,
   MS_FORWARD,
   MS_REVERSE
} MotorState;

// Number of inches per encoder tick, accounting for error
static const double INCH_PER_TICK = 2 * M_PI * WHEEL_RADIUS / ENTICKS_PER_REV;

// tracks the encoder tick count and distance
static volatile int     gblTickCountLeft  = 0;
static volatile int     gblTickCountRight = 0;
static volatile double  gblDistanceLeft   = 0;
static volatile double  gblDistanceRight  = 0;

// track the current state of the motors
static MotorState leftMotorState    = MS_STOPPED;
static MotorState rightMotorState   = MS_STOPPED;

// Handles ISRs from PORT E (encoders and bumpers)
void PortEHandler(const tWheel sensor)
{
   switch (sensor)
   {
   case WHEEL_LEFT:
      if (leftMotorState == MS_FORWARD)
      {
         gblTickCountLeft++;
         gblDistanceLeft += INCH_PER_TICK;
      }
      else if (leftMotorState == MS_REVERSE)
      {
         gblTickCountLeft--;
         gblDistanceLeft -= INCH_PER_TICK;
      }
      break;
   case WHEEL_RIGHT:
      if (rightMotorState == MS_FORWARD)
      {
         gblTickCountRight++;
         gblDistanceRight += INCH_PER_TICK;
      }
      else if (rightMotorState == MS_REVERSE)
      {
         gblTickCountRight--;
         gblDistanceRight -= INCH_PER_TICK;
      }
      break;
   default:
      return;
   }
}

void turn90(const bool clockwise)
{
   // Number of inches per wheel movement in circle; r = inter_wheel / 2; d = pi * r / 2
   static const double INCH_PER_90  = M_PI_4 * INTER_WHEEL_DIAMETER;

   const tDirection dirL      = clockwise ? FORWARD : REVERSE;
   const tDirection dirR      = clockwise ? REVERSE : FORWARD;
   const MotorState dirStateL = clockwise ? MS_FORWARD : MS_REVERSE;
   const MotorState dirStateR = clockwise ? MS_REVERSE : MS_FORWARD;
   const unsigned short speed = 50 << 8;

   MotorDir(LEFT_SIDE, dirL);
   MotorDir(RIGHT_SIDE, dirR);
   leftMotorState = dirStateL;
   rightMotorState = dirStateR;

   MotorSpeed(LEFT_SIDE, speed);
   MotorSpeed(RIGHT_SIDE, speed);

   MotorRun(LEFT_SIDE);
   MotorRun(RIGHT_SIDE);

   // wait until the action has completed
   if (clockwise)
   {
      const double distanceStop = gblDistanceLeft + INCH_PER_90;
      do
      {
      } while(gblDistanceLeft < distanceStop);
   }
   else
   {
      const double distanceStop  = gblDistanceRight + INCH_PER_90;
      do
      {
      } while(gblDistanceRight < distanceStop);
   }

   MotorStop(LEFT_SIDE);
   MotorStop(RIGHT_SIDE);
   leftMotorState = MS_STOPPED;
   rightMotorState = MS_STOPPED;
}

void lsMotorsInit()
{
   // Initialize Motors
   MotorsInit();

   // Initialize wheel sensors
   WheelSensorsInit(PortEHandler);
   WheelSensorEnable();
   WheelSensorIntEnable(WHEEL_LEFT);
   WheelSensorIntEnable(WHEEL_RIGHT);
}
