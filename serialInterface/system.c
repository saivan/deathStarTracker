#include "system.h"

SystemFlags systemFlags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief High Priority Interrupt Service Routine
 */
#pragma interrupt highPriorityISR
void highPriorityISR(void)
{

}

/**
 * @brief Low Priority Interrupt Service Routine
 */
#pragma interrupt lowPriorityISR
void lowPriorityISR(void)
{
    /* Start processing the received inputs if something was received */
    if(PIR1bits.RCIF)
    {
        /* Read the received character into a 2 byte buffer */
        userInputBuffer[rcPosition++] = RCREG;
    }
    else if(INTCONbits.RBIF)
    {
        buttonPressValue = PORTB >> 2;
        buttonPressValue &= LOWNIBBLE;
        userInputBuffer[rcPosition++] = buttonPressValue;
//        if(buttonPressValue == UPBUTTON)
//        {
//            //userInputBuffer[rcPosition++] = UPCHAR;
//        }
//        else if(buttonPressValue == DOWNBUTTON)
//        {
//            //userInputBuffer[rcPosition++] = DOWNCHAR;
//        }
//        else if(buttonPressValue == BACKBUTTON)
//        {
//            userInputBuffer[rcPosition++] = '\b';
//        }
//        else if(buttonPressValue == GOBUTTON)
//        {
//            userInputBuffer[rcPosition++] = '\r';
//        }
//        else
//        {
//            userInputBuffer[rcPosition++] = buttonPressValue;
//        }

        INTCONbits.RBIF = 0;
        buttonFlags.buttonPressed = 1;
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



void showTargetStatus(void)
{

	if (systemFlags.targetfound)
	{
		//...do appropriate stuff
	}
	else
	{
		//...do appropriate stuff
	}
}

void showTemperature(void)
{
	// Show temperature stuff

}