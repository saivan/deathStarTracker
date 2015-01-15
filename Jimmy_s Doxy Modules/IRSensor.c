/*
 * File:   IR_Sensor.c
 * Author: David Jimenez 312 069 065
 *
 * Created on 4 October 2014, 12:35 AM
 *
 */

#include "masterHeader.h"


IRFlagType IRFlags={0,0,0};         ///< Flags for the IR module
/// TimeTag for next time IRSensor is to be called
timeTag IRSensorTimer = {0,0,0,0};
/// Delay before IRSensor can be called again in ms
unsigned int IRSensorDelay = 0;     

/// Amount of samples averaged together to make each estimate
unsigned char samplesPerEstimate = 10;
/// Incrementer to see if the amount of samples needed is taken
unsigned char sampleNumber = 0;
/// Gathered readings from the GP2Y0A02YK
unsigned int gatheredData [] = {0,0,0,0,0,0,0,0,0,0};
/// Averaged reading across the samples taken
unsigned int avgIRReading = 0;
/// Constant used in distance calculations
unsigned int calibrationVariable = 14000;   
unsigned int IRDistance = 0;	///< The predicted distance from the IR sensor
unsigned int maxRange = 2000;
unsigned int minRange = 500;

/**
 * @brief Handles the data harvesting and distance calculations from the
 *        GP2Y0A02YK sensor
 *
 * @details The function does 1 of 2 things based on what state is in
 *          (the value of IRState)
 *
 * This takes denotes the 2 operating modes:
 *  GATHERING_DATA          - Do nothing if Temp Sensor is busy
 *                          - Change the AD Conversion Channel to that of the
 *                              IR sensor
 *                          - reset the IR flags
 *                          - If data is not ready to be collected: do nothing
 *                          - Store data into gatheredData[]
 *                          - If more data is needed, start AD conversion again
 *                          - If no more data is needed, set IRState to
 *                            CALCULATING_DISTANCE
 * CALCULATING_DISTANCE     - Find average of gathered data
 *                          - Calculate distance based on data and store in
 *                            IRDistance
 *                          - Set IRState to GATHERING_DATA
 *
 * If the distance calculated by this module is deemed invalid in any way,
 * IRDistance is set to a "null" value of 0 to rerpresent this fact.
 */
#pragma code
void IRSensor( void ){

    unsigned char i=0;

    /// If the IR sensor is supposed to be idle, do nothing
    if (IRFlags.idle){
        return;
    }
    /// Only do conversions at the desired frequency
    if (!eventDue(&IRSensorTimer)){
        return;
    }

    if ( IRFlags.IRState == GATHERING_DATA ){
        if( tempFlags.tempConverting ){ ///< do nothing if ADC in use
            return;
        }
        /// Set time that IRSensor is next called
        setTimeTag(IRSensorDelay, &IRSensorTimer);
        
        if (PIR1bits.ADIF){///< If there is data to be harvested
            /// Store gathered data in memory
            gatheredData[sampleNumber] = (AD_READING); 
            sampleNumber++;     ///< increment new data counter
            if (sampleNumber == samplesPerEstimate){ ///< If we have enough data
                sampleNumber = 0;
                /// Move to calculating distance
                IRFlags.IRState = CALCULATING_DISTANCE;
                return;
            }
        }

        /// Set up AD conversion settings
        ADCON0 = ADCON0_SETTING;
        ADCON1 = ADCON1_SETTING;
        ADCON2 = ADCON2_SETTING;
        ADCON0bits.GO = 1;///< If more data is needed, start AD conversion again
        return;
    } else {    ///< IRState is CALCULATING_DISTANCE
        IRFlags.distanceReady = 1;
        avgIRReading = gatheredData[0];

        ///Calculate average reading
        i=1;
        while (i<samplesPerEstimate){
            avgIRReading += gatheredData[i];
            i++;
        }
        avgIRReading = avgIRReading/samplesPerEstimate;
        

        if (avgIRReading < 50){      ///< If object is not found
            IRDistance = 0;         ///< return a null value
        } else {
            ///multiplication by 10 here is necessary to convert from cm to mm
            IRDistance = 10*((calibrationVariable)/(avgIRReading));
            if (IRDistance > maxRange || IRDistance < minRange){
                IRDistance = 0; ///< return null if object is out of range
            } else {
                trackFlags.targetFound = 1; ///< Declare target found
                ///report last known location
                lastKnownAzimuth = currentAzimuth;
                lastKnownElevation = currentElevation;
            }

        }


        IRFlags.IRState = GATHERING_DATA;   ///< Calculation is complete
        IRFlags.idle = 1;       ///< IR module is back to idle mode
        return;
    }
}


/**
 * @brief Allows the user to choose the amount of samples that
 * will be taken before an estimate of the distance will be calculated.
 *
 * The IRSamplesPerEstimate must be checked for invalid values in the calling
 * function. If this is not done, ie the value is > 10 or < 1, a default value
 * of 5 is used instead.
 *
 * @param IRSamplesPerEstimate, the desired amount of samples per estimate
 */
void setIRSamplesPerEstimate(unsigned char IRSamplesPerEstimate){
    if (IRSamplesPerEstimate > 10 || IRSamplesPerEstimate < 1){
        samplesPerEstimate = 5;
        return;
    }
    samplesPerEstimate = IRSamplesPerEstimate;  ///< Set new sample rate
    
    ///Reset IR sampling state
    IRFlags.IRState = GATHERING_DATA;
    sampleNumber = 0;
}

/**
 * @brief Allows the user to choose the rate at which the IR sensor is sampled.
 *
 * @details Sample rate is set as whole ms delay chunks. As a result, the 
 *          allowable frequencies are non-continuous, ie 1ms delay -> 1kHz, 2ms
 *          delay -> 500Hz etc.
 *
 * Note: A given sample rate > 1000 will be executed as fast as the program can.
 *
 * @param IRSampleRate, the desired sample rate in Hz
 */
void setIRSampleRate(unsigned int IRSampleRate){
    
    if (IRSampleRate < 1){  ///< Negative or zero sample rates are not valid
        return;
    }

    IRSensorDelay = 1000/IRSampleRate;  ///< Set sample rate
}

/**
 * @brief Sets the minimum range for the IR sensor
 * 
 * @details Calculated ranges below this range will be interpreted as invalid
 *          readings (equated to 0)
 *
 * @param min, the desired minimum range in mm
 */
void setIRMin(unsigned int min){
    minRange = min;
}

/**
 * @brief Sets the maximum range for the IR sensor
 *
 * @details Calculated ranges above this range will be interpreted as invalid
 *          readings (equated to 0)
 * @param max, the desired maximum range in mm
 */
void setIRMax(unsigned int max){
    maxRange = max;
}

/**
 * @brief returns the raw data used to estimate distances
 *
 * @details The value returned is the first value detected in each sampling
 *          instance, read off the ADC
 *
 * @return gatheredData[0], The first reading taken from the ADC since the
 *         data pointer was reset
 */
unsigned int getIRRaw(void){
    return gatheredData[0];
}

/**
 * @brief Gives the variance in the IR sensor readings
 *
 * @details finds the largest difference between each reading and the mean,
 *          and then returns this value.
 * @return maxVariance, greatest difference between a reading and the mean
 */
unsigned int getIRVariance(void){
    unsigned char i = 0;
    unsigned int maxVariance = 0;
    unsigned int variance = 0;

    while (i < samplesPerEstimate){
        ///find the absolute difference between avgIRValue and gatheredData[i]
        variance = ((avgIRReading-gatheredData[i])<<1)>>1;
        if ( variance > maxVariance){
            maxVariance = variance;
        }
    }
    return maxVariance;
}

/**
 * @brief Initialises a new estimate of distance given by the IR sensor
 *
 * @details This function reinitialises variables that need to be changed
 *          whenever a new estimate is required from the IR module, and begins
 *          the first analog to digital conversion or a sample set.
 */
void getNewIRDistance(void){

    IRFlags.distanceReady = 0;  ///< New distance reading is not yet calculated

    /// Set the state of IRSensor to data harvesting mode
    IRFlags.IRState = GATHERING_DATA;

    IRFlags.idle = 0;
    sampleNumber = 0;
    ADCON0 = ADCON0_SETTING;
    ADCON1 = ADCON1_SETTING;
    ADCON2 = ADCON2_SETTING;
    ADCON0bits.GO = 1;
}