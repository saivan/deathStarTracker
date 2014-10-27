#include "tempSensor.h"
#include "ConfigPIC18F452.h"
#include "LCD.h"

///< Make some defintions
#define     HIGH            1
#define     LOW             0
#define     OUTPUT          0
#define     INPUT           1
#define     TEMPERATURE_PIN PORTAbits.RA1
#define     TEMPERATURE_DDR TRISAbits.RA1                   ///< DDR stands for Data Direction Register

///< Function prototypes
void tempSensor (void);
//void calibrateTempSensor (void);

void main(void){
    TEMPERATURE_PIN = 0x00;                                 ///< Clear RA0
    TEMPERATURE_DDR = INPUT;                                ///< Set RA0 as input


    ADCON0 = 0b01001001;                                    ///< Fosc/8, A/D enable, RA1
    ADCON1 = 0b10001110;                                    ///< Right justify (so we can use 10 bit res. only left justify when wiping off 2 LSB), 1 analog channel
    LCDInitialise();                                        ///< Initialise the LCD

    while(1){
        tempSensor();
    }
}
