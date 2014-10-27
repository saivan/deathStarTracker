
#include "masterHeader.h"

/// Declaring the servo flags for opperation
volatile ServoFlags servoFlags = { 0, 0 };

// Defining the servos current positions and parameters
unsigned int currentAzimuth = 0;
unsigned int currentElevation = 0;

// The defined on and off times for the servo
unsigned int servoOffTime;
unsigned int elevationServoOnTime;
unsigned int azimuthServoOnTime;

//// Set up the angular limits - stored as tenths of degrees
unsigned int azimuthMin = 0;
unsigned int azimuthMax = 1800;
unsigned int elevationMin = 0;
unsigned int elevationMax = 1800;


/**
 * @brief Setup the servos to start moving when powered
 * Given a particular azimuth and elevation, the servos will start continuously
 * moving towards the given azimuth and given elevation.  
 */
void setupServos( void ){
    // Set up the correct data directions
    DDR_AZIMUTH = 0;
    DDR_ELEVATION = 0;
    // Set up timer 1 : Prescaler 1 for max resolution
    WriteTimer1(0);                           // Clear Timer 1
    T1CONbits.RD16 = 1;                 // Latch in 16 bit values for CCPR1L and CCPR1H
    T1CONbits.TMR1ON = 1;               // Turn on timer 1
    // Setup the initial Servo position
    updateCCPServoAngle( 1800, 1800 );
    // Setup the capture Compare module
    CCPR1 = servoOffTime;               // Write the initial delay
    CCP1CONbits.CCP1M = 0b1010;         // Setup the compare interrupt
    // Enable the CCP interrupt
    RCONbits.IPEN = 1;                  // Enable interrupt priorites
    IPR1bits.CCP1IP = 1;                // Set CCP interrupt to high priority
    PIE1bits.CCP1IE = 1;                // Enable the capture compare interrupt
    INTCONbits.GIEH = 1;                // Enable high priority interrupts
}


/**
 * @brief Calculates the CCP values to reach a particular azimuth and elevation
 * Uses the inputs to calculate the desired azimuth and elevation on times for the CCP module
 * to use during it's execution. 
 * 
 * @param azimuthAngle [0-1800] The desired azimuth angle in tenths of degrees from the origin
 * @param elevationAngle [0-1800] The desired elevation angle in tenths of degrees from the origin
 */
void updateCCPServoAngle( unsigned int azimuthAngle, unsigned int elevationAngle ){
    /// Stop any entries greater than the defined limits
    if ( ( azimuthAngle > azimuthMax ) || ( elevationAngle > elevationMax ) ||
            ( azimuthAngle < azimuthMin) || ( elevationAngle < elevationMin) ){
        return;
    }
    /// Storing the new azimuth and elevation
    currentAzimuth = azimuthAngle;
    currentElevation = elevationAngle;   
    /// Calculate the CCPTime by using $ \text{time} = {2000\over 1800}\theta + 2000 $
    // azimuthServoOnTime = ((11*azimuthAngle)>>3) + SERVO_AZIMUTH_YINT;
    // elevationServoOnTime = ((11*elevationAngle)>>3) + SERVO_ELEVATION_YINT;
    azimuthServoOnTime = 5000;
    elevationServoOnTime = 5000;

    // Calculate the servos off time by subtracting
    servoOffTime = SERVO_CCP_PERIOD - azimuthServoOnTime - elevationServoOnTime;
}



