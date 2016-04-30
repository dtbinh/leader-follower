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

// Number of system ticks per second
#define TICKS_PER_SECOND 100

// scheduled function prototypes
static void runStateMachine(void *pvParam);

// Arguments to display at every scheduled display task.
static DisplayArgs gblDisplayArgs =
{
   .state = SEARCH,
   .distanceL = -1,
   .distanceR = -1
};

// This table defines all the tasks that the scheduler is to run, the periods
// between calls to those tasks, and the parameter to pass to the task.
tSchedulerTask g_psSchedulerTable[] =
{
   { runStateMachine,      NULL,             0, 0, true },  // run every time
   { lfUpdateSound,        NULL,             4, 0, true },  // run every 40 ms
   { lfUpdateDisplayTask,  &gblDisplayArgs,  10, 0, true }  // run every 100 ms
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
static void pollAvgSensorVal(IrDistance * const leftDist,
                             IrDistance * const rightDist)
{
   if ((leftDist == NULL) || (rightDist == NULL))
   {
      return;
   }

   unsigned long irLeftVal;
   unsigned long irRightVal;
   lfSensorsGetReading(IR_LEFT, &irLeftVal);
   lfSensorsGetReading(IR_RIGHT, &irRightVal);

    // output est distance
   lfSensorsMapDistance(irLeftVal, leftDist);
   lfSensorsMapDistance(irRightVal, rightDist);
}

// State machine implementing run-time logic for leader or follower (scheduled task)
// This function must not sleep.
void runStateMachine(void *pvParam)
{
   static FollowerState state = SEARCH;

   // always check distance
   IrDistance leftDist;
   IrDistance rightDist;
   pollAvgSensorVal(&leftDist, &rightDist);

   if (state == FOLLOW)
   {
      // update display args
      gblDisplayArgs.state       = FOLLOW;
      gblDisplayArgs.distanceL   = leftDist;
      gblDisplayArgs.distanceR   = rightDist;
   }
   else if (state == SEARCH)
   {
      // update display args
      gblDisplayArgs.state       = SEARCH;
      gblDisplayArgs.distanceL   = -1;
      gblDisplayArgs.distanceR   = -1;
   }

   // change states ever 10 seconds for verification
   static unsigned long lastChange = 0;
   if (SchedulerElapsedTicksGet(lastChange) > (TICKS_PER_SECOND * 10))
   {
      lastChange = SchedulerTickCountGet();

      state = (state == FOLLOW) ? SEARCH : FOLLOW;

      lfPlaySound(state);
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
