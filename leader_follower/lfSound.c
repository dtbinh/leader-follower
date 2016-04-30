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

// Structure containing metadata for .wav that is currently playing.
static tWaveHeader gblWaveHeader;

// waves
static const tWaveClip FOLLOW_WAVE = { SOUND_FOLLOW, "Wav1"};
static const tWaveClip SEARCH_WAVE = { SOUND_SEARCH, "Wav2"};

void lfPlaySound(FollowerState state)
{
   tWaveClip waveClip;
   if (state == FOLLOW)
   {
      waveClip = FOLLOW_WAVE;
   }
   else if (state == SEARCH)
   {
      waveClip = SEARCH_WAVE;
   }
   else
   {
      return;
   }

   // Stop an existing tune.
   WaveStop();

   if(WaveOpen((unsigned long *) waveClip.pucWav, &gblWaveHeader) == WAVE_OK)
   {
      WavePlayStart(&gblWaveHeader);
   }
}

void lfUpdateSound(void *pvParam)
{
   if (WavePlaybackStatus())
   {
      WavePlayContinue(&gblWaveHeader);
   }
}

void lfSoundInit(void)
{
   // Initialize the sound driver
   SoundInit();
}
