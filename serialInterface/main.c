
/**********************************************************
**  File: main.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/


#include "system.h"
#include "ConfigRegs.h"

void setup(void)
{
    serialSetup();
    interfaceSetup();
}

/* 'main' function */
void main(void)
{
    /* Setup and Config the microcontroller */
    setup();

    for(EVER)
    {
        /* Any received bytes will be handled here */
        handleReception();

        // See C++ test code
        showChildOptions();
	executeOwnFunction();
        handleTransmission();
    }
}