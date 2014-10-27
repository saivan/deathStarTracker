#include "tempSensor.h"
#include "LCD.h"
#include <delays.h>
#include "IR_Sensor.h"  ///< Must include this in order to check the infrared state
#include "realTimer.h"

#define     SUPPLY_VOLTAGE      500                 ///< This can be varied depending on the supply mV we get
#define     AD_READING          ADRES               ///< Location that the ADC stores the converted values
#define     ADCON0_AD_SETTING   0b01001001          ///< Fosc/8, A/D enable, RA1
#define     ADCON1_AD_SETTING   0b10001110          ///< Right justify (so we can use 10 bit res. only left justify when wiping off 2 LSB), 1 analog channel
#define     OUTPUT              0
#define     INPUT               1
#define     TEMPERATURE_PIN     PORTAbits.RA1
#define     TEMPERATURE_DDR     TRISAbits.RA1       ///< DDR stands for Data Direction Register

///< Define variables
int temperatureReading = 0;
int temperaturemV = 0;                               
int temperatureDegC = 25;
int offset = 0;
int tempSensorDelay = 1000;        ///< Set the temperature to only update every 1second
int calibrationTemperature = 0;

timeTag tempSensorTimer = {0,0,0,0};  ///< TimeTag for next time tempSensor is to be called

typedef struct tempFlags{
	unsigned char tempState: 1;		///< Current state of the temp Sensor()
	unsigned char temeratureReady : 1;		///< The distance is ready to read
        unsigned char calibrationComplete: 1;         ///< temp Calibration was completed correctly.
}tempFlags;

void tempSensor(void){
///< THIS SHOUL BE DONE ELSEWHERE AND LEFT PERMANENTLY
    TEMPERATURE_PIN = 0x00;                         ///< Clear RA0
    TEMPERATURE_DDR = INPUT;                        ///< Set RA0 as input

    ///< If an event is not due OR the infrared sensr is busy, return from function
///< I WASN'T QUITE SURE HOW TO DO THE TIMER TAGS THING

    
    if(tempFlags.tempState == ADCSETUP){
        if ((~eventDue(&tempSensorTimer))||(IRFlags.IRState == GATHERING_DATA)){
        return;
        }
        else{
        ADCON0 = 0b01001001;                            ///< Fosc/8, A/D enable, RA1
        ADCON1 = 0b10001110;                            ///< Right justify (so we can use 10 bit res. only left justify when wiping off 2 LSB), 1 analog channel
        ADCON0bits.GO=1;
        tempFlags.tempState == CALCULATE_TEMPERATURE;
        return;
        }
    }
    
    else if(tempFlags.tempState == CALCULATE_TEMPERATURE){
        if (PIR1bits.ADIF){
            temperatureReading = AD_READING;                                 ///< Store our ADC value in ADRESH (wiping off the 2 bits stored in ADRESH as we can get away with 8bit resolution)
            temperaturemV = (int)(SUPPLY_VOLTAGE*temperatureReading);  ///< make the value an int so it can be used with the inToDisplay function. 500 is our max mV based on supply Voltage
            temperatureDegC = temperaturemV >> 10;                      ///< Bit shift by 10 to divide by 1024 to give the temp to the nearest DegC
            temperatureDegC = temperatureDegC + offset;
            PIR1bits.ADIF = 0;
            setTimeTag(tempSensorDelay, &tempSensorTimer);
            tempFlags.tempState == ADCSETUP;
        }

        else{}
            
    }
}

void calibrateTempSensor(void){
    offset = calibrationTemperature - temperaturemV;
}