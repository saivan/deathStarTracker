
/* 
 * File:   timer.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 5:58 PM
 */

#include "masterHeader.h"

 time time;            					// Declare the time in the local scope

/**
 * @brief 	Sets up timer3 to work as a real time clock
 * @details Sets up the CCP2 mode to work with timer 3 and also
 * 			to interrupt when the CCP value has been reached.
 * 			
 * 			Timer3 is clocked from an external 4.096MHz clock.
 * 			With a prescaler of 255, it becomes a 16KHz clock
 * 			so we have up to 16 'submilliseconds' to work with
 *
 *			TMR3<0:3> will represent the time in quarts, whilst
 *			TMR3<4:15> represents the time in milliseconds 
 *			TMR3 is reset when it reaches 16000quarts
 */
void setupRealTimeTimer( void ){
    /// Turning on and setting up timer 3
    T3CONbits.TMR3ON = 1; 				///< Turn on the timer
    T3CONbits.T3SYNC = 0; 				///< Use an external clock
    T3CONbits.TMR3CS = 1; 				///< Use the clock on T13CKl    
    TRISCbits.RC0 = 1; 					///< Set T13CKl (RC0) to an input Pin 
    /// Setting up the CCP3 Module to work in compare mode
    T3CONbits.T3CCP2 = 1;               ///< Use timer3 as the CCP source
    CCP2CONbits.CCP2M = 0b1010;			///< Generate interrupt on successful compare
	
    /// Turning on the interrupts and setting priorities
    IPR2bits.CCP2IP = 1;				///< Set CCP2 to high Priority
    PIE2bits.CCP2IE = 1; 				///< Enable CCP2 compare interrupts
    INTCONbits.GIEH = 1;				///< Enable high priority interrupts
}

/**
 * @brief 	Updates the time in miliseconds and quarts when needed
 * @details Call this function when you need the time in milliseconds updated
 * 			forgetting to call this function will keep the old value in the timers
 */
void updateMilliseconds( void ){
	/// Get the Timer value, so that it doesnt change during execution
	char timer3HighBits = TMR3H;
	char timer3LowBits = TMR3L;
	/// Set TMR3<0:3> to †he time in quarts
	time.sixteenths = (TMR3L)&(0x0F);	///< Get the low bytes and set them to the time in quarts
	/// Set TMR3<4:13> to the time in miliseconds
	time.milliseconds = (TMR3H<2);		///< Update the high bytes
	if ( (TMR3L&(1<<6)) != 0 )
		time.milliseconds++;			///< If the lowest bit is set, add one        
	if ( (TMR3L&(1<<7)) != 0 )
		time.milliseconds+=2;			///< If the next lowest bit is set, add two        
}

