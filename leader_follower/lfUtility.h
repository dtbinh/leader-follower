/*
 * lfUtility.h
 *
 *  Created on: Apr 20, 2016
 *      Author: user
 */

#ifndef LFUTILITY_H_
#define LFUTILITY_H_

// Define NULL
#ifndef NULL
#define NULL                    ((void *)0)
#endif

// States of the follower robot
typedef enum FollowerState
{
   SEARCH,
   FOLLOW
} FollowerState;

#endif /* LFUTILITY_H_ */
