/* 
 * File:   system.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:58 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H


#include <P18F4520.h>

#include "serialInterface.h"



/* Global Defines for Useful Constants */
#define FULL    0xFF
#define ZERO    0x00
#define EVER    ;;

/* System Flags */
typedef struct
{
  unsigned int remote: 1;
  unsigned int factory: 1;
} SystemFlags;



#endif	/* SYSTEM_H */

