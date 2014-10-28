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
#include "LCD.h"

#ifdef  USING_PIC18F4520
#include "ConfigPIC18F4520.h"
#endif

#ifdef  USING_PIC18F452
#include "ConfigPIC18F452.h"
#endif


void updateLCD( void ){
    static unsigned char LCDState = 0;
    if ( systemFlags.LCDRequiresUpdate ) {
       if( eventDue(&LCDUpdate) ) {
         	/// Display stuff to the screen
         	if( LCDState == 0 ){
         		LCDInstruction(EMPTY_DISPLAY,COMMAND_LCD);
         		setTimeTag(3,&LCDUpdate);
         		LCDState++;
        	} else if ( LCDState == 1 ){
        		LCDWriteHere( LCDTopLine );
        		setTimeTag(2,&LCDUpdate);
                        LCDMoveCursor(1,0);
                        LCDState++;
        	} else if( LCDState == 2 ){
        		LCDWriteHere( LCDBottomLine );
        		setTimeTag(60,&LCDUpdate);
        		LCDState = 0;
                }
        }
    }
}




void setup(void)
{
    treeSetup();
    serialSetup();
    selectNextChild(); selectNextChild(); selectNextChild();
    setupRealTimeTimer();
    PIE1bits.TXIE = 0;
    PIE1bits.RCIE = 0;
//    enterRemoteMode();
    enterLocalMode();
}

/* 'main' function */
void main(void)
{

    
    timeTag LCDUpdate = { 0, 0, 0, 0 };

    /* Setup and Config the microcontroller */
    setup();
    for(EVER)
    {
        updateTime();

        if(systemFlags.remote)
        {
            handleReception();
            
            handleTransmission();
            if(systemFlags.updatePrompt)
            {
                systemFlags.updatePrompt = 0;
                showChildOptions();
                printRomString(msgNewLine);
                printRomString(msgNewLine);
                prompt();
            }
        }
        else
        {
            handleReceptionLocal();
            handleTransmissionLocal();
            updateLCD();
                /// Update the LCD
//                LCDStuff();
            
        }

        executeCurrentNodeFunction();
    }
}