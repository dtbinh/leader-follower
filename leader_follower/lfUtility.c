/*
 * lfUtility.c
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/ustdlib.h"

#include "lfUtility.h"

// Define maximum random number
#define RAND_MAX     32767

// The number of SysTick ticks per second used for the SysTick interrupt.
#define SYSTICKS_PER_SECOND     1000

# define CM_PER_INCH     2.54

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

unsigned int randInterval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = urand();
    } while (r >= limit);

    return min + (r / buckets);
}

float inchesToCm(float inches)
{
   float cm;
   cm = inches * CM_PER_INCH;

   return cm;
}

float cmToInches(float cm)
{
   float inches;
   inches = cm / CM_PER_INCH;

   return inches;
}

int intFloor(double x)
{
    return (int)(x+100000) - 100000;
}

void lfUtilInit()
{
   // Configure SysTick to occur n times per second, to use as a time
   // reference.  Enable SysTick to generate interrupts.
   SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SECOND);
   SysTickIntEnable();
   SysTickEnable();
}
