

#include "masterHeader.h"
#include "Configurations.h"

SystemFlags systemFlags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


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


int m = 1800;
int n = 1800;

static char LCDState = 0;


char Hello[] = "DeathStarTracker";
timeTag doThingo = { 0, 0, 0, 0 };
timeTag ServoPositionUpdate = { 0, 0, 0, 2 };
timeTag moveServo = { 0, 0, 0, 2 };


void setup( void ){	
	/// Setup Routines go here
	setupRealTimeTimer();
	LCDInitialise();   
        setupServos();
	USSetup();
}


void main( void ){	

	setup();
    LCDWriteHere( Hello );
    LCDMoveCursor(1,0);    
        
    while(1){

        updateTime();

        track();

        /// Update the LCD
        if( eventDue(&LCDUpdate) ){
        	/// Display stuff to the screen
        	if( LCDState == 0 ){        		
        		LCDInstruction(EMPTY_DISPLAY,COMMAND_LCD);
        		setTimeTag(2,&LCDUpdate);
        		LCDState++;
       		} else if ( LCDState == 1 ){
       			LCDWriteHere( Hello );
       			LCDMoveCursor(1,0);
       			setTimeTag(2,&LCDUpdate);
                LCDState++;
       		} else if ( LCDState == 2 ){
       			intToDisplay((int)(IRDistance),0);
       			LCDWriteHere(displayChars.characters);
       			LCDMoveCursor(1,10);
       			setTimeTag(2,&LCDUpdate);
                LCDState++;
       		} else if( LCDState == 3 ){
       			intToDisplay((int)(trackFlags.targetFound),0);
       			LCDWriteHere(displayChars.characters);       			
       			setTimeTag(35,&LCDUpdate);
       			LCDState = 0;
       		}
		}


	}
}
