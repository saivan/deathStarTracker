

#include "masterHeader.h"

//#pragma code highPriorityInterruptAddress=0x08
//void high_interrupt( void ){
//    if( PIR1bits.CCP1IF )
//        WriteTimer1(0);                             // Reset the Timer
//    _asm GOTO highPriorityISR _endasm
//}

// #pragma code lowPriorityInterruptAddress=0x18
// void high_interrupt( void ){
//     _asm GOTO highPriorityISR _endasm
// }


unsigned int m = 0;
char Hello[] = "hello";

void main( void ){

	/// Setup Routines go here
	LCDInitialise();    

	while(1){

		intToDisplay(m);
		LCDWriteHere(displayChars.characters);
		Delay10KTCYx(5);
		LCDMoveCursor(0,0);
		m++;
                if( m == 999){
                    m = 0;
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
















