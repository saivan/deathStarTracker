
/* 
 * File:   timer.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 5:58 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#define SECOND_CCPVAL


typedef struct timeTag {
    unsigned int microseconds;
    unsigned int minutes;
    unsigned int quarts : 4;
} timeTag;

void setupRealTimeTimer( void );
char timeGreaterThan( timeTag *refTime );

#endif	/* TIMER_H */
