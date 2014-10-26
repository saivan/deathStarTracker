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

unsigned int  milliTime;
unsigned int  subMilliTime;
unsigned char USFireStatus;
unsigned char USSampleSize;
unsigned char USArrayPosition;

unsigned int USDistance;
unsigned int USMaxRange;
unsigned int USMinRange;
unsigned int tempDistance;
unsigned int speed;
unsigned int distPerMs;
unsigned int distPerSubMs;
unsigned int UScapturedValue[10];
unsigned char voidUSData;
unsigned char i;
timeTag echoCanFire = {0,0,0,0};
unsigned int test;


void high_interrupt(void);
void highPriorityIsr(void);
void fireEcho (void);
void echoCalc (void);
void testUSState (void);
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

    milliTime = 0;
    subMilliTime = 0;
    USDistance = 0;                         ///<holds current value of calc distance
    setupRealTimeTimer();
    speed = 34000;                          ///<speed in cm/s
    distPerMs = speed/200;                  ///<divides by 2 for travel, 100 to be mm/ms
    distPerSubMs = speed/4000;              ///<divides by 20 times to get 20 subsections
    USFireStatus = 0;                       ///<status of 0 means nothing is occuring, 1 means echo is incoming and has not been calculated yet
    USSampleSize = 2;                       ///<starting values
    USMinRange = 45;
    USMaxRange = 210;
    USArrayPosition = 0;
}

void main(void) {

    setup();
    LCDInitialise();
    fireEcho();
    while (1)
    {
        test = 0;
        while (test < 10000) {
            test++;             ///<delay to simulate other
        }
        testUSState();
    }
}
/**
 * @brief Tests the current state of the US
 * @details Call this function to test what
 *          the state is and what needs to
 *          be done. The If statements:
 *      1)  Turns on the CCP2 if the noise threshold
 *          has been passed.
 *      2)  If an echo has not been recieved by then
 *          it is out of range,calls calc passing 0
 *      3)  Echo recieved, calls calc passing captured
 *          cycle value
 *      4)  If 80ms has passed, fire the next echo
 */
void testUSState (void){
    if (TMR3H > 0x07 && USFireStatus) {  ///< change to 0x0D
        CCP2CON = 0x05; ///<capture mode, every rising edge
    }
    if (TMR3H > 0x3B && USFireStatus) {  ///<change to 0x58
        UScapturedValue[USArrayPosition] = 0x00;
        echoCalc();
        CCP2CON = 0x00;                         ///<turn of ccp since no value is coming
        USFireStatus = 0;                       ///<reset status
    }

    if (PIR2bits.CCP2IF && USFireStatus) {
        PIR2bits.CCP2IF = 0;
        UScapturedValue[USArrayPosition] = ReadTimer3();    ///<store captured time into array
        echoCalc();

        Delay10KTCYx(1);                                    //display block, remove when needed
        intToDisplay(USDistance);
        LCDMoveCursor(1,0);
        Delay10KTCYx(1);
        LCDWriteHere(displayChars.characters);

        CCP2CON = 0x00;
        USFireStatus = 0;
    }

    if (eventDue(&echoCanFire)){                            ///<this flag must be true for the US to fire again
        fireEcho(); //this currently fires the echo every 80ms
    }
}

/**
 * @brief Call this function to start an echo
 *        transmission.
 * @details Clears timer 3, sets initpin high
 *          and sets the time tag for next echo
 *          to be in 80 ms
 */
void fireEcho (void) {
    INITPIN = 0;
    PIR2bits.CCP2IF = 0;
    USFireStatus = 1;               ///<change status to be waiting for echos
    updateTime();
    storeCurrentTime(&echoCanFire);
    setTimeTag(&echoCanFire,80);    ///<time tag for when next echo can be sent
    TMR3L = 0;
    TMR3H = 0;
    INITPIN = 1;                    ///<Start transmitting echos*/
}

/**
 * @brief This function deals with data processing from US input
 * @details it updates USDistance to contain the latest moving average
 *          calculations. Any value that is above or below the min/max range
 *          is treated as void,and given a value of 0.
 *
 */
void echoCalc (void) {
    USDistance = 0;
    voidUSData = 0;
    if (UScapturedValue[USArrayPosition] != 0) {
        milliTime = UScapturedValue[USArrayPosition]/1000; //divide by 2500
        subMilliTime = (UScapturedValue[USArrayPosition] - milliTime*1000)/50; //multiply by 2500
        if ((subMilliTime*50 + 25) < (UScapturedValue[USArrayPosition] - milliTime*1000)) { //multiply by 2500
            subMilliTime = subMilliTime + 1;
        }
        UScapturedValue[USArrayPosition] = milliTime*distPerMs/10 + subMilliTime*distPerSubMs/10;
    }
    i = 0;
    while (i < USSampleSize) {  ///<checks for void values
        tempDistance = UScapturedValue[i];
        if (tempDistance < USMinRange || tempDistance > USMaxRange) {
            tempDistance = 0;
            voidUSData++;
        }
        USDistance += tempDistance;
        i++;
    }

    if (voidUSData != USSampleSize) {
        USDistance = USDistance/(USSampleSize-voidUSData); ///<average distance values
    }
    USArrayPosition++;
    if (USArrayPosition >= USSampleSize) {
        USArrayPosition = 0; ///<reset poisition if over buffer
    }
}

#pragma interrupt highPriorityIsr
void highPriorityIsr (void) {
     if( INTCONbits.TMR0IF ){
        time.updatesRequired++;	///< Flag another update for the main
        INTCONbits.TMR0IF = 0; ///< Clear the interrupt flag
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
