
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
    if( INTCONbits.TMR0IF ){        
    	time.updatesRequired++;			///< Flag another update for the main
    	INTCONbits.TMR0IF = 0; 			///< Clear the interrupt flag
    }
}

#pragma code highPriorityInterruptAddress=0x08
void high_interrupt( void ){       
    _asm GOTO highPriorityISR _endasm
}

void main( void ){

    timeTag nextUpdate = {0,0,0,0};

    setupRealTimeTimer();				///< See definition for full functionality

			
	TRISBbits.RB0 = 0; 					// Setup portB for an output to test timing
    TRISCbits.RC6 = 0; 					// Setup portB for an output to test timing
    TRISBbits.RB2 = 0; 					// Setup portB for an output to test timing
	PORTBbits.RB0 = 0;					// turn off RB0
    PORTBbits.RB1 = 0;					// turn off RB0
    PORTCbits.RC6 = 0;					// turn off RB0

    setTimeTag( 2, &nextUpdate );

	while(1){
        /// Test code to check the timers opperation
        updateTime();					// Update the time
        
//        if( time.milliseconds%15 == 0 ){
//        	PORTBbits.RB0 = 1;			// Turn on RB0 after 3 seconds
//        } else {
//            PORTBbits.RB0 = 0;
//        }
        PORTBbits.RB2 ^= 1;
        /// This will cause a blink once per second.
        if ( eventDue( &nextUpdate ) ){
            PORTCbits.RC6 ^= 1;
            setTimeTag( 200, &nextUpdate );
        }
  }
}

 #pragma config OSC = HS
 #pragma config PWRT = ON
 #pragma config WDT  = OFF, WDTPS = 1
 #pragma config LVP  = OFF
 #pragma config DEBUG = OFF
 #pragma config CP0  = OFF,CP1 = OFF,CP2 = OFF,CP3 = OFF,CPB = OFF,CPD = OFF
 #pragma config WRT0 = OFF,WRT1 = OFF,WRT2 = OFF,WRT3 = OFF,WRTB = OFF,WRTC = OFF,WRTD = OFF
 #pragma config EBTR0 = OFF,EBTR1 = OFF,EBTR2 = OFF,EBTR3 = OFF,EBTRB = OFF

