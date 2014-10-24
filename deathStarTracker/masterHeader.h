

/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

#ifndef SERVOS_H
#define	SERVOS_H

#include <p18f4520.h>
#include <timers.h>
#include <delays.h>

#include "LCD.h"
#include "realTimer.h"
#include "tracking.h"
#include "servo.h"

#define TRUE 1
#define FALSE 0

void highPriorityISR( void );
void lowPriorityISR( void );

#endif	/* SERVOS_H */

