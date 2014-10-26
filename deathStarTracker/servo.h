/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

 ///// DECLARE STUFF IN INTERRUPTS VOLATILEEEEEE

#ifndef SERVOS_H
#define	SERVOS_H

#define AZIMUTH_PIN PORTCbits.RC4
#define ELEVATION_PIN PORTCbits.RC5
#define DDR_AZIMUTH TRISCbits.RC4
#define DDR_ELEVATION TRISCbits.RC5

// Don't forget to put in your clock frequency!!!
#define CLOCK_FREQ 10000000
#define SERVO_CCP_PERIOD (CLOCK_FREQ/200)
#define SERVO_CCP_MAX (CLOCK_FREQ/2000)
#define SERVO_CCP_MIN (CLOCK_FREQ/4000)
// Finding the Mapping gradient and Intercept
#define SERVO_MAPPING_GRAD (SERVO_CCP_MIN/180)
#define SERVO_MAPPING_YINT (SERVO_CCP_MIN) + 4 ///< We add four to correct the servo frequency


// Flags to help us keep track of when to turn each servo on
typedef struct ServoFlags{
    unsigned char azimuthFired : 1;
    unsigned char elevationFired : 1;
} ServoFlags;
extern volatile ServoFlags servoFlags;

// The defined on and off times for the servo
extern unsigned int servoOffTime;
extern unsigned int elevationServoOnTime;
extern unsigned int azimuthServoOnTime;

// Set up the angular limits
extern unsigned char azimuthMin;
extern unsigned char azimuthMax;
extern unsigned char elevationMin;
extern unsigned char elevationMax;



void updateCCPServoAngle( unsigned char azimuthAngle, unsigned char elevationAngle );
void setupServos( void );

#endif	/* SERVOS_H */

