

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
#include "ultrasound.h"
#include "servo.h"
#include "LCD.h"
#include "tracker.h"
#include "IRSensor.h"

/// The system flags
typedef struct {
 		unsigned int remote : 1;
 		unsigned int factory : 1;
 		unsigned int optionsShown : 1;
        unsigned int commandReceived : 1;
        unsigned int numberInput : 1;
        unsigned int userError : 1;
        unsigned int upPressed : 1;
        unsigned int downPressed : 1;
        unsigned int userChosen : 1;
        unsigned int updatePrompt : 1;
        unsigned int numberParsed : 1;
        unsigned int LCDRequiresUpdate : 1;
} SystemFlags;
extern SystemFlags systemFlags;

#define TRUE 1
#define FALSE 0

void highPriorityISR( void );
void lowPriorityISR( void );

#endif	/* MASTERHEADER_H */

