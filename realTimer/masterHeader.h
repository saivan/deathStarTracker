
/* 
 * File:   masterHeader.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 6:00 PM
 */

#ifndef MASTERHEADER_H
#define	MASTERHEADER_H

/**
 * Including all of the external headers
 */
#include <p18f452.h>
#include "realTimer.h"

/**
 * @struct 	Time
 * @brief	Stores the time for system wide opperations
 * @details Used to keep systemwide time for various modules which are time dependent, 
 * 			including the timing and execution of tasks.
 */
typedef struct time {
    unsigned char hours;				///< The time in hours since launch
    unsigned char minutes;				///< The time in minutes since the last hour
    unsigned char seconds;				///< The time in seconds since the last minute
	unsigned int milliseconds;			///< The time in milliseconds since the last second
	unsigned char quarts : 4;			///< The time in sixteenths of a second since the last millisecond
} time;

#endif	/* MASTERHEADER_H */

