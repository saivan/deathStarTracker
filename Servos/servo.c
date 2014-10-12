
#include "servo.h"


// The defined on and off times for the servo
unsigned int servoOffTime;
unsigned int elevationServoOnTime;
unsigned int azimuthServoOnTime;

// Set up the angular limits
unsigned char azimuthMin = 0;
unsigned char azimuthMax = 180;
unsigned char elevationMin = 0;
unsigned char elevationMax = 180;

void setupServos( void ){
    // Set up the correct data directions
    DDR_AZIMUTH = 0;
    DDR_ELEVATION = 0;
    // Set up timer 1 : Prescaler 1 for max resolution
    WriteTimer1(0);                           // Clear Timer 1
    T1CONbits.RD16 = 1;                 // Latch in 16 bit values for CCPR1L and CCPR1H
    T1CONbits.TMR1ON = 1;               // Turn on timer 1
    // Setup the initial Servo position
    updateServoCCP( 90, 90 );
    // Setup the capture Compare module
    CCPR1 = servoOffTime;               // Write the initial delay
    CCP1CONbits.CCP1M = 0b1010;         // Setup the compare interrupt
    // Enable the CCP interrupt
    RCONbits.IPEN = 1;                  // Enable interrupt priorites
    IPR1bits.CCP1IP = 1;                // Set CCP interrupt to high priority
    PIE1bits.CCP1IE = 1;                // Enable the capture compare interrupt
    INTCONbits.GIEH = 1;                // Enable high priority interrupts
}


// Calculate the CCP times for the servo controls
void updateServoCCP( unsigned char azimuthAngle, unsigned char elevationAngle ){
    // Stop any entries greater than the defined limits
    if ( ( azimuthAngle > azimuthMax ) || ( elevationAngle > elevationMax ) ||
            ( azimuthAngle < azimuthMin) || ( elevationAngle < elevationMin) ){
        return;
    }
    // Calculate the CCPTime by multiplying the angle
    azimuthServoOnTime = SERVO_MAPPING_GRAD*azimuthAngle + SERVO_MAPPING_YINT;
    elevationServoOnTime = SERVO_MAPPING_GRAD*elevationAngle + SERVO_MAPPING_YINT;
    // Calculate the servos off time by subtracting
    servoOffTime = SERVO_CCP_PERIOD - azimuthServoOnTime - elevationServoOnTime;
}


