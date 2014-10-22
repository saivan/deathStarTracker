/*
 * File:   system.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:58 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H
/*
 * File:   system.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#define USING_PIC18F4520

#ifdef  USING_PIC18F4520
#include <P18F4520.h>
#endif

#ifdef  USING_PIC18F452
#include <P18F452.h>
#endif

#include "tree.h"
#include "remoteInterface.h"
#include "interface.h"

/* Global Defines for Useful Constants */
#define FULL    0xFF
#define ZERO    0x00
#define NULL    0x00
#define EVER    ;;

/* System Flags */
typedef struct
{
	unsigned int remote : 1;
	unsigned int factory : 1;
	unsigned int targetfound : 1;
	unsigned int userYes : 1;
	unsigned int optionsShown : 1;
        unsigned int cueOverflow : 1;
        unsigned int commandReceived : 1;
        unsigned int numberInput : 1;
        unsigned int userError : 1;
} SystemFlags;

extern SystemFlags systemFlags;
extern void showTargetStatus(void);
extern void showTemperature(void);

#endif	/* SYSTEM_H */

