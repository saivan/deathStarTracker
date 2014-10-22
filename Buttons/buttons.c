
#include "Header.h"

#define     HIGH 1
#define     LOW 0
#define     OUTPUT 0
#define     INPUT  0xff
#define     DOWNBUTTON   10
#define     UPBUTTON     11
#define     GOBUTTON     12
#define     BACKBUTTON   13
//#define CLEARB PortBbits.RB3 = 0;

#define BUTTONBUFFERSIZE 4
unsigned char buttonBuffer[BUTTONBUFFERSIZE] = { 0, 0, 0, 0 };

typedef struct buttonFlags{
/// Execution Flags
unsigned char buttonPressed : 1; ///< A button has been pressed and we need to handle it
unsigned char acceptingInput : 1; ///< The keypad is active and buttons can be pressed
unsigned char numericalInput : 1; ///< The progam is expecting numerical input (up and down disabled)
/// Specific Button Flags
unsigned char upPressed : 1;
unsigned char downPressed : 1;
unsigned char goPressed : 1;
unsigned char backPressed : 1;
unsigned char bufferFull : 1;
} buttonFlags;

unsigned char buttonPressValue = 0;
unsigned char bufferLocation = 0;
unsigned char interruptCount = 0;

void buttonsInitialisation(void){
    TRISB = INPUT;

    ///< Interrupt initialisation
    RCONbits.IPEN = HIGH;                       ///< enable priority interrupts
    INTCONbits.GIEH = HIGH;                     ///< enables all high priority interrupts. MAYBE NEED GIEL IF NOT WORKING?
    INTCONbits.GIEL = HIGH;
    INTCON2bits.INTEDG1 = HIGH;
    INTCONbits.RBIE = LOW;
    INTCON3bits.INT1IP = HIGH;
    INTCON3bits.INT1IE = HIGH;
    INTCON3bits.INT1IF = LOW;

    //< Don't have to enable INT0 priority as it is always high priority

}

///< remove jumpers 20 and 21 on the PICDEM (These were misfiring our digital port b interrupts. RB5 connected to TXD and TXD connected to RC6 abnd RC6 is whatever it was last time and happened to be high and was triggering interrupt
#pragma interrupt highPriorityISR          /*define receive interrupt service routine*/
void highPriorityISR(void){
    if (INTCONbits.RBIF){                   ///< if evaluates to 0, doesnt run. If evaluates to anything else, will run it
        buttonPressValue = PORTB >> 2;
        buttonPressValue = buttonPressValue & 0x0F;
        if (bufferLocation < 5){ //< use < if possible otherwise doing unnecessary condition
            buttonBuffer[bufferLocation] = buttonPressValue;
            bufferLocation++;
        } else {
            buttonFlags.bufferFull = 1; //< THIS SHOULD THEN PROMPT A MESSAGE TO THE USER TO PRESS ENTER
            // do nothing and FORCE AN ENTER PRESS
        }
//        interruptCount++;
        INTCONbits.RBIF = LOW;
        buttonFlags.buttonPressed = 1;
    }
}

void buttonHandling (void){
//    buttonBuffer1 = buttonBuffer[0];
//    buttonBuffer2 = buttonBuffer[1];
//    buttonBuffer3 = buttonBuffer[2];
//    buttonBuffer4 = buttonBuffer[3];
//
//    if (buttonBuffer1 = 10){
//        buttonFlags
//    }

    // ECHO
    if(buttonPressValue < 10)
    {
        //ECHO TO LCD. lcdWrite(buttonPressValue);
    }
    else
    {
    
    }
    
}



