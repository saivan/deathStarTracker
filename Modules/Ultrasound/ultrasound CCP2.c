/*
;**********************************************************
;*  proj_ultrasound.c
;**********************************************************
;*  Michael Rowlinson 312139837 Group Germain
;*  29 September 2014
;*  Assembled with MPLAB X IDE v2.15
;**********************************************************
;*  Pin RC2 used to initilize echos
;*  Pin RC1 used to recieve the echo THIS MUST BE SELECTED IN CCP2MUX
;*  Timer 0 used in capture mode on the rising edge of echo
;**********************************************************
 */

#include <p18cxxx.h>
#include <timers.h>
//#include "ConfigRegs18f4520.h"
#include <p18f452.h>
#include "realTimer.h"
#include "LCD.h"
//#include <ConfigRegs.h> //make sure CCP2MUX is set to RC1


#define     INITPIN     PORTCbits.RC2
#define     INITTRIS    TRISCbits.RC2
#define     ECHOPIN     PORTCbits.RC1
#define     ECHOTRIS    TRISCbits.RC1
#define     OVERFCONST  0xFF

unsigned int  milliTime = 0;
unsigned int  subMilliTime = 0;
unsigned char USStatus = 0;
unsigned char USArrayPosition = 0;
volatile unsigned char USOverF = 0;
unsigned int distance = 0;
unsigned int tempDistance = 0;
unsigned int speed = 0;
unsigned int distPerMs = 0;
unsigned int distPerSubMs = 0;
unsigned int UScapturedValue[10];
timeTag echo = {0,0,0,0};
unsigned char test;


void high_interrupt(void);
void highPriorityIsr(void);
void fireEcho (void);
void echoCalc (void);
void low_interrupt(void);

#pragma code lowerPriorityInterruptAddress=0x0008
void low_interrupt(void) {
    _asm GOTO highPriorityIsr _endasm
}

#pragma code highPriorityInterruptAddress=0x0018
void high_interrupt(void) {
    _asm GOTO highPriorityIsr _endasm
}


void setup(void) {
    INITPIN = 0;
    ECHOTRIS = 1;                   ///<Input for Echo
    INITTRIS = 0;                   ///<Output for INIT
    INTCONbits.GIE_GIEH = 1;        ///<Turn on interrupts
    INTCONbits.PEIE_GIEL = 1;

    PIR2bits.CCP2IF = 0;
    CCP2CON = 0x00; ///<capture mode, every rising edge
    T3CONbits.RD16 = 0; ///<set to 8 bit mode
    T3CONbits.T3CCP2 = 0;
    T3CONbits.T3CCP1 = 1; ///<timer 3 used for CCP2
    T3CONbits.T3CKPS1 = 0;
    T3CONbits.T3CKPS0 = 0; ///<prescaler is 1:1
    T3CONbits.TMR3ON = 1;

    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 0;

    milliTime = 0;
    subMilliTime = 0;
    distance = 0;
    setupRealTimeTimer();
    speed = 34000;
    distPerMs = speed/200;
    distPerSubMs = speed/4000;
    USOverF = 0;
    USStatus = 0;
    USArrayPosition = 0;
}

void main(void) {

    setup();
    LCDInitialise();
    test = 1;
    while (test != 0) //delay for device to ready
    {
        test++;
    }
    fireEcho();
    while (1)
    {
        if (TMR3H > 0x07 && USStatus) {
            CCP2CON = 0x05; ///<capture mode, every rising edge
        }

        if (PIR2bits.CCP2IF && USStatus) {
            PIR2bits.CCP2IF = 0;
            UScapturedValue[USArrayPosition] = ReadTimer3();
            USArrayPosition++;
            if (USArrayPosition == 5) {
                echoCalc();
                Delay10KTCYx(1);
                intToDisplay(distance);
                LCDMoveCursor(1,0);
                Delay10KTCYx(1);
                LCDWriteHere(displayChars.characters);
                }
            USStatus = 2;
        }
        if (USOverF > 0) {
            fireEcho();
        }
    }
}

void fireEcho (void) {
    INITPIN = 0;
    CCP2CON = 0x00;
    USStatus = 1;
    distance = 0;
    milliTime = 0;
    TMR3L = 0;
    TMR3H = 0;
    USOverF = 0;
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
    INITPIN = 1;                    //Start transmitting echos*/
}

void echoCalc (void) {
    unsigned char i = 0;
    distance = 0;
    while (i < USArrayPosition) {
        milliTime = UScapturedValue[i]/1000;
        subMilliTime = (UScapturedValue[i] - milliTime*1000)/50;
        if ((subMilliTime*50 + 25) < (UScapturedValue[i] - milliTime*1000)) {
            subMilliTime = subMilliTime + 1;
        }
        tempDistance = milliTime*distPerMs/10 + subMilliTime*distPerSubMs/10;
        if (tempDistance < 45 || tempDistance > 210) {
            tempDistance = 0;
        }
        i++;
        distance += tempDistance;
    }
    distance = distance/USArrayPosition;
    USArrayPosition = 0;
}

#pragma interrupt highPriorityIsr
void highPriorityIsr (void) {
     if( INTCONbits.TMR0IF ){
        time.updatesRequired++;	///< Flag another update for the main
        INTCONbits.TMR0IF = 0; ///< Clear the interrupt flag
    }
     if(PIR2bits.TMR3IF && PIE2bits.TMR3IE) {
         USOverF++;
         PIR2bits.TMR3IF = 0;
     }
}

#pragma config OSC = HS
#pragma config PWRT = ON
#pragma config WDT = OFF, WDTPS = 1
#pragma config LVP = OFF
#pragma config DEBUG = OFF
#pragma config CP0 = OFF,CP1 = OFF,CP2 = OFF,CP3 = OFF,CPB = OFF,CPD = OFF
#pragma config WRT0 = OFF,WRT1 = OFF,WRT2 = OFF,WRT3 = OFF,WRTB = OFF,WRTC = OFF,WRTD = OFF
#pragma config EBTR0 = OFF,EBTR1 = OFF,EBTR2 = OFF,EBTR3 = OFF,EBTRB = OFF
#pragma config CCP2MUX = ON
