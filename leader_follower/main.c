//****************************************************************************
//
// main.c - leader_follower
//
//****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "lfDisplay.h"
#include "lfMotors.h"
#include "lfSensors.h"
#include "lfUtility.h"

#if 0
// Self-test routines
static void selfTest(void)
{
   /* test behaviors */
   turn(-90);
   sleep(5000);
   turn(90);
   sleep(5000);

   lfUpdateDisplay(SEARCH, -1, -1);

   turn(-45);
   sleep(2000);
   turn(-45);
   sleep(2000);
   turn(45);
   sleep(2000);
   turn(45);
   sleep(2000);

   lfUpdateDisplay(FOLLOW, 5, 15);

   moveForward(2, true);
   moveBackward(2, true);
   sleep(5000);
   moveForward(2, false);
   moveBackward(2, false);
   sleep(5000);
}
#endif

// Average the sensor readings over 1 second to characterize them
static void pollAvgSensorVal(void)
{
   // sample each sensor multiple times and average
   const int SAMPLES = 5;
   unsigned long irLeftVal;
   unsigned long irRightVal;
   unsigned long irLeftAvgVal = 0;
   unsigned long irRightAvgVal = 0;

   for (int i = 0; i < SAMPLES; ++i)
   {
      lfSensorsGetReading(IR_LEFT, &irLeftVal);
      lfSensorsGetReading(IR_RIGHT, &irRightVal);

      irLeftAvgVal += irLeftVal;
      irRightAvgVal += irRightVal;

      sleep(200);
   }

   irLeftAvgVal /= SAMPLES;
   irRightAvgVal /= SAMPLES;
   //lfUpdateDisplay(FOLLOW, irLeftAvgVal, irRightAvgVal);

   // output est distance
   IrDistance leftDist;
   IrDistance rightDist;
   lfSensorsMapDistance(irLeftAvgVal, &leftDist);
   lfSensorsMapDistance(irRightAvgVal, &rightDist);
   lfUpdateDisplay(FOLLOW, leftDist, rightDist);
}

// State machine implementing run-time logic for leader or follower
static void runStateMachine(void)
{
   lfUpdateDisplay(FOLLOW, 5, 15);

   while(1)
   {
      pollAvgSensorVal();
   }
}

// Perform module initialization
static void initialize(void)
{
   // Set the system clock to run at 50MHz from the PLL
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   lfUtilInit();
   lfMotorsInit();
   lfSensorsInit();
   lfDisplayInit();

   // Enable interrupts to the CPU
   IntMasterEnable();
}
/**
 * Main function - leader_follower
 */
int main(void)
{
   initialize();

   runStateMachine();

   return 0;
}
