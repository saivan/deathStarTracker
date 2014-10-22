

#include "servo.h"

// Flags to help us keep track of when to turn each servo on
struct servoFlags{
    unsigned char azimuthFired : 1;
    unsigned char elevationFired : 1;
} servoFlags;


#pragma interrupt highPriorityISR
void highPriorityISR( void ){
    if( PIR1bits.CCP1IF ){
        PIR1bits.CCP1IF = 0;                    // Clear the flag for the next interrupt
        if( ~servoFlags.azimuthFired ){
            AZIMUTH_PIN = 1;                    // Turn on the azimuth servo
            CCPR1 = azimuthServoOnTime;         // Set CCP1 to the Azimuth on time
            servoFlags.azimuthFired = 1;        // Note that the azimuth servo was triggered
        } else if ( ~servoFlags.elevationFired ){
            AZIMUTH_PIN = 0;                    // Turn off the Azimuth servo
            ELEVATION_PIN = 1;                  // Turn on the elevation servo
            CCPR1 = elevationServoOnTime;       // Set CCP1 to the Elevation on time
            servoFlags.elevationFired = 1;      // Note that the elevation servo was triggered
        } else {
            ELEVATION_PIN = 0;                  // Turn off the Elevation servo
            CCPR1 = servoOffTime;               // Move the off time to the CCP1
            servoFlags.azimuthFired = 0;        // Restart the azimuth cycle
            servoFlags.elevationFired = 0;      // Restart the elevation cycle
        }
    }
}

#pragma code highPriorityInterruptAddress=0x08
void high_interrupt( void ){
    if( PIR1bits.CCP1IF )
        WriteTimer1(0);                             // Reset the Timer    
    _asm GOTO highPriorityISR _endasm
}



void main( void ){

    TRISB = 0;

    setupServos();
    ADCON0bits.ADON = 1;    // Turn on analog to digital converter
//    ADCON2bits.ADFM = 1;    // Right Justify the results
    while(1){

        // Setup Analog to digital conversion
        ADCON0bits.GO = 1;  // Start analog to digital conversion
        while(ADCON0bits.GO){/*Do Nothing*/}
        updateServoCCP(ADRESH-12,ADRESH-6);
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

