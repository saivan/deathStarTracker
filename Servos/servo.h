/* 
 * File:   servos.h
 * Author: saivanhamama
 *
 * Created on October 12, 2014, 4:59 PM
 */

#ifndef SERVOS_H
#define	SERVOS_H

#include <p18f452.h>
#include <timers.h>

#define AZIMUTH_PIN PORTCbits.RC4
#define ELEVATION_PIN PORTCbits.RC5
#define DDR_AZIMUTH TRISCbits.RC4
#define DDR_ELEVATION TRISCbits.RC5

// Don't forget to put in your clock frequency!!!
#define CLOCK_FREQ 4000000
#define SERVO_CCP_PERIOD (CLOCK_FREQ/200)
#define SERVO_CCP_MAX (CLOCK_FREQ/2000)
#define SERVO_CCP_MIN (CLOCK_FREQ/4000)
// Finding the Mapping gradient and Intercept
#define SERVO_MAPPING_GRAD (SERVO_CCP_MIN/180)
#define SERVO_MAPPING_YINT (SERVO_CCP_MIN)

// The defined on and off times for the servo
extern unsigned int servoOffTime;
extern unsigned int elevationServoOnTime;
extern unsigned int azimuthServoOnTime;

// Set up the angular limits
extern unsigned char azimuthMin;
extern unsigned char azimuthMax;
extern unsigned char elevationMin;
extern unsigned char elevationMax;



void updateServoCCP( unsigned char azimuthAngle, unsigned char elevationAngle );
void setupServos( void );

#endif	/* SERVOS_H */

