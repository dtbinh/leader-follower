//****************************************************************************
//
// main.c - leader_follower
//
//****************************************************************************



#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"


static void runStateMachine(void)
{
   // todo
   while(1)
   {
      int i = 0;
      for(; i < 5000; ++i)
      {

      }
   }
}

/**
 * Main function - leader_follower
 */
int main(void)
{
   // Set the system clock to run at 50MHz from the PLL
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   runStateMachine();
}
