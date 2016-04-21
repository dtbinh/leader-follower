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
#include "lfUtility.h"

static void runStateMachine(void)
{
   lfUpdateDisplay(FOLLOW, 5);

   while(1)
   {
      /* test behaviors */
      turn(-90);
      sleep(5000);
      turn(90);
      sleep(5000);

      lfUpdateDisplay(SEARCH, -1);

      turn(-45);
      sleep(2000);
      turn(-45);
      sleep(2000);
      turn(45);
      sleep(2000);
      turn(45);
      sleep(2000);

      lfUpdateDisplay(FOLLOW, 5);

      moveForward(2, true);
      moveBackward(2, true);
      sleep(5000);
      moveForward(2, false);
      moveBackward(2, false);
      sleep(5000);
   }
}

// Perform module initialization
static void initialize(void)
{
   // Set the system clock to run at 50MHz from the PLL
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   lfUtilInit();
   lfMotorsInit();
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
