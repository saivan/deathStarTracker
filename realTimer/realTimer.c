
/* 
 * File:   timer.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 5:58 PM
 */

#include "masterHeader.h"

volatile Time time = {0,0,0,0,0};     // Declare the time in the local scope initialised to zero

/**
 * @brief Sets up Timer0 to work as a real time clock
 * @details Sets up Timer0 with a prescaler of 16 to overflow
 *          once every millisecond. You can read the value of
 *          Timer0 for submillisecond precision if you need it
 */
void setupRealTimeTimer( void ){
    /// Turning on timer 0 and setting it up
    T0CONbits.T08BIT = 1;				///< Use timer 0 in 8 bit mode
    T0CONbits.T0CS = 1;					///< Use the external clock
    T0CONbits.PSA = 0;					///< Use a hardware prescaler
    T0CONbits.T0PS = 0b011;				///< Set up a 16 prescaler
    T0CONbits.TMR0ON = 1;				///< Turn on timer 0
    /// Turning on the overflow interrupts and setting priorities
    INTCON2bits.TMR0IP = 1;				///< Set TMR0 overflow to high Priority
    INTCONbits.TMR0IE = 1; 				///< Enable TMR0 Interrupts
    RCONbits.IPEN =1;					///< Enable interrupt priorities
    INTCONbits.GIEH = 1;				///< Enable high priority interrupts
}

/**
 * @brief 	Updates the systemwide time
 * @details Call this function when you need the time in milliseconds updated
 * 			forgetting to call this function will keep the old value in the timers
 */
void updateTime( void ){
    /// Check if we need to update the time
	if( time.updatesRequired > 0 ){		
		/// Update the time in milliseconds
        time.milliseconds += time.updatesRequired; 			///< Increment the milliseconds
        time.updatesRequired = 0;							///< Reset updates to zero as they are done
        if( time.milliseconds > 999 ){
        	time.milliseconds -= 1000;						///< If we reached 1 second, we subtract a second 		
			time.seconds++;									///< And register the new second                
			if( time.seconds > 59 ){      
				time.seconds = 0;							///< If we reached 1 minute, we reset seconds
				time.minutes++;								///< Then register the new minute
				if( time.minutes > 59 ){
					time.minutes = 0;						///< We just reached an hour, so set minutes to zero
					time.hours++;							///< Increment hours
				}
			}
		}
	}
}


/**
 * @brief Sets the time for the next event
 * @details Takes the 
 * 
 * @param eventTag The event that is expected to occur
 * @param msToNextEvent Time in milliseconds till the next event : 10 < msToNextEvent < 999
 */
void setTimeTag( int msToNextEvent, timeTag *eventTag ){
	/// Copy all of the current time information to the eventTag
	eventTag->milliseconds = time.milliseconds;
	eventTag->seconds = time.seconds;
	eventTag->minutes = time.minutes;
	eventTag->hours = time.hours;

	/// Add the required milliseconds to the current time
	eventTag->milliseconds += msToNextEvent;
	if( eventTag->milliseconds > 999 ){
		eventTag->milliseconds -= 1000;
		eventTag->seconds++;
		if( eventTag->seconds > 59 ){
			eventTag->seconds = 0;
			eventTag->minutes++;
			if( eventTag->minutes > 59 ){
				eventTag->minutes = 0;
				eventTag->hours++;
			}
		}
	}
}

/**
 * @brief Checks if the time for an event has been reached 
 * @param eventTag The event to check, should use a timeTag
 * @return true if the time has been exceeded, otherwise false
 */
char eventDue( timeTag *eventTag ){
	
	return TRUE;
}




