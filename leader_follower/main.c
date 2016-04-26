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

// State machine implementing run-time logic for leader or follower
static void runStateMachine(void)
{
   lfUpdateDisplay(FOLLOW, 5, 15);

   while(1)
   {
      unsigned long irLeftVal;
      unsigned long irRightVal;
      lfSensorsGetReading(IR_LEFT, &irLeftVal);
      lfSensorsGetReading(IR_RIGHT, &irRightVal);
      lfUpdateDisplay(FOLLOW, irLeftVal, irRightVal);
      sleep(500);
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
