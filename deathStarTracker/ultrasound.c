
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
unsigned char USArrayPosition = 0;
unsigned int speedOfSound = 0;
unsigned int distPerMs = 0;
unsigned int distPerSubMs = 0;
unsigned int UScapturedValue[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
timeTag echoCanFire = {0,0,0,0};
USFlagType USFlags = {0, 0};
USValueType USValues = {0,0,0,0,0};

/**
 * @brief sets up the US]
 * @details [configures timer 3 and sets initial status of
 *			 US variables]
 */
void USSetup(void) {
    INITPIN = 0;
    ECHOTRIS = 1;                                   ///<Input for Echo
    INITTRIS = 0;                                   ///<Output for INIT
    INTCONbits.GIE_GIEH = 1;                        ///<Turn on interrupts
    INTCONbits.PEIE_GIEL = 1;

    PIR2bits.CCP2IF = 0;
    CCP2CON = 0x00;                                 ///< Capture mode, every rising edge
    T3CONbits.RD16 = 0;                             ///< Set to 8 bit mode
    T3CONbits.T3CCP2 = 0;
    T3CONbits.T3CCP1 = 1;                           ///< Timer 3 used for CCP2
    T3CONbits.T3CKPS1 = 0;
    T3CONbits.T3CKPS0 = 0;                          ///< Prescaler is 1:1
    T3CONbits.TMR3ON = 1;

    USValues.distance = 0;                          ///< Holds current value of calc distance    
    speedOfSound = 34000;                           ///< Speed in cm/s
    distPerMs = speedOfSound/200;                   ///< Divides by 2 for travel, 100 to be mm/ms
    distPerSubMs = speedOfSound/4000;               ///< Divides by 20 times to get 20 subsections
    USFlags.fireStatus = 0;                         ///< Status of 0 means nothing is occuring, 1 means echo is incoming and has not been calculated yet
    USFlags.distanceReady = 0;                      ///< Whether the new distance has been read
    USValues.sampleSize = 2;                        ///< Starting values
    USValues.minRange = 450;
    USValues.maxRange = 2100;
    USValues.freq_ms = 80;
    USArrayPosition = 0;
}

/**
 * @brief [sets up the US Min Distance]
 */
void setUSMin (unsigned int min){
    USValues.minRange = min;
}

/**
 * @brief [sets up the US Max Distance]
 */
void setUSMax (unsigned int max){
    USValues.maxRange = max;
}

/**
 * @brief [sets up the US Samples per Average]
 */
void setUSSamplePerEstimate (unsigned char samples){
    unsigned char i = 0;
    while (i < 10) {
        UScapturedValue[i] = 0;
        i++;
    }
    if (samples <= 10 && samples > 0) {
        USValues.sampleSize = samples;
    }
}

/**
 * @brief [sets up the US fire frequency]
 * Cannot be set faster than 12Hz or less than 1Hz
 */
void setUSFrequency (unsigned char frequency){
    if (frequency > 1 && frequency < 13) {
        USValues.freq_ms = 1000/frequency;
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
 */
void testUSState (void){
    if( USFlags.fireStatus ){
        if ( TMR3H > 0x0D ) {  ///< change to 0x0D
            CCP2CON = 0x05; ///<capture mode, every rising edge
        }
    
        if (TMR3H > 0x58 ) {  ///<change to 0x58
            UScapturedValue[USArrayPosition] = 0x00;
            echoCalc();
            CCP2CON = 0x00;                         ///<turn off ccp since no value is coming
            USFlags.fireStatus = 0;                       ///<reset status
        }
    
        if ( PIR2bits.CCP2IF ) {
            PIR2bits.CCP2IF = 0;
            UScapturedValue[USArrayPosition] = ReadTimer3();    ///<store captured time into array
            echoCalc();
            CCP2CON = 0x00;
            USFlags.fireStatus = 0;
        }
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
    USFlags.fireStatus = 1;               ///<change status to be waiting for echos
    updateTime();    
    setTimeTag(USValues.freq_ms,&echoCanFire);    ///<time tag for when next echo can be sent
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
    unsigned char i;
    unsigned char voidUSData;
    unsigned int  milliTime;
    unsigned int  subMilliTime;
    unsigned int tempDistance;
    USValues.distance = 0;
    voidUSData = 0;
    if (UScapturedValue[USArrayPosition] != 0) {
        milliTime = UScapturedValue[USArrayPosition]/2500;                      ///<divides into millisec           //divide by 2500
        subMilliTime = (UScapturedValue[USArrayPosition] - milliTime*2500)/50;  ///<divides into millisec/20        //multiply by 2500
        if ((subMilliTime*50 + 25) < (UScapturedValue[USArrayPosition] - milliTime*2500)) {                         //multiply by 2500
            subMilliTime = subMilliTime + 1;
        }
        UScapturedValue[USArrayPosition] = milliTime*distPerMs + subMilliTime*distPerSubMs;   ///<gives a distance in cm (remove the /10 for mm)
    }
    i = 0;    
    while (i < USValues.sampleSize) {  ///<checks for void values
        tempDistance = UScapturedValue[i];
        if (tempDistance < USValues.minRange || tempDistance > USValues.maxRange) {       ///<remove /10 for mm
            tempDistance = 0;
            voidUSData++;
        }
        USValues.distance += tempDistance;
        i++;
    }

    if (voidUSData != USValues.sampleSize) {
        USValues.distance = USValues.distance/(USValues.sampleSize-voidUSData); ///<average distance values
    }
    USArrayPosition++;
    if (USArrayPosition = USValues.sampleSize) {
        USArrayPosition = 0; ///<reset poisition if over buffer
    }
    USFlags.distanceReady = 1;
}

