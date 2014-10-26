

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


int m = 0;
int n = 0;

static char LCDState = 0;


char Hello[] = "DeathStarTracker";
timeTag doThingo = { 0, 0, 0, 0 };
timeTag LCDUpdate = { 0, 0, 0, 2 };
timeTag ServoPositionUpdate = { 0, 0, 0, 2 };
timeTag moveServo = { 0, 0, 0, 2 };

void main( void ){





	/// Setup Routines go here
	LCDInitialise();   
	USSetup();
	setupRealTimeTimer();
    setupServos();


    LCDWriteHere( Hello );
    LCDMoveCursor(1,0);

    // Test code to test shit...
	TRISC = 0x00;
        
	while(1){
        
        updateTime();

        /// Update the LCD
        Nop();
        if( eventDue(&LCDUpdate) ){
        	/// Display stuff to the screen
            Nop();
        	if( LCDState == 0 ){        		
        		LCDInstruction(CLEAR_LCD,COMMAND_LCD);
        		setTimeTag(5,&LCDUpdate);
        		LCDState++;
        	} else if ( LCDState == 1 ){
       			LCDMoveCursor(0,0);
       			setTimeTag(2,&LCDUpdate);
                LCDState++;
       		} else if ( LCDState == 2 ){
       			LCDWriteHere( Hello );
                LCDState++;
       		} else if ( LCDState == 3 ){
       			LCDMoveCursor(1,0);
                LCDState++;
       		} else if ( LCDState == 4 ){
       			intToDisplay(35,4);
       			LCDWriteHere(displayChars.characters);
                LCDState=0;
       		}
		}

        Nop();

        /// Make the LED Blink
        if(eventDue(&doThingo)){
        	PORTCbits.RC6 ^= 1;
        	setTimeTag(500,&doThingo);
        }

		/// Move the servos
		if( eventDue(&moveServo) ){
           	m+=20;
			n+=20;

			if( m > 1800 ){
				m = 0;
			}

			if( n > 1800 ){
				n = 0;
			}

			updateCCPServoAngle(m,n);
			setTimeTag(50,&moveServo);
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

