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

#include "lsMotors.h"
#include "lsUtility.h"

static void runStateMachine(void)
{
   while(1)
   {
      turn90(true);
      sleep(5000);
      turn90(false);
      sleep(5000);
   }
}

// Perform module initialization
static void initialize(void)
{
   // Set the system clock to run at 50MHz from the PLL
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   lsUtilInit();
   lsMotorsInit();

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
