

#include "masterHeader.h"

#pragma code highPriorityInterruptAddress=0x08
void high_interrupt( void ){
  if( PIR1bits.CCP1IF )
      WriteTimer1(0);                             // Reset the Timer
  _asm GOTO highPriorityISR _endasm
}

// #pragma code lowPriorityInterruptAddress=0x18
// void high_interrupt( void ){
//     _asm GOTO highPriorityISR _endasm
// }


unsigned int m = 0;
char Hello[] = "DeathStarTracker";
timeTag doThingo = { 0, 0, 0, 0 };
timeTag LCDUpdate = { 0, 0, 0, 2 };


void main( void ){

	// Test code to test shit...
	TRISC = 0x00;


	/// Setup Routines go here
	LCDInitialise();   
	setupRealTimeTimer();

        LCDWriteHere( Hello );
        LCDMoveCursor(1,0);

	while(1){
            updateTime();

        if( eventDue(&LCDUpdate) ){
			/// Display stuff to the screen
			intToDisplay(m);
			LCDWriteHere(displayChars.characters);
			setTimeTag(15,&LCDUpdate);
			LCDMoveCursor(1,0);
			m++;
	        if( m == 999){
	             m = 0;
	             PORTCbits.RC7 ^= 1;
	        }
		}

        /// Make the LED Blink
        if(eventDue(&doThingo)){
        	PORTCbits.RC7 ^= 1;
        	setTimeTag(500,&doThingo);
        }

		/// Update the time

		/// Update display values if required

		/// Handle local mode or remote mode interface actions

		/// Run a tracking step
		

	}
}


 #pragma config OSC  = HS
 #pragma config PWRT = ON
 #pragma config WDT  = OFF, WDTPS = 1
 #pragma config LVP  = OFF
 #pragma config DEBUG = OFF
 #pragma config CP0  = OFF,CP1 = OFF,CP2 = OFF,CP3 = OFF,CPB = OFF,CPD = OFF
 #pragma config WRT0 = OFF,WRT1 = OFF,WRT2 = OFF,WRT3 = OFF,WRTB = OFF,WRTC = OFF,WRTD = OFF
 #pragma config EBTR0 = OFF,EBTR1 = OFF,EBTR2 = OFF,EBTR3 = OFF,EBTRB = OFF
















