/* 
 * File:   masterHeader.h
 * Author: saivanhamama
 *
 * Created on September 30, 2014, 7:22 PM
 */

#ifndef MASTERHEADER_H
#define	MASTERHEADER_H

#include <p18f452.h>
#include "LCD.h"
#include "userLocal.h"


#define TRUE 1
#define FALSE 0

struct time{
    unsigned int minutes;
    unsigned int microseconds;
    unsigned char sixteenths : 4;
};


#include "LCD.h"



#endif	/* MASTERHEADER_H */

