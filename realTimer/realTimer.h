/* 
 * File:   realTimer.h
 * Author: saivanhamama
 *
 * Created on October 13, 2014, 8:44 PM
 */

#ifndef REALTIMER_H
#define	REALTIMER_H

#define CCP_SECOND 160000

/**
 * @struct time Variables related to the real time
 * @brief Stores timekeeping variables
 * @details milliseconds and quarts updated only on request
 */
typedef struct time {
	char hours;						///< The time in hours since system booted
	char minutes;					///< The time in minutes since the last hour
	char seconds;					///< The time in seconds since the last minute
	unsigned int milliseconds;		///< The time in milliseconds since the last second
	char sixteenths;				///< Sixteenths of a millisecond since the last millisecond
} time;

/// Function forward declartions go here
void setupRealTimeTimer( void );
void updateMilliseconds( void );

#endif	/* REALTIMER_H */

