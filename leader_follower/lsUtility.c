/*
 * lsUtility.c
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "lsUtility.h"

// The number of SysTick ticks per second used for the SysTick interrupt.
#define SYSTICKS_PER_SECOND     1000

// The number of milliseconds elapsed since the start of the program.
static volatile unsigned long gblMillis = 0;

// SysTick timer ISR
void SysTickHandler(void)
{
   gblMillis++;
}

unsigned long getSysTicks()
{
   return gblMillis;
}

void sleep(const unsigned int millis)
{
   const unsigned long stop = gblMillis + millis;
   while (gblMillis < stop);
}

void lsUtilInit()
{
   // Configure SysTick to occur n times per second, to use as a time
   // reference.  Enable SysTick to generate interrupts.
   SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SECOND);
   SysTickIntEnable();
   SysTickEnable();
}
