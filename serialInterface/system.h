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

#include <P18F4520.h>

#include "serialInterface.h"
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
} SystemFlags;

extern SystemFlags systemFlags;
extern void toggleFactoryMode(void);
extern void toggleRemoteMode(void);
extern void showChildOptions(void);
extern void showTargetStatus(void);
extern void showTemperature(void);

#endif	/* SYSTEM_H */

