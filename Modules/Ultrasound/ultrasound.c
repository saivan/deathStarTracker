/*
;**********************************************************
;*  proj_ultrasound.c
;**********************************************************
;*  Michael Rowlinson 312139837 Group Germain
;*  29 September 2014
;*  Assembled with MPLAB X IDE v2.15
;**********************************************************
;*  Pin RC1 used to initilize echos
;*  Pin RB5 used to recieve the echo
;*  Timer 0 used in capture mode on the rising edge of echo
;**********************************************************
 */

#include <p18cxxx.h>
#include <timers.h>
//#include "ConfigRegs18f4520.h"
#include <p18f452.h>
#include "realTimer.h"
#include "LCD.h"


//#define     INITPIN     PORTCbits.RC1
//#define     INITTRIS    TRISCbits.RC1
#define     INITPIN     PORTBbits.RB2
#define     INITTRIS    TRISBbits.RB2
#define     ECHOPIN     PORTBbits.RB1
#define     ECHOTRIS    TRISBbits.RB1
#define     OVERFCONST  0xFF

unsigned int  TMR0StartMilli;
unsigned int  TMR0StartSubMilli;
unsigned int  TMR0EndMilli;
unsigned int  TMR0EndSubMilli;
unsigned int  milliTime;
unsigned int  subMilliTime;
unsigned char done;
unsigned char i;
unsigned int distance;
unsigned int speed;
unsigned int distPerMs;
unsigned int distPerSubMs;
timeTag echoStartTime = {0,0,0,0};
timeTag echoEndTime = {0,0,0,0};
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
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG2 = 1;
    INTCON3bits.INT1IP = 1;

    TMR0StartMilli = 0;
    TMR0EndMilli = 0;
    TMR0StartSubMilli = 0;
    TMR0EndSubMilli = 0;
    milliTime = 0;
    subMilliTime = 0;
    distance = 0;
    setupRealTimeTimer();
    speed = 34000;
    distPerMs = speed/2000;
    distPerSubMs = speed/16000;
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
    done = 2;
    while (1)
    {
//        char string[] =  "bah humbug";
//        updateTime();
//        intToDisplay(0);
//        LCDMoveCursor(0,0);
//        delayMs(1);
//        LCDWriteHere( string );
//        delayMs(1);
//        intToDisplay(distPerMs);
//        LCDMoveCursor(1,0);
//        delayMs(1);
//        LCDWriteHere(displayChars.characters);

        if ((distance == 0) && done == 1) {
            echoCalc();
        }
//        updateTime();
//        if (eventDue(&echo) && done == 0 && INTCON3bits.INT1IE == 0){
//            INTCON3bits.INT1IF = 0;
//            INTCON3bits.INT1IE = 1;
//        }

    }
}

void fireEcho (void) {
    INITPIN = 0;
    done = 0;
    distance = 0;
    milliTime = 0;
    updateTime();
    TMR0StartSubMilli = ReadTimer0();
    storeCurrentTime(&echoStartTime);
    storeCurrentTime(&echo);
    INITPIN = 1;                    //Start transmitting echos*/                 
    setTimeTag(2,&echo);
}

void echoCalc (void) {
    
//    INTCONbits.INT0IE = 0;
    INTCON3bits.INT1IE = 0;
    TMR0StartMilli = echoStartTime.milliseconds;
    TMR0EndMilli = echoEndTime.milliseconds;
    if (TMR0StartMilli > TMR0EndMilli) { ///<checking for overflows
        TMR0EndMilli = TMR0EndMilli + 0x3E8; ///<adds 1000 to the end so calculation still works
    }
    milliTime = TMR0EndMilli - TMR0StartMilli; ///<gives the time taken in milliseconds
    if (TMR0StartSubMilli >= TMR0EndSubMilli) { ///<case where the start is a higher value then the end
        subMilliTime = 0xFF - (TMR0StartSubMilli - TMR0EndSubMilli); ///< gives the correct value without going above FF
    }
    else {
        subMilliTime = TMR0EndSubMilli - TMR0StartSubMilli; ///<case where the end is a higher value then the start
    }
    i = subMilliTime >> 5;                ///<bit shift to store 3 most sig bits
    distance = i*distPerSubMs;                         ///<simplification of distance from submilliseconds
    distance = distance + milliTime*distPerMs;     ///<simplification of distance from milliseconds
    INITPIN = 0;
    done = 2;
    //storeCurrentTime(echo);
    //setTimeTag(900,echo);
}

#pragma interrupt highPriorityIsr
void highPriorityIsr (void) {
    updateTime();
    if (INTCON3bits.INT1IF && INTCON3bits.INT1IE && eventDue(&echo)) {     ///<When echo is recieved as high
        updateTime();
        TMR0EndSubMilli = ReadTimer0();            ///<check submilli interval
        storeCurrentTime(&echoEndTime);             ///<store final time
        done = 1;                                    ///<show it is ready for calc
        INTCON3bits.INT1IF = 0;
    }
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
