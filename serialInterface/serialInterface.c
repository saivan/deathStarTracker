/**********************************************************
**  File: serialInterface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/


#include "system.h"
#include "ConfigRegs.h"

/* High Priority Interrupt Service Routine */
#pragma code highISR = 0x0008
void gotoHighISR(void)
{
    _asm goto highPriorityISR _endasm   /* Simply go to 'highPriorityISR' */
}

/* 'highPriorityISR' function: High Priority Interrupt Action */
#pragma interrupt highPriorityISR
void highPriorityISR(void)
{
    /* Start processing the received inputs if something was received */
    if(PIR1bits.RCIF == 1)
    {
        /* Read the received character into a 2 byte buffer */
        circBuffer[rcPosition] = RCREG;
        rcPosition++;
    }
}

#pragma code
/* 'main' function */
void main(void)
{
    /* Setup and Config the microcontroller */
    serialSetup();

    /* Wait for reception */
    for(EVER)
    {
        while(scPosition < rcPosition)
        {
            rcWord[0] = circBuffer[scPosition];

            /* Echo what was received */
            transmit(rcWord);

            /* If the character was not a carriage return, then */
            /* Note the change of position by incrementing and check whether or *
             * not we have reached the end of our buffer */
            if(scPosition++ >= MAXPOS)
            {
                transmit(maxReached);
                rcWord[0] = '\r';
            }

            if(rcWord[0] == '\r')
            {
                /* If the character is a carriage return, or if the maximum position of *
                 * the buffer was reached, then do the following:                       */

                /* Attach LF, and NULL to the end and note the change of position */
                circBuffer[scPosition] = '\0';

                /* Transmit data back to the user and re-prompt them again */
                transmit(youTyped);
                transmit(circBuffer);
                prompt();

                scPosition = 0;
                rcPosition = 0;
            }

        }

    }
}

/* 'tx232' function: Transmits characters of a given null terminated string */
/* Arguments: [1] A pointer to the start of a string */
void transmit(unsigned char* str)
{ 
    /* Copy the pointer, so that the original message is not modified */
    unsigned char* pchar = str;

    /* Transmit the message while the character is not a NULL character */
    while(*pchar != '\0')
    {
        /* Wait for Transmit Buffer, TXREG, to be empty */
        while(PIR1bits.TXIF == 0) {/* Do nothing and just wait */}

        /* Once it is empty, transmit the message*/
        TXREG = *pchar++;
    }
}


// ----- functions -----
void prompt(void)
{
    
    if(systemFlags.factory)
    {
        transmit(msgFactory);
    }
    else
    {
        transmit(msgUser);
    }
    
    transmit(msgStarTracker);
    transmit(msgEndPrompt);
}