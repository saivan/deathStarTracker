
#include "masterHeader.h"

void buttonsInitialisation(void);
void gotoHighISR (void);
void highPriorityISR (void);

volatile char gotShit = 0;


/* High Priority Interrupt Service Routine */
#pragma code highISR = 0x0008
void gotoHighISR(void)
{
    _asm goto highPriorityISR _endasm   /* Simply go to 'highPriorityISR' */
}



#pragma code

void main( void ) {                 /*set up main with initialisation function, then remain in infinite loop*/
    buttonsInitialisation();
    TRISD = 0;
    LCDInitialise();
    while(1){
        if(gotShit){
            LCDMoveCursor(0,0);
            intToDisplay(buttonPressValue);
            LCDWriteHere(displayChars.characters);
            delayMs(1);


            LCDMoveCursor(1,0);
            intToDisplay(interuptCount);
            LCDWriteHere(displayChars.characters);
            delayMs(1);
            gotShit = 0;
        }
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


    



