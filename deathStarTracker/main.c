

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


unsigned char m = 0;
unsigned char n = 0;

static char LCDState = 0;


char Hello[] = "DeathStarTracker";
timeTag doThingo = { 0, 0, 0, 0 };
timeTag LCDUpdate = { 0, 0, 0, 2 };
timeTag ServoPositionUpdate = { 0, 0, 0, 2 };
timeTag moveServo = { 0, 0, 0, 2 };

void main( void ){


	// Test code to test shit...
	TRISC = 0x00;


	/// Setup Routines go here
	LCDInitialise();   
	setupRealTimeTimer();
        setupServos();

        LCDWriteHere( Hello );
        LCDMoveCursor(1,0);

	while(1){
        
        updateTime();

        /// Update the LCD
        if( eventDue(&LCDUpdate) ){
        	/// Display stuff to the screen
        	if( LCDState == 0 ){
        		intToDisplay((unsigned int)(m));	
        		
        		setTimeTag(15,&LCDUpdate);
        	} else if ( LCDState == 1 ){
        		LCDMoveCursor(1,0);
        	} else if ( LCDState == 2 ){

        	}


			
			LCDWriteHere(displayChars.characters);
			
			
		}

        /// Make the LED Blink
        if(eventDue(&doThingo)){
        	PORTCbits.RC6 ^= 1;
        	setTimeTag(999,&doThingo);
        }

		/// Move the servos
		if( eventDue(&moveServo) ){
           	m+=2;
			n+=2;

			if( m > 180 ){
				m = 0;
			}

			if( n > 180 ){
				n = 0;
			}

			updateCCPServoAngle(m,n);
			setTimeTag(10,&moveServo);
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

