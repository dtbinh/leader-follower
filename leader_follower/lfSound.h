/*
 * lfSound.h
 *
 *  Created on: Apr 25, 2016
 *      Author: user
 */

#ifndef LFSOUND_H_
#define LFSOUND_H_

#include "lfUtility.h"

// Initiate playback of the sound corresponding to each state.
void lfPlaySound(RobotState state);

// Continue playback of a sound, if any (scheduled task).
void lfUpdateSound(void *pvParam);

// Initialize the sound functions.
void lfSoundInit(void);

#endif /* LFSOUND_H_ */
