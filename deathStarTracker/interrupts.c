
#include "masterHeader.h"

SystemFlags systemFlags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief High Priority Interrupt Service Routine
 */
#pragma interrupt highPriorityISR
void highPriorityISR(void)
{
    /// Receive Interrupt
    if(PIR1bits.RCIF)
    {
        userInputBuffer[rcPosition++] = RCREG;
    }
    /// Transmit Interrupt
    if(PIR1bits.TXIF)
    {
        if(romramIndicator[toPrintIndex - 1].inRom)
        {
            TXREG = *txPtr.romPtr++;

            if(!*txPtr.romPtr)
            {
                PIE1bits.TXIE = 0;
            }
        }
        else
        {
            TXREG = *txPtr.ramPtr++;

            if(!*txPtr.ramPtr)
            {
                PIE1bits.TXIE = 0;
            }
        }
    }
    
    /// Handling the servos 
    if( PIR1bits.CCP1IF ){
         PIR1bits.CCP1IF = 0;                       ///< Clear the flag for the next interrupt
         if( ~servoFlags.azimuthFired ){    
             AZIMUTH_PIN = 1;                       ///< Turn on the azimuth servo
             CCPR1 = azimuthServoOnTime;            ///< Set CCP1 to the Azimuth on time
             servoFlags.azimuthFired = 1;           ///< Note that the azimuth servo was triggered
         } else if ( ~servoFlags.elevationFired ){
             AZIMUTH_PIN = 0;                       ///< Turn off the Azimuth servo
             ELEVATION_PIN = 1;                     ///< Turn on the elevation servo
             CCPR1 = elevationServoOnTime;          ///< Set CCP1 to the Elevation on time
             servoFlags.elevationFired = 1;         ///< Note that the elevation servo was triggered
         } else {   
             ELEVATION_PIN = 0;                     ///< Turn off the Elevation servo
             CCPR1 = servoOffTime;                  ///< Move the off time to the CCP1
             servoFlags.azimuthFired = 0;           ///< Restart the azimuth cycle
             servoFlags.elevationFired = 0;         ///< Restart the elevation cycle
         }
    }


}

/**
 * @brief Low Priority Interrupt Service Routine
 */
#pragma interrupt lowPriorityISR
void lowPriorityISR(void)
{

    /// Handling the timer
    if( INTCONbits.TMR0IF )
    {
        time.updatesRequired++;                     ///< Flag another update for the main
        INTCONbits.TMR0IF = 0;                      ///< Clear the interrupt flag
    }
    else if(INTCON3bits.INT1IF)
    {
        buttonPressValue = PORTB >> 2;
        buttonPressValue &= LOWNIBBLE;
        userInputBuffer[rcPosition++] = buttonPressValue;

        INTCON3bits.INT1IF = 0;
    }
    
}

/* High Priority Interrupt Service Routine */
#pragma code highISR = 0x0008
void gotoHighISR(void)
{
    _asm goto highPriorityISR _endasm   /* Simply go to 'highPriorityISR' */
}

/* Low Priority Interrupt Service Routine */
#pragma code lowISR = 0x0018
void gotoLowISR(void)
{
    _asm goto lowPriorityISR _endasm   /* Simply go to 'lowPriorityISR' */
}

#pragma code
// These functions need to be put in the correct file when other modules are integrated

void systemReset(void)
{
    _asm reset _endasm
}

