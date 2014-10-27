#include "tempSensor.h"
#include "ConfigPIC18F452.h"

#define     HIGH 1
#define     LOW 0
#define     OUTPUT 0
#define     INPUT  0xff

void tempSensor (void);
//void calibrateTempSensor (void);

void main(void){
    PORTAbits.RA0  = 0x00;                               //Clear RA0
    TRISAbits.RA0 = INPUT;                               //Set RA0 as input


    ADCON0 = 0b01000001;                        //Fosc/8, A/D enable
    ADCON1 = 0b00001110;                        //Left justify, 1 analog channel

    while(1){
        tempSensor();
    }
}
