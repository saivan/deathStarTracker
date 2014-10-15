
#include <stdio.h>
#define TRUE 1
#define FALSE 0


/**
 * @struct timeTag Stores the time when an event should run
 * @brief The time for the next event 
 */
typedef struct timeTag {
	unsigned char hours;							///< The time in hours since system booted
	unsigned char minutes;							///< The time in minutes since the last hour
	unsigned char seconds;							///< The time in seconds since the last minute
	unsigned int milliseconds;						///< The time in milliseconds since the last second		
} timeTag;


typedef struct Time {
	unsigned char hours;							///< The time in hours since system booted
	unsigned char minutes;							///< The time in minutes since the last hour
	unsigned char seconds;							///< The time in seconds since the last minute
	unsigned int milliseconds;						///< The time in milliseconds since the last second		
} Time;

void setTimeTag( int msToNextEvent, timeTag *eventTag );
char eventDue( timeTag *eventTag );
















Time time = {0,59,59,200};

int main(void){

	timeTag nextEvent = {0,0,0,0};

	setTimeTag(999, &nextEvent);
	time.milliseconds = 198;
	time.seconds = 0;
	time.minutes = 1;
	time.hours = 1;

	printf("hours \ntime: %i \t evt: %i \n", time.hours, nextEvent.hours );
	printf("minutes \ntime: %i \t evt: %i \n", time.minutes, nextEvent.minutes );
	printf("seconds \ntime: %i \t evt: %i \n", time.seconds, nextEvent.seconds );
	printf("milliseconds \ntime: %i \t evt: %i \n\n", time.milliseconds, nextEvent.milliseconds );

	printf("%i\n",eventDue(&nextEvent));

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
	eventTag->milliseconds += msToNextEvent;			///< Add the milliseconds till the next event
	if( eventTag->milliseconds > 999 ){					///< If we've overflowed the milliseconds
		eventTag->milliseconds -= 1000;					///< Subtract 1000ms and
		eventTag->seconds++;							///< Increment to the next second
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
	if( time.hours < eventTag->hours ) 
		return FALSE;
	else if ( (time.minutes < eventTag->minutes) && (time.hours == eventTag->hours) )
		return FALSE;
	else if ( (time.seconds < eventTag->seconds) && (time.minutes == eventTag->minutes) )
		return FALSE;
	else if ( (time.milliseconds < eventTag->milliseconds) && (time.seconds == eventTag->seconds) )
		return FALSE;
	else 
		return TRUE;
}



