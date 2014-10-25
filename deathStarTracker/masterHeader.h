

/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

#ifndef MASTERHEADER_H
#define	MASTERHEADER_H

#include <p18f4520.h>
#include <timers.h>
#include <delays.h>

#include "realTimer.h"
#include "servo.h"
#include "LCD.h"
//#include "tracking.h"

#define TRUE 1
#define FALSE 0

void highPriorityISR( void );
void lowPriorityISR( void );

#endif	/* MASTERHEADER_H */

