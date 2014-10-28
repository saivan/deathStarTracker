/*
 * File:   tempSensor.h
 * Author: Kye Ridley-Smith
 *
 * Created on 26 October 2014
 */

#include <p18f452.h>
#include "IR_Sensor.h"  ///< Must include this in order to check the infrared state
#include "realTimer.h"
#include <delays.h>
#include "LCD.h"

#ifndef TEMPSENSOR_H
#define	TEMPSENSOR_H

#define TRUE    1
#define FALSE   0



///< Function declarations
void tempSensor(void);
void calibrateTempSensor(void);
void clearCalibationOffset(void);
void tempSensorInitialisation(void);

///< Set the 2 states to be checked when the function is called. numbered sequentially 0 and 1
enum AD_STATES {
	ADCSETUP,
	CALCULATE_TEMPERATURE,
};

///< Define variables
extern int temperatureReading;                         ///< Initialise the ADC value to 0
extern int temperaturemV;                              ///< Initialie the temperature voltage to 0
extern int temperatureDegC;                            ///< Set the temperature initially to a standard room temp value of 25
extern int temperatureOffset;                          ///< set the temperature offset to 0
extern int tempSensorDelay;                            ///< Set the temperature to only update every 1second
extern int calibrationTemperature;                     ///< This is a temperature which the user will input to calibrate our own temperature


#endif //< Need this as have a ifndef




