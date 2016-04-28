/*
 * lfSensors.c
 *
 *  Created on: Apr 25, 2016
 *      Author: user
 */

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "lfSensors.h"

// Sequencers to use for sensors (4 deep)
#define ADC_SEQUENCER_LEFT    1UL
#define ADC_SEQUENCER_RIGHT   2UL

#define ADC_CHAN_LEFT         ADC_CTL_CH11 // PB5 pin 91 (EM1 4th from top left)
#define ADC_CHAN_RIGHT        ADC_CTL_CH10 // PB4 pin 92 (EM1 5th from top left)

void lfSensorsGetReading(const IRSensors sensor,
                         unsigned long * const sVal)
{
   unsigned long sequencer = ADC_SEQUENCER_RIGHT;

   if (sensor == IR_LEFT)
   {
      sequencer = ADC_SEQUENCER_LEFT;
   }
   else if (sensor == IR_RIGHT)
   {
      sequencer = ADC_SEQUENCER_RIGHT;
   }
   else
   {
      *sVal = 0;
      return;
   }

   // Trigger the ADC conversion.
   ADCProcessorTrigger(ADC0_BASE, sequencer);

   // Wait for conversion to be completed.
   while(!ADCIntStatus(ADC0_BASE, sequencer, false))
   {
   }

   // Clear the ADC interrupt flag.
   ADCIntClear(ADC0_BASE, sequencer);

   // Read ADC Value.
   ADCSequenceDataGet(ADC0_BASE, sequencer, sVal);
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

void lfSensorsInit()
{
   // Enable ADC0 peripheral
   SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

   // Enable GPIO port B (PB4, PB5)
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

   // Select the analog ADC function for these pins.
   GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

   // Configure two sequencers, both with identical depth, one per
   // ADC input. They have identical priority and are sampled upon request.
   ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENCER_LEFT, ADC_TRIGGER_PROCESSOR, 0);
   ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENCER_RIGHT, ADC_TRIGGER_PROCESSOR, 0);

   ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCER_LEFT, 0, ADC_CHAN_LEFT | ADC_CTL_IE | ADC_CTL_END);
   ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCER_RIGHT, 0, ADC_CHAN_RIGHT | ADC_CTL_IE | ADC_CTL_END);

   // Clear the interrupt status flag.
   ADCIntClear(ADC0_BASE, ADC_SEQUENCER_LEFT);
   ADCIntClear(ADC0_BASE, ADC_SEQUENCER_RIGHT);

   // enable maximum oversampling (~1ms penalty per reading)
   ADCHardwareOversampleConfigure(ADC0_BASE, 64);

   // enable sequences
   ADCSequenceEnable(ADC0_BASE, ADC_SEQUENCER_LEFT);
   ADCSequenceEnable(ADC0_BASE, ADC_SEQUENCER_RIGHT);
}
