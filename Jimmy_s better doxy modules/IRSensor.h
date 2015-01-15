/* 
 * File:   IRSensor.h
 * Author: David
 *
 * This file encompasses the declaration and heading of the infrared module.
 * The infrared module utilises a GP2Y0A02YK sensor attached to pin AN2 of the
 * pic18f4520 board in order to create estimates of the distance to the nearest
 * target.
 *
 * Created on 11 October 2014, 10:39 PM
 */

#ifndef IR_SENSOR_H
#define	IR_SENSOR_H

#define AD_READING ADRES  ///< Location that the ADC stores the converted values
#define ADCON0_SETTING 0x09  ///< AN2, ADC module enabled
#define ADCON1_SETTING 0x0E  ///< Standard voltage reference, AN2 analog in
#define ADCON2_SETTING 0x81  ///< Right justified, Fosc/8

///Function declarations
extern void IRSensor(void);
extern void setIRSamplesPerEstimate(unsigned char IRSamplesPerEstimate);
extern void setIRSampleRate(unsigned int IRSampleRate);
extern void getNewIRDistance(void);
extern void setIRMin(unsigned int min);
extern unsigned int getIRVariance(void);
extern unsigned int getIRRaw(void);

///States that the flag IRState can take
enum IR_STATES {
	GATHERING_DATA,
	CALCULATING_DISTANCE,	
};

///Container for the flags that relate to the IR module
typedef struct IRFlagType{
	unsigned char IRState: 1;   ///< Current state of the IRSensor() method
	unsigned char distanceReady : 1;    ///< The distance is ready to read
        unsigned char idle : 1;
}IRFlagType;

///Externally defined variables
extern IRFlagType IRFlags;  ///< IR Flag variable
extern unsigned int IRDistance;  ///< The calculated distance from the IR sensor
extern unsigned int gatheredData[10];   ///< Stores the data gathered
extern unsigned int avgIRReading;///< latest calculated average reading from ADC

#endif	/* IR_SENSOR_H */