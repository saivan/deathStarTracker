
/* 
 * File:   timer.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 5:58 PM
 */

#include "masterHeader.h"

// The timer should work in this mode
#pragma interrupt highPriorityISR
void highPriorityISR( void ){
	/// Serve the capture compare interrupt
	if( PIR2bits.CCP2IF ){
		time.seconds++; 				///< Increment seconds
		if( time.seconds == 0 )
			time.minutes++;				///< If seconds overflowed, increment minutes
		if( time.minutes == 0 )
			time.hours++;				///< If minutes overflowed, increment hours
		PIR2bits.CCP2IF = 0; 			///< Clear the interrupt flag
	}
}

#pragma code highPriorityInterruptAddress=0x08
void high_interrupt( void ){   
	if( PIR2bits.CCP2IF ){
		
	}
    _asm GOTO highPriorityISR _endasm
}

void main( void ){

	setupRealTimeTimer();				///< See definition for full functionality
	
//	if( /*/EXPECTED TIME*/ )
    
}



 #pragma config OSC  = HS
 #pragma config PWRT = ON
 #pragma config WDT  = OFF, WDTPS = 1
 #pragma config LVP  = OFF
 #pragma config DEBUG = OFF
 #pragma config CP0  = OFF,CP1 = OFF,CP2 = OFF,CP3 = OFF,CPB = OFF,CPD = OFF
 #pragma config WRT0 = OFF,WRT1 = OFF,WRT2 = OFF,WRT3 = OFF,WRTB = OFF,WRTC = OFF,WRTD = OFF
 #pragma config EBTR0 = OFF,EBTR1 = OFF,EBTR2 = OFF,EBTR3 = OFF,EBTRB = OFF


