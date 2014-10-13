

/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

#ifndef SERVOS_H
#define	SERVOS_H

#include <p18f452.h>
#include <timers.h>

void highPriorityISR( void );
void lowPriorityISR( void );

#endif	/* SERVOS_H */

