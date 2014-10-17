

#include "masterHeader.h"

char topLine[16] = "Events: ";

//This is a cool

void main( void ){

    char lcdChar[] = "=";
    char lcdChar2[] = "behaving";
    unsigned char i = 0;
    char *pointToChar = &lcdChar[0];
    unsigned int m = 1;
    //    stringToRam( LCDStrings[3], topLine );

    // Set ports to outputs
    TRISD = 0x00;
    TRISB = 0x00;
    
    LCDInitialise();


//    intToDisplay(35);
//    LCDMoveCursor(0,7);
//    delayMs(1);
//    LCDWriteHere( displayChars.characters );
//    delayMs(1);
//    // LCDWriteHere( topLine );
//    // delayMs(1);

        LCDMoveCursor(0,0);
        delayMs(5);
        intToDisplay(6);
//        LCDWriteHere( displayChars.characters );
        LCDWriteHere( topLine );
        delayMs(45);


    while(1){
        LCDMoveCursor(0,8);
        delayMs(5);
        intToDisplay(m);
        LCDWriteHere( displayChars.characters );
        delayMs(5);

        m++;
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


