
#include "system.h"
ButtonFlags buttonFlags = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned char buttonPressValue = 0;

void buttonsSetup(void)
{
    TRISB = INPUT;

    ///< Interrupt initialisation
    RCONbits.IPEN = HIGH;                       ///< enable priority interrupts
    INTCONbits.GIEH = HIGH;                     ///< enables all high priority interrupts. MAYBE NEED GIEL IF NOT WORKING?
    INTCONbits.GIEL = HIGH;
    INTCON2bits.INTEDG1 = HIGH;
    INTCONbits.RBIE = LOW;
    INTCON3bits.INT1IP = LOW;
    INTCON3bits.INT1IE = HIGH;
    INTCON3bits.INT1IF = LOW;

    //< Don't have to enable INT0 priority as it is always high priority

}



