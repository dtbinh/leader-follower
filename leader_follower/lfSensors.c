/*
 * lfSensors.c
 *
 *  Created on: Apr 25, 2016
 *      Author: user
 */

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "lfSensors.h"

// ADC block
#define IR_ADC_BASE  ADC0_BASE

// Sequencers to use for sensors (4 deep)
#define ADC_SEQUENCER_LEFT    1UL
#define ADC_SEQUENCER_RIGHT   2UL

#define ADC_CHAN_LEFT         ADC_CTL_CH11 // PB5 pin 91 (EM1 4th from top left)
#define ADC_CHAN_RIGHT        ADC_CTL_CH10 // PB4 pin 92 (EM1 5th from top left)

// Timer block
#define TIMER_BASE TIMER0_BASE

// Depth of averaging array
#define IR_AVG_DEPTH 5
#define IR_IDX_MAX   (IR_AVG_DEPTH - 1)

// arrays used to average the last few IR readings
volatile unsigned long gblIrLeftVal[IR_AVG_DEPTH];
volatile unsigned long gblIrRightVal[IR_AVG_DEPTH];
volatile unsigned int gblIrLeftIdx = 0;
volatile unsigned int gblIrRightIdx = 0;

// Timer interrupt handler. Trigger next ADC reading.
void TimerIntHandler(void)
{
    // Clear the timer interrupt flag.
    TimerIntClear(TIMER_BASE, TIMER_TIMA_TIMEOUT);

    // Trigger the ADC conversion.
    ADCProcessorTrigger(IR_ADC_BASE, ADC_SEQUENCER_LEFT);
    ADCProcessorTrigger(IR_ADC_BASE, ADC_SEQUENCER_RIGHT);
}

// ADC Interrupt handlers. Read into index.
void AdcLeftHandler(void)
{
   // Clear the ADC interrupt flag.
   ADCIntClear(IR_ADC_BASE, ADC_SEQUENCER_LEFT);

   // read into the least recently used slot
   unsigned long * const sValPtr = (unsigned long * const)&gblIrLeftVal[gblIrLeftIdx];

   // Read ADC Value
   ADCSequenceDataGet(IR_ADC_BASE, ADC_SEQUENCER_LEFT, sValPtr);

   // update least recently used index
   if (gblIrLeftIdx < IR_IDX_MAX)
   {
      ++gblIrLeftIdx;
   }
   else
   {
      gblIrLeftIdx = 0;
   }
}

void AdcRightHandler(void)
{
   // Clear the ADC interrupt flag.
   ADCIntClear(IR_ADC_BASE, ADC_SEQUENCER_RIGHT);

   // read into the least recently used slot
   unsigned long * const sValPtr = (unsigned long * const)&gblIrRightVal[gblIrRightIdx];

   // Read ADC Value
   ADCSequenceDataGet(IR_ADC_BASE, ADC_SEQUENCER_RIGHT, sValPtr);

   // update least recently used index
   if (gblIrRightIdx < IR_IDX_MAX)
   {
      ++gblIrRightIdx;
   }
   else
   {
      gblIrRightIdx = 0;
   }
}

void lfSensorsGetReading(const IRSensors sensor,
                         unsigned long * const sVal)
{
   unsigned long avgVal = 0;
   if (sensor == IR_LEFT)
   {
      for (unsigned int i = 0; i < IR_IDX_MAX; ++i)
      {
         avgVal += gblIrLeftVal[i];
      }
   }
   else if (sensor == IR_RIGHT)
   {
      for (unsigned int i = 0; i < IR_IDX_MAX; ++i)
      {
         avgVal += gblIrRightVal[i];
      }
   }
   else
   {
      *sVal = 0;
      return;
   }

   avgVal /= IR_IDX_MAX;
   *sVal = avgVal;
}

void lfSensorsMapDistance(const unsigned long sVal,
                          IrDistance * const distance)
{
   if (sVal > DIST_BIN_10)
   {
      *distance = DIST_MIN;
   }
   else if (sVal > DIST_BIN_15)
   {
      *distance = DIST_10;
   }
   else if (sVal > DIST_BIN_20)
   {
      *distance = DIST_15;
   }
   else if (sVal > DIST_BIN_25)
   {
      *distance = DIST_20;
   }
   else if (sVal > DIST_BIN_30)
   {
      *distance = DIST_25;
   }
   else if (sVal > DIST_BIN_35)
   {
      *distance = DIST_30;
   }
   else if (sVal > DIST_BIN_40)
   {
      *distance = DIST_35;
   }
   else if (sVal > DIST_BIN_45)
   {
      *distance = DIST_40;
   }
   else if (sVal > DIST_BIN_50)
   {
      *distance = DIST_45;
   }
   else if (sVal > DIST_BIN_55)
   {
      *distance = DIST_50;
   }
   else if (sVal > DIST_BIN_60)
   {
      *distance = DIST_55;
   }
   else if (sVal > DIST_BIN_70)
   {
      *distance = DIST_60;
   }
   else if (sVal > DIST_BIN_80)
   {
      *distance = DIST_70;
   }
   else if (sVal > DIST_BIN_90)
   {
      *distance = DIST_80;
   }
   else if (sVal > DIST_BIN_MAX)
   {
      *distance = DIST_90;
   }
   else
   {
      *distance = DIST_MAX;
   }
}

// Initialize timer used to drive ADC
static void lfTimerInit(void)
{
   // Enable Timer0 peripheral.
   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

   // Configure as a 32-bit periodic timer.
   TimerConfigure(TIMER_BASE, TIMER_CFG_PERIODIC);

   // Set the load value to 100ms.
   TimerLoadSet(TIMER_BASE, TIMER_BOTH, SysCtlClockGet() / 10);

   // Configure the interrupt for timer timeout.
   TimerIntEnable(TIMER_BASE, TIMER_TIMA_TIMEOUT);

   // Enable the timer interrupt on the processor (NVIC).
   IntEnable(INT_TIMER0A);

   // Enable timer.
   TimerEnable(TIMER_BASE, TIMER_A);
}

void lfSensorsInit()
{
   lfTimerInit();

   // Enable ADC0 peripheral
   SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

   // Enable GPIO port B (PB4, PB5)
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

   // Select the ADC function for these pins.
   GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

   // Configure two sequencers, both with identical depth, one per
   // ADC input. They have identical priority and are sampled upon request.
   ADCSequenceConfigure(IR_ADC_BASE, ADC_SEQUENCER_LEFT, ADC_TRIGGER_PROCESSOR, 0);
   ADCSequenceConfigure(IR_ADC_BASE, ADC_SEQUENCER_RIGHT, ADC_TRIGGER_PROCESSOR, 0);

   ADCSequenceStepConfigure(IR_ADC_BASE, ADC_SEQUENCER_LEFT, 0, ADC_CHAN_LEFT | ADC_CTL_IE | ADC_CTL_END);
   ADCSequenceStepConfigure(IR_ADC_BASE, ADC_SEQUENCER_RIGHT, 0, ADC_CHAN_RIGHT | ADC_CTL_IE | ADC_CTL_END);

   // enable maximum oversampling (~1ms penalty per reading)
   ADCHardwareOversampleConfigure(IR_ADC_BASE, 64);

   // Enable interrupts and register handlers
   ADCIntEnable(IR_ADC_BASE, ADC_SEQUENCER_LEFT);
   ADCIntEnable(IR_ADC_BASE, ADC_SEQUENCER_RIGHT);

   ADCIntRegister(IR_ADC_BASE, ADC_SEQUENCER_LEFT, AdcLeftHandler);
   ADCIntRegister(IR_ADC_BASE, ADC_SEQUENCER_RIGHT, AdcRightHandler);

   // enable sequences
   ADCSequenceEnable(IR_ADC_BASE, ADC_SEQUENCER_LEFT);
   ADCSequenceEnable(IR_ADC_BASE, ADC_SEQUENCER_RIGHT);
}
