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
#include "utils/ustdlib.h"

#include "lfDisplay.h"
#include "lfMotors.h"
#include "lfSensors.h"
#include "lfUtility.h"


/****************************************************************
 * These two preprocessors dictate which binary you are building.
 * They are mutually exclusive options preprocessors. Only one of
 * them should be uncommented at a time.
 * **************************************************************/
#define LEADER_ROBOT
//#define FOLLOWER_ROBOT

// Current state of the robot's autonomy state machine
RobotState currentState;


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
   lfUpdateSensorDataDisplay(FOLLOW, leftDist, rightDist);
}

// State machine implementing run-time logic for leader or follower
static void runStateMachine(void)
{
   //lfUpdateDisplay(FOLLOW, 5, 15);

	// Initialize the state machine
#ifdef FOLLOWER_ROBOT
	currentState = SEARCH;
#endif

#ifdef LEADER_ROBOT
	currentState = WANDER;
#endif

   while(1)
   {
#ifdef FOLLOWER_ROBOT
     pollAvgSensorVal();

	  if(currentState == FOLLOW)
      {
    	  //follow();
      }
      else if(currentState == SEARCH)
      {
    	  //search();
      }
#endif
#ifdef LEADER_ROBOT
      if(currentState == WANDER)
      {
         //wander();
      }
#endif
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

   // Initialize the IR sensors only for the follower robot
#ifdef FOLLOWER_ROBOT
   lfSensorsInit();
#endif

   // Enable interrupts to the CPU
   IntMasterEnable();
}
/**
 * Main function - leader_follower
 */
int main(void)
{
   // Seed the pseudo-random number generator
   unsigned int seed = 0;
   usrand(seed);

   initialize();

   runStateMachine();

   return 0;
}
