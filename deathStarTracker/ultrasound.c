
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

#include "masterHeader.h"


/// The external variables are all declared here
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
unsigned int UScapturedValue[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
timeTag echo = {0,0,0,0};


/**
 * @brief [brief description]
 * @details [long description]
 */
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


/**
 * @brief [brief description]
 * @details [long description]
 */
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



/**
 * @brief [brief description]
 * @details [long description]
 */
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


