

#include "masterHeader.h"
#include "Configurations.h"

// static char LCDState = 0;

void setup( void ){	
	/// Setup Routines go here
  treeSetup();
  serialSetup();
  enterRemoteMode();
	//setupRealTimeTimer();   
 //  setupServos();
	// USSetup();
  enterRemoteMode();
}


void main( void ){	

	setup();
        
  while(1)
  {

        //updateTime();
        //track();            
  
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

        }

        executeCurrentNodeFunction();        


  //       /// Update the LCD
  //       if( eventDue(&LCDUpdate) ){
  //       	/// Display stuff to the screen
  //       	if( LCDState == 0 ){        		
  //       		LCDInstruction(EMPTY_DISPLAY,COMMAND_LCD);
  //       		setTimeTag(2,&LCDUpdate);
  //       		LCDState++;
  //      		} else if ( LCDState == 1 ){
  //      			LCDWriteHere( Hello );
  //      			LCDMoveCursor(1,0);
  //      			setTimeTag(2,&LCDUpdate);
  //               LCDState++;
  //      		} else if ( LCDState == 2 ){
  //      			intToDisplay((int)(IRDistance),0);
  //      			LCDWriteHere(displayChars.characters);
  //      			LCDMoveCursor(1,10);
  //      			setTimeTag(2,&LCDUpdate);
  //               LCDState++;
  //      		} else if( LCDState == 3 ){
  //      			intToDisplay((int)(trackFlags.targetFound),0);
  //      			LCDWriteHere(displayChars.characters);       			
  //      			setTimeTag(35,&LCDUpdate);
  //      			LCDState = 0;
  //      		}
		// }


    }
}
