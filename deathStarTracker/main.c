

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


void main( void ){


	while(1){
		
		/// Update the time

		/// Update display values if required

		/// Handle local mode or remote mode interface actions

		/// Run a tracking step
		

	}
}











