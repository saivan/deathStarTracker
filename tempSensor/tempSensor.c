/*
 * File:   tempSensor.c
 * Author: Kye Ridley-Smith
 *
 * Created on 26 October 2014
 */

#include "tempSensor.h"
#include "IR_Sensor.h"  ///< Must include this in order to check the infrared state
#include "realTimer.h"

#define     SUPPLY_VOLTAGE      500                 ///< This can be varied depending on the supply mV we get
#define     AD_READING          ADRES               ///< Location that the ADC stores the converted values
#define     ADCON0_AD_SETTING   0b01001001          ///< Fosc/8, A/D enable, RA1
#define     ADCON1_AD_SETTING   0b10001110          ///< Right justify (so we can use 10 bit res. only left justify when wiping off 2 LSB), 1 analog channel
#define     OUTPUT              0
#define     INPUT               1
#define     TEMPERATURE_PIN     PORTAbits.RA1       ///< RA0 is attached to the POT so use RA1
#define     TEMPERATURE_DDR     TRISAbits.RA1       ///< DDR stands for Data Direction Register

///< Define variables
int temperatureReading = 0;                         ///< Initialise the ADC value to 0
int temperaturemV = 0;                              ///< Initialie the temperature voltage to 0
int temperatureDegC = 25;                           ///< Set the temperature initially to a standard room temp value of 25
int temperatureOffset = 0;                          ///< set the temperature offset to 0
int tempSensorDelay = 1000;                         ///< Set the temperature to only update every 1second
int calibrationTemperature = 0;                     ///< This is a temperature which the user will input to calibrate our own temperature

timeTag tempSensorTimer = {0,0,0,0};                ///< TimeTag for next time tempSensor is to be called

typedef struct tempFlags{
	unsigned char tempState: 1;                 ///< Current state of the temp Sensor
}tempFlags;

void tempSensor(void){
///< THIS SHOUL BE DONE ELSEWHERE AND LEFT PERMANENTLY
    TEMPERATURE_PIN = 0x00;                         ///< Clear RA0
    TEMPERATURE_DDR = INPUT;                        ///< Set RA0 as input

    if(tempFlags.tempState == ADCSETUP){            ///< If state is set to setup ADC, go here
///< I WASN'T QUITE SURE HOW TO DO THE TIMER TAGS THING
        if ((~eventDue(&tempSensorTimer))||(IRFlags.IRState == GATHERING_DATA)){    ///< If an event is not due OR the infrared sensr is busy, return from function
        return;
        }

        else{
        ADCON0 = 0b01001001;                            ///< Fosc/8, A/D enable, RA1
        ADCON1 = 0b10001110;                            ///< Right justify (so we can use 10 bit res. only left justify when wiping off 2 LSB), 1 analog channel
        ADCON0bits.GO=1;                                ///< Stard ADC
        tempFlags.tempState == CALCULATE_TEMPERATURE;   ///< Set the state to go into calculate temp next time the function is called
        return;
        }
    }

    else if(tempFlags.tempState == CALCULATE_TEMPERATURE){              ///< if the state is set to calculate temp, go here
        if (PIR1bits.ADIF){                                             ///< if conversion flag is set, there is data to be harvested
            temperatureReading = AD_READING;                            ///< Store our ADC value in ADRESH (wiping off the 2 bits stored in ADRESH as we can get away with 8bit resolution)
            temperaturemV = (int)(SUPPLY_VOLTAGE*temperatureReading);   ///< make the value an int so it can be used with the inToDisplay function. 500 is our max mV based on supply Voltage
            temperatureDegC = temperaturemV >> 10;                      ///< Bit shift by 10 to divide by 1024 to give the temp to the nearest DegC
            temperatureDegC = temperatureDegC + temperatureOffset;                 ///< implement the user calibrated offset (initially 0)
            PIR1bits.ADIF = 0;                                          ///< Clear the ADC flag
            setTimeTag(tempSensorDelay, &tempSensorTimer);              ///< Set the next time for the ADC to run
            tempFlags.tempState == ADCSETUP;                            ///< set the state so that next time the function runs it will go to ADC setup
        }

        else{}                                          ///< If conversion not done, exit the function

    }
}

///< Function to determine a temp offset so our temp matches user input temp
void calibrateTempSensor(void){
    temperatureOffset = calibrationTemperature - temperaturemV;
}

///< function which allows for the temperature offset to be easily cleared
void clearCalibrationOffset(void){
    temperatureOffset = 0;
}