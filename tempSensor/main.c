#include "tempSensor.h"
#include "ConfigPIC18F452.h"
#include "LCD.h"

///< Function prototypes
void tempSensor (void);
void calibrateTempSensor(void);
//void calibrateTempSensor (void);

void main(void){
    while(1){
        tempSensor();
    }
}
