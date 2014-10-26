#include "tempSensor.h"
#define     SUPPLY_VOLTAGE 5


char temperatureReading = 0;
unsigned int temperature_mV = 0;                                //Define a variables
unsigned int temperature_DegC = 25;

//#define TEMP_READING ADRES        ///< Location that the ADC stores the converted values
//#define ADCON0_TEMP_SETTING 0x51     ///< Value that ADCON0 must be set to for operation
//#define ADCON1_TEMP_SETTING 0x0E     ///< Value that ADCON1 must be set to for operation

void tempSensor(void){
   ADCON0bits.GO=1;                            // STart A/D conversion

/*Wait for AD conversion*/
    while (PIR1bits.ADIF != 1){                 //set up loop to wait for the AD conversion complete flag to set
        temperatureReading = ADRESH;                             // Write digital result value to variable Voltage
        temperature_mV = SUPPLY_VOLTAGE*1000;
        temperature_mV = temperature_mV/255;
        temperature_mV = temperatureReading*temperature_mV;
        temperature_DegC = temperature_mV/10;
        PIR1bits.ADIF = 0;
    }
}


//void tempSensor(void){
//   ADCON0bits.GO=1;                            // STart A/D conversion
//
///*Wait for AD conversion*/
//    while (PIR1bits.ADIF != 1){                 //set up loop to wait for the AD conversion complete flag to set
//        temperatureReading = ADRESH;                             // Write digital result value to variable Voltage
//        temperature_mV = SUPPLY_VOLTAGE*1000;
//        temperature_mV = temperature_mV/255;
//        temperature_DegC = temperature_mV/10;
//    }
//}

//TEMPFlagsType TEMPFlags={0,0,0};
//timeTag TEMPSensorTimer = {0,0,0,0};  ///< TimeTag for next time IRSensor is to be called
//unsigned int TEMPSensorDelay = 100;     ///< Delay before IRSensor can be called again in ms
//
//#pragma code
////< probably do this one just as a define in the main integrated part
//void tempSensorInitialisation (void){
//    TRISAbits.RA0 = INPUT;
//}
//
//
////< Need a calibration function tempSensor
//
//void tempSensor(void){
//    //< If the event is not due, return from the function with no further action
//    if (~eventDue(&TEMPSensorTimer)){
//        return;
//    }
//    //< If gathering data then IR sensor is busy.
//    if ( TEMPFlags.TEMPState == GATHERING_DATA ){
//        if( 0 /* IR SENSOR IS BUSY */ ){
//                return;
//        }
//        setTimeTag(TEMPSensorDelay, &TEMPSensorTimer);  ///< Set time that IRSensor is next called
//
//        if (PIR1bits.ADIF){///< If there is data to be harvested
//            gatheredData[sampleNumber] = (TEMP_READING >> 6); ///< Store gathered data in memory
//            sampleNumber++;     ///< increment new data counter
//            if (sampleNumber == samplesPerEstimate){///< If we have enough data
//                sampleNumber = 0;
//                IRFlags.IRState = CALCULATING_DISTANCE;     ///< Move to calculating distance
//                return;
//            }
//        }
//
//        /// Set up AD conversion settings
//        ADCON0 = ADCON0_SETTING;
//        ADCON1 = ADCON1_SETTING;
//        ADCON0bits.GO = 1;  ///< If more data is needed, start AD conversion again
//        return;
//    } else {    ///< IRState is CALCULATING_DISTANCE
//        IRFlags.distanceReady = 1;
//        reading = gatheredData[0];
//
//        ///Calculate average reading
//        if (samplesPerEstimate == 2){
//            reading += gatheredData[1];
//            reading = reading >> 1;
//        } else if (samplesPerEstimate == 4){
//            reading += gatheredData[1];
//            reading += gatheredData[2];
//            reading += gatheredData[3];
//            reading = reading >> 2;
//        } else {
//            i=1;
//            while (i<samplesPerEstimate){
//
//                reading += gatheredData[i];
//                i++;
//            }
//            reading = reading/samplesPerEstimate;
//        }
//
//        if (reading < 50){      ///< If object is not found
//            currentIRDistance = 0;
//        } else {
//            currentIRDistance = 10*((calibrationVariable)/(reading));
//            if (currentIRDistance > 1500 || currentIRDistance < 500){ ///@todo change to range max and min
//                currentIRDistance = 0;
//            }
//        }
//
//        IRFlags.IRState = GATHERING_DATA;
//        return;
//    }
//}
//
///**
// * calibrateIR() allows the user to calibrate the IR module to work off a calculated value.
// *
// * This is set by having the user place the object being tracked at 1m distance, and press a button.
// *
// * This will change the value of calibrationVariable which is used in distance calculation.
// *
// */
//void calibrateIR(void){
//    ///@Kelvin print startCalibration
//    IRFlags.calibrationInProgress = 0;
//
//    while(IRFlags.calibrationInProgress){  ///< While calibration is not over
//
//        while (0 /*button is not pressed*/){    //stall
//        }
//
//        /// Set up AD conversion settings
//        ADCON0 = ADCON0_SETTING;
//        ADCON1 = ADCON1_SETTING;
//        ADCON0bits.GO = 1;  ///< Start AD conversion
//
//        while(~PIR1bits.ADIF);  ///< Wait until AD conversion is complete
//
//        calibrationVariable = (IR_READING >> 6) * 1000;  ///< Calculate calibration variable
//
//        if (calibrationVariable > 500){     ///< If the calculated variable is valid
//            IRFlags.calibrationInProgress = 1;  ///< Calibration is over
//            ///@Kelvin print calibrationSucessful
//            //could tell the user that calibration was successful with a pause
//
//        } else {
//            ///@Kelvin print targetNotFound
//
//        }
//    }
//    return;
//
//}
//
//void calibrateTempSensor(void){
//
//}


