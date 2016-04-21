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

// Moves straight, either forwards or backwards, a set distance at either 80% or 40% PWM
static void moveStraight(const bool forward,
                         const unsigned int inches,
                         const bool fast)
{
   const tDirection dir = forward ? FORWARD : REVERSE;
   const MotorState ms  = forward ? MS_FORWARD : MS_REVERSE;
   const unsigned short duty = fast ? 80 << 8 : 40 << 8;

   MotorDir(LEFT_SIDE, dir);
   MotorDir(RIGHT_SIDE, dir);
   leftMotorState = ms;
   rightMotorState = ms;

   MotorSpeed(LEFT_SIDE, duty);
   MotorSpeed(RIGHT_SIDE, duty);

   MotorRun(LEFT_SIDE);
   MotorRun(RIGHT_SIDE);

   // wait until the action has completed
   if (forward)
   {
      const double distanceStop = gblDistanceLeft + inches;
      do
      {
      } while(gblDistanceLeft < distanceStop);
   }
   else
   {
      const double distanceStop = gblDistanceLeft - inches;
      do
      {
      } while(gblDistanceLeft > distanceStop);
   }

   MotorStop(LEFT_SIDE);
   MotorStop(RIGHT_SIDE);
   leftMotorState = MS_STOPPED;
   rightMotorState = MS_STOPPED;
}

void moveForward(const unsigned int inches,
                 const bool fast)
{
   moveStraight(true, inches, fast);
}

void moveBackward(const unsigned int inches,
                  const bool fast)
{
   moveStraight(false, inches, fast);
}

void turn(int degrees)
{
   // Number of inches per wheel movement in circle; r = inter_wheel / 2; d = pi * r / 2
   static const double INCH_PER_90  = M_PI_4 * INTER_WHEEL_DIAMETER;
   const int LIMIT = 90;

   if (degrees == 0)
   {
      return;
   }

   // bound degrees
   degrees = (degrees > LIMIT) ? LIMIT : degrees;
   degrees = (degrees < -LIMIT) ? -LIMIT : degrees;

   bool clockwise = false;
   // determine if clockwise, and get abs() of degrees for distance calculation
   if (degrees < 0)
   {
      clockwise = true;
      degrees = degrees * -1;
   }

   const double turnDistance = INCH_PER_90 * ((double)degrees / (double)LIMIT);

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
      const double distanceStop = gblDistanceLeft + turnDistance;
      do
      {
      } while(gblDistanceLeft < distanceStop);
   }
   else
   {
      const double distanceStop  = gblDistanceRight + turnDistance;
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
