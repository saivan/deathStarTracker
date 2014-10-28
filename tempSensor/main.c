#include "tempSensor.h"
#include "ConfigPIC18F452.h"

///< Function prototypes
void tempSensor (void);
void calibrateTempSensor(void);

void main(void){
    LCDInitialise();
    tempSensorInitialisation();
    while(1){
        tempSensor();
    }
}
