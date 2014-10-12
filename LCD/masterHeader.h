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

typedef struct displayDigit{
    unsigned char upper;
    unsigned char middle;
    unsigned char lower;
} displayDigit;

#include "LCD.h"



#endif	/* MASTERHEADER_H */

