/* 
 * File:   realTimer.h
 * Author: saivanhamama
 *
 * Created on October 13, 2014, 8:44 PM
 */

#ifndef REALTIMER_H
#define	REALTIMER_H

#define CLOCK_MSUP 1
#define CCP_SECOND 160000

/**
 * @struct time Variables related to the real time
 * @brief Stores timekeeping variables
 * @details milliseconds and quarts updated only on request
 */
typedef struct Time {
	unsigned char hours;					///< The time in hours since system booted
	unsigned char minutes;					///< The time in minutes since the last hour
	unsigned char seconds;					///< The time in seconds since the last minute
	unsigned int milliseconds;				///< The time in milliseconds since the last second
	unsigned char sixteenths;				///< Sixteenths of a millisecond since the last millisecond
	volatile unsigned char updatesRequired;			///< Count milliseconds since last update
} Time;

/// Function forward declartions go here
void setupRealTimeTimer( void );
void updateTime( void );

/// Declare the time for the system to use
extern volatile Time time;


#endif	/* REALTIMER_H */

