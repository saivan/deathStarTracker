/**
 * File:   IR_Sensor.c
 * Author: David Jimenez 312 069 065
 *
 * Created on 4 October 2014, 12:35 AM
 *
 * @todo go through and make sure everything is externally defined and in the header if needed
 */

#include "masterHeader.h"
#include "p18f452.h"

#define AD_READING ADRES        ///< Location that the ADC stores the converted values
#define ADCON0_SETTING 0x51     ///< Value that ADCON0 must be set to for operation
#define ADCON1_SETTING 0x0E     ///< Value that ADCON1 must be set to for operation

IRFlagType IRFlags={0,0,0};
timeTag IRSensorTimer = {0,0,0,0};  ///< TimeTag for next time IRSensor is to be called
unsigned int IRSensorDelay = 100;     ///< Delay before IRSensor can be called again in ms

unsigned char samplesPerEstimate = 10;   ///< Amount of samples averaged together to make each estimate
unsigned char sampleNumber = 0;     ///< Incrementer to see if the amount of samples needed is taken
unsigned int gatheredData [] = {0,0,0,0,0,0,0,0,0,0};   ///< Gathered readings from the GP2Y0A02YK
unsigned int reading = 0;   ///< Averaged reading across the samples taken
unsigned int calibrationVariable = 14000;   ///< constant used in distance calculations
unsigned int currentIRDistance = 0;	///< The predicted distance from the IR sensor
unsigned int i = 0;

char startCalibration [] = "Place the object 1m from sensor and press the button when ready";
char calibrationSuccessful [] = "Calibration Successful";
char targetNotFound [] = "Target not found. Please try again";
/**
 * IRSensor() handles the data harvesting and distance calculations from the GP2Y0A02YK sensor
 *
 * The function does 1 of 2 things based on what state is in (the value of IRState)
 *
 * This takes denotes the 2 operating modes:
 *  GATHERING_DATA          - Do nothing if Temp Sensor is busy
 *                          - Change the AD Conversion Channel to that of the
 *                              IR sensor
 *                          - reset the IR flags
 *                          - If data is not ready to be collected: do nothing
 *                          - Store data into gatheredData[]
 *                          - If more data is needed, start AD conversion again
 *                          - If no more data is needed, set IRState to CALCULATING_DISTANCE
 * CALCULATING_DISTANCE     - Find average of gathered data
 *                          - Calculate distance based on data and store in currentIRDistance
 *                          - Set IRState to GATHERING_DATA
 *
 *
 * @todo reconsider use of flags. IRState could become a flag
 */
#pragma code
void IRSensor( void ){

    if (~eventDue(&IRSensorTimer)){
        return;
    }
    if ( IRFlags.IRState == GATHERING_DATA ){
        if( 0 /* TEMP SENSOR IS BUSY */ ){
                return;
        }
        setTimeTag(IRSensorDelay, &IRSensorTimer);  ///< Set time that IRSensor is next called
        
        if (PIR1bits.ADIF){///< If there is data to be harvested
            gatheredData[sampleNumber] = (AD_READING >> 6); ///< Store gathered data in memory
            sampleNumber++;     ///< increment new data counter
            if (sampleNumber == samplesPerEstimate){///< If we have enough data
                sampleNumber = 0;
                IRFlags.IRState = CALCULATING_DISTANCE;     ///< Move to calculating distance
                return;
            }
        }

        /// Set up AD conversion settings
        ADCON0 = ADCON0_SETTING;
        ADCON1 = ADCON1_SETTING;
        ADCON0bits.GO = 1;  ///< If more data is needed, start AD conversion again
        return;
    } else {    ///< IRState is CALCULATING_DISTANCE
        IRFlags.distanceReady = 1;
        reading = gatheredData[0];

        ///Calculate average reading
        if (samplesPerEstimate == 2){
            reading += gatheredData[1];
            reading = reading >> 1;
        } else if (samplesPerEstimate == 4){
            reading += gatheredData[1];
            reading += gatheredData[2];
            reading += gatheredData[3];
            reading = reading >> 2;
        } else {
            i=1;
            while (i<samplesPerEstimate){

                reading += gatheredData[i];
                i++;
            }
            reading = reading/samplesPerEstimate;
        }

        if (reading < 50){      ///< If object is not found
            currentIRDistance = 0;
        } else {
            currentIRDistance = 10*((calibrationVariable)/(reading));
            if (currentIRDistance > 1500 || currentIRDistance < 500){ ///@todo change to range max and min
                currentIRDistance = 0;
            }
        }

        IRFlags.IRState = GATHERING_DATA;
        return;
    }
}

/**
 * calibrateIR() allows the user to calibrate the IR module to work off a calculated value.
 *
 * This is set by having the user place the object being tracked at 1m distance, and press a button.
 *
 * This will change the value of calibrationVariable which is used in distance calculation.
 *
 */
void calibrateIR(void){
    ///@Kelvin print startCalibration
    IRFlags.calibrationInProgress = 0;

    while(IRFlags.calibrationInProgress){  ///< While calibration is not over

        while (0 /*button is not pressed*/){    //stall
        }
        
        /// Set up AD conversion settings
        ADCON0 = ADCON0_SETTING;
        ADCON1 = ADCON1_SETTING;
        ADCON0bits.GO = 1;  ///< Start AD conversion

        while(~PIR1bits.ADIF);  ///< Wait until AD conversion is complete

        calibrationVariable = (AD_READING >> 6) * 1000;  ///< Calculate calibration variable

        if (calibrationVariable > 500){     ///< If the calculated variable is valid
            IRFlags.calibrationInProgress = 1;  ///< Calibration is over
            ///@Kelvin print calibrationSucessful
            //could tell the user that calibration was successful with a pause
            
        } else {
            ///@Kelvin print targetNotFound

        }
    }
    return;
}

/**
 * setIRSamplesPerEstimate() allows the user to choose the amount of samples that
 * will be taken before an estimate of the distance will be calculated.
 *
 * Allowable values are from 1-4.
 *
 * @todo See if this function should handle input and output. Maybe return 0 for success
 *
 * @param IRSamplesPerEstimate
 */
void setIRSamplesPerEstimate(unsigned char IRSamplesPerEstimate){
    if (IRSamplesPerEstimate > 4 || IRSamplesPerEstimate < 1){
        //print TOO MANY SAMPLES etc.
        return;
    }
    samplesPerEstimate = IRSamplesPerEstimate;
    
    ///Reset IR sampling state
    IRFlags.IRState = GATHERING_DATA;
    sampleNumber = 0;
}

/**
 * setIRSampleRate() allows the user to choose the rate at which the IR sensor is sampled.
 *
 * Sample rate is set as whole ms delay chunks. As a result, the allowable frequencies are
 * non-continuous, ie 1ms delay -> 1kHz, 2ms delay -> 500Hz etc.
 *
 * Note: A given sample rate > 1000 will be executed as fast as the machine is able to.
 *
 * @todo consider telling the user about the limits of this program
 * @todo consider telling the user when they input invalid values
 * @todo consider having the function return values to reflect the above 2 points
 *
 * @param IRSampleRate
 */
void setIRSampleRate(unsigned int IRSampleRate){
    
    if (IRSampleRate < 1){
        //possibly return that the sample rate must be positive
        return;
    }

    IRSensorDelay = 1000/IRSampleRate;  ///< Set sample rate;
}