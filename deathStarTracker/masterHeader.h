

/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

#ifndef MASTERHEADER_H
#define	MASTERHEADER_H


#define USING_PIC18F4520

#ifdef  USING_PIC18F4520
#include <P18F4520.h>
#endif

#ifdef  USING_PIC18F452
#include <P18F452.h>
#endif

#include <timers.h>
#include <delays.h>

#include "realTimer.h"
#include "ultrasound.h"
#include "servo.h"
#include "LCD.h"
#include "tracker.h"
#include "IRSensor.h"
#include "tree.h"
#include "remoteInterface.h"
#include "localInterface.h"
#include "stateFunctions.h"
#include "interface.h"
#include "buttons.h"

/// The system flags
typedef struct {
        unsigned int toTrack : 1;
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
        unsigned int userInputStored : 1;
        unsigned int numberParsed : 1;
        unsigned int LCDRequiresUpdate : 1;
} SystemFlags;

#define TRUE 1
#define FALSE 0

/* Global Defines for Useful Constants */
#define	BIT(x)	(1 << (x))
#define FULL    0xFF
#define ZERO    0x00
#define NULL    0x00
#define EVER    ;;

#define HIGH    1
#define LOW     0
#define TRUE    1
#define FALSE   0
#define OUTPUT  ZERO
#define INPUT   FULL
#define LOWNIBBLE   0x0F
#define HIGHNIBBLE  0xF0

/* System Flags */
typedef struct
{
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
        unsigned int userInputStored : 1;
} SystemFlags;

extern SystemFlags systemFlags;
extern void systemReset(void);
void highPriorityISR( void );
void lowPriorityISR( void );

#endif	/* MASTERHEADER_H */

