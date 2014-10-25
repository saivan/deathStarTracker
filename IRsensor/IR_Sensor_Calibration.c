/* 
 * File:   IR_Sensor_Calibration.c
 * Author: David
 *
 * Created on 11 October 2014, 10:33 PM
 */

#include "masterHeader.h"

/**
 * This file exists solely to test the IR_Sensor file.
 *
 */

#pragma code
void main(void) {
    int i = 0;
    TRISA = 0xFF;   //set PORTA to input
    TRISB = 0;
    TRISD = 0;
    LCDInitialise();    //initialise LCD
    currentIRDistance = 0;
    while(1){
        LCDMoveCursor(0,0);
        Delay10TCYx(2);
        IRSensor();
        if (i== 0){
            i++;
            calibrateIR();
        }
        intToDisplay(currentIRDistance/10);
        LCDWriteHere(displayChars.characters);
        Delay10TCYx(5);
    }



//    while (1){
//        LCDMoveCursor(0,0);
//        delayMs(2);
//        intToDisplay(i);
//        LCDWriteHere(displayChars.characters);
//        delayMs(5);
//        i++;
//    }

}

// CONFIG1H
#pragma config OSC = HS       // Oscillator Selection bits (RC oscillator w/ OSC2 configured as RA6)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

