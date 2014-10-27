/*
 * File:   tempSensor.h
 * Author: Kye Ridley-Smith
 *
 * Created on 26 October 2014
 */

#include <p18f452.h>

#ifndef TEMPSENSOR_H
#define	TEMPSENSOR_H

///< Make some defintions
#define     TRUE 1
#define     FALSE 0
#define     HIGH            1
#define     LOW             0


void tempSensor(void);
void calibrateTempSensor(void);

enum AD_STATES {
	ADCSETUP,
	CALCULATE_TEMPERATURE,
};

///< Extern these variable so it can be used by the intToDisplay function / just in case. Seems to wrok without
extern int temperaturemV;
extern int temperatureDegC;
extern unsigned char tempState;

#endif //< Need this as have a ifndef

//void tempSensor(void);
//void calibrateTempSensor(void);
//
//extern unsigned char temperature;
//
////< enum is like a #define but whatever your write below it is sequentially numbered from 0 to 1
//enum tempSensor_STATES {
//	GATHERING_DATA,
//	CALCULATING_TEMPERATURE,
//};
//
//typedef struct TEMPFlagsType{
//	unsigned char TEMPState: 1;		///< Current state of the IRSensor() method
//	unsigned char temperatureReady : 1;		///< The distance is ready to read
//        unsigned char tempCalibrationInProgress: 1;         ///< IRCalibration was completed correctly. Jimmy should change this to IRCalibration etc
//}TEMPFlagsType;
//
//extern TEMPFlagType TEMPFlags;
//extern unsigned int currentTEMP;			///< The predicted distance from the IR sensor
//extern unsigned char TEMPState;


