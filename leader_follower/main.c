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
#include "utils/scheduler.h"

#include "lfDisplay.h"
#include "lfMotors.h"
#include "lfSensors.h"
#include "lfSound.h"
#include "lfUtility.h"

// Define NULL
#ifndef NULL
#define NULL                    ((void *)0)
#endif

// Number of system ticks per second
#define TICKS_PER_SECOND 100

// scheduled function prototypes
static void runStateMachine(void *pvParam);

// This table defines all the tasks that the scheduler is to run, the periods
// between calls to those tasks, and the parameter to pass to the task.
tSchedulerTask g_psSchedulerTable[] =
{
   { runStateMachine, NULL, 0, 0, true },
   { lfUpdateSound,   NULL, 4, 0, true }
};

// Indices of the various tasks described in g_psSchedulerTable.
#define TASK_STATE_MACHINE 0
#define TASK_UPDATE_SOUND  1

// The number of entries in the global scheduler task table.
unsigned long g_ulSchedulerNumTasks = (sizeof(g_psSchedulerTable) /
                                       sizeof(tSchedulerTask));

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
   unsigned long irLeftVal;
   unsigned long irRightVal;

   lfSensorsGetReading(IR_LEFT, &irLeftVal);
   lfSensorsGetReading(IR_RIGHT, &irRightVal);

    // output est distance
   IrDistance leftDist;
   IrDistance rightDist;
   lfSensorsMapDistance(irLeftVal, &leftDist);
   lfSensorsMapDistance(irRightVal, &rightDist);
   lfUpdateDisplay(FOLLOW, leftDist, rightDist);
}

// State machine implementing run-time logic for leader or follower (scheduled task)
// This function must not sleep.
void runStateMachine(void *pvParam)
{
   pollAvgSensorVal();

   // change states ever 10 seconds
   static unsigned long lastChange = 0;
   if (SchedulerElapsedTicksGet(lastChange) > (TICKS_PER_SECOND * 10))
   {
      lastChange = SchedulerTickCountGet();
      lfPlaySound();
   }
}

// Perform module initialization
static void initialize(void)
{
   // Set the system clock to run at 50MHz from the PLL
   SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   lfMotorsInit();
   lfSensorsInit();
   lfDisplayInit();
   lfSoundInit();

   // Initialize the task scheduler.
   SchedulerInit(TICKS_PER_SECOND);

   // Enable interrupts to the CPU
   IntMasterEnable();
}
/**
 * Main function - leader_follower
 */
int main(void)
{
   initialize();

   while(1)
   {
      // Tell the scheduler to call any periodic tasks that are due to be called.
      SchedulerRun();
   }

   return 0;
}
