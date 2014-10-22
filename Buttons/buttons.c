
#include "masterHeader.h"


#define     HIGH 1
#define     LOW 0
#define     OUTPUT 0
#define     INPUT  0xff

volatile int interuptCount = 0;

int buttonPressValue = 0;
int bufferLocation = 0;
char buttonBuffer[4];
//ButtonFlags buttonFlags;        //type is ButtonFlags and name is buttonFlags. don't want to call same thing

char RC6pin = LOW;

void buttonsInitialisation(void){
   
    TRISCbits.RC4 = OUTPUT;                   ///< set RC7 to output for TEST
    TRISB = INPUT;
    TRISBbits.RB4 = INPUT;
    
   
    ///< Interrupt initialisation
    RCONbits.IPEN = HIGH;                       ///< enable priority interrupts
    INTCONbits.GIEH = HIGH;                     ///< enables all high priority interrupts
    INTCONbits.RBIE = HIGH;
    INTCONbits.RBIF = LOW;
    INTCON2bits.RBIP = HIGH;

    PORTC = LOW;
}
///< remove jumpers 20 and 21 (These were misfiring our digital port b interrupts. RB5 connected to TXD and TXD connected to RC6 abnd RC6 is whatever it was last time and happened to be high and was triggering interrupt
#pragma interrupt highPriorityISR          /*define receive interrupt service routine*/
void highPriorityISR(void){
    if (INTCONbits.RBIF){                   ///< if evaluates to 0, doesnt run. If evaluates to anything else, will run it
        buttonPressValue = PORTB & 0x0F;
        if (bufferLocation <= 4){
        buttonBuffer[bufferLocation] = buttonPressValue;
        bufferLocation++;
        } else {
            // FORCE AN ENTER PRESS
        }
        interuptCount++;
        INTCONbits.RBIF = LOW;
        gotShit = 1;
    }

}