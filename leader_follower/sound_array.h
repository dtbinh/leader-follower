/*
 * header.h
 *
 *  Created on: Apr 7, 2016
 *      Author: user
 */

#ifndef SOUND_ARRAY_H_
#define SOUND_ARRAY_H_

#include <stddef.h>

extern const unsigned char SOUND[];

extern const unsigned char g_ucSoundWav1[]; // voice
extern const unsigned char g_ucSoundWav2[]; // boing
extern const unsigned char g_ucSoundWav3[]; // boing
extern const unsigned char g_ucSoundWav4[]; // horn
extern const unsigned char g_ucSoundWav5[]; // gong
extern const unsigned char g_ucSoundWav6[]; // tap

// The chosen sounds
// Can only include in a limited number of sounds due to space
//#define  S_GUITAR
//#define S1
#define S2
//#define S3
#define S4
//#define S5
//#define S6

#define SOUND_SEARCH g_ucSoundWav2
#define SOUND_FOLLOW g_ucSoundWav4

#endif /* SOUND_ARRAY_H_ */
