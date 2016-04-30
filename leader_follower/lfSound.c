/*
 * lfSound.c
 *
 *  Created on: Apr 25, 2016
 *      Author: user
 */

#include "inc/hw_types.h"

#include "driverlib/udma.h"
#include "drivers/dac.h"
#include "drivers/sound.h"
#include "drivers/wav.h"

#include "sound_array.h"
#include "lfSound.h"

// Identifies a .wav
typedef struct tWaveClip
{
   const unsigned char *pucWav;
   const char * pcName;
} tWaveClip;

// The DMA control structure table.  This is required by the sound driver.
#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable sDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(sDMAControlTable, 1024)
tDMAControlTable sDMAControlTable[64];
#else
tDMAControlTable sDMAControlTable[64] __attribute__ ((aligned(1024)));
#endif

void lfPlaySound()
{
   // play a tune
   const tWaveClip waveClip = { SOUND, "Wav1"};
   tWaveHeader sWaveHeader;
   if(WaveOpen((unsigned long *) waveClip.pucWav, &sWaveHeader) == WAVE_OK)
   {
      WavePlayStart(&sWaveHeader);
      while (!WavePlayContinue(&sWaveHeader))
      {
      }
   }
}

void lfSoundInit()
{
   // Initialize the sound driver
   SoundInit();
}
