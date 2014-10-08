

#include "masterHeader.h"

char topLine[16] = "shrivelled";

void stringToRam( static char rom *source, static char *destination ){
    while( (*destination++ = *source++) != '\0' ){
    }
}




void main( void ){

    char lcdChar[] = "=";
    char lcdChar2[] = "behaving";
    unsigned char i = 0;
    char *pointToChar = &lcdChar[0];

//    stringToRam( LCDStrings[3], topLine );

    // Set ports to outputs
    TRISD = 0x00;
    TRISB = 0x00;
    
    LCDInitialise();
    
    LCDWriteHere( topLine );
    delayMs(1);
    LCDWriteHere( LCDSElevation );
    delayMs(1);
    LCDMoveCursor( 1, 1 );
    delayMs(1);
    LCDWriteHere( setParam.bottomLineArray[1] );

    while(1){
        PORTB = 0xFF;
        PORTBbits.RB0 = 0;
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


