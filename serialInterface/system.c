#include "system.h"

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