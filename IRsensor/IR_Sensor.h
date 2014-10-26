/* 
 * File:   IR_Sensor.h
 * Author: David
 *
 * Created on 11 October 2014, 10:39 PM
 */

#ifndef IR_SENSOR_H
#define	IR_SENSOR_H

void IRSensor(void);
void calibrateIR(void);
void setIRSamplesPerEstimate(unsigned char IRSamplesPerEstimate);
void setIRSampleRate(unsigned int IRSampleRate);


enum IR_STATES {
	GATHERING_DATA,
	CALCULATING_DISTANCE,	
};

typedef struct IRFlagType{
	unsigned char IRState: 1;		///< Current state of the IRSensor() method
	unsigned char distanceReady : 1;		///< The distance is ready to read
        unsigned char calibrationInProgress: 1;         ///< IRCalibration was completed correctly.
}IRFlagType;

extern IRFlagType IRFlags;
extern unsigned int currentIRDistance;			///< The predicted distance from the IR sensor
extern unsigned char IRState;
extern unsigned int gatheredData[10];              ///< Stores the data gathered



#endif	/* IR_SENSOR_H */