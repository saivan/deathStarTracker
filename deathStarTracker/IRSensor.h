/* 
 * File:   IR_Sensor.h
 * Author: David
 *
 * Created on 11 October 2014, 10:39 PM
 */

#ifndef IR_SENSOR_H
#define	IR_SENSOR_H

#define AD_READING ADRES        ///< Location that the ADC stores the converted values
#define ADCON0_SETTING 0x09     ///< Value that ADCON0 must be set to for operation
#define ADCON1_SETTING 0x0E     ///< Value that ADCON1 must be set to for operation (might be 0C)
#define ADCON2_SETTING 0x81

void IRSensor(void);
void calibrateIR(void);
void setIRSamplesPerEstimate(unsigned char IRSamplesPerEstimate);
void setIRSampleRate(unsigned int IRSampleRate);
void getNewIRDistance(void);

enum IR_STATES {
	GATHERING_DATA,
	CALCULATING_DISTANCE,	
};

typedef struct IRFlagType{
	unsigned char IRState: 1;							///< Current state of the IRSensor() method
	unsigned char distanceReady : 1;					///< The distance is ready to read
        unsigned char calibrationInProgress: 1;         ///< IRCalibration was completed correctly.
        unsigned char idle : 1;
}IRFlagType;

extern IRFlagType IRFlags;
extern unsigned int IRDistance;			///< The predicted distance from the IR sensor
extern unsigned char IRState;
extern unsigned int gatheredData[10];              ///< Stores the data gathered



#endif	/* IR_SENSOR_H */