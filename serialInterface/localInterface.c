#include "system.h"


void handleReceptionLocal(void)
{
    /// Check if we have not scanned everything that was received
    if(scPosition < rcPosition)
    {
        /// Scan any un-scanned characters
        char buttonNumber = userInputBuffer[scPosition];

        if(systemFlags.numberInput)
        {
            if(buttonNumber < 10)
            {

            }

            scPosition++;

        }
        else
        {
            systemFlags.commandReceived = 1;
            
            if(buttonNumber == GOBUTTON)
            {
                moveToChildNode();
            }
            else if(buttonNumber == UPBUTTON)
            {
                
                
                if(currentNode->child == NULL)
                {
                    return;
                }

                else if((currentNode->child)->sibling == NULL)
                {
                    return;
                }
                else
                {
                    Node * currentChildNode = currentNode->child;

                    selectNextChild();

                    while((currentNode->child)->sibling != currentChildNode)
                    {
                        selectNextChild();
                    }
                }
            }
            else if(buttonNumber == DOWNBUTTON)
            {
                selectNextChild();
            }
            else if(buttonNumber == BACK)
            {
                moveToParentNode();
            }
            else
            {
                systemFlags.commandReceived = 0;
            }

            rcPosition = 0;
            scPosition = 0;        
        }
    }
}

void updateLocalInterface(void)
{
    char ramToRomBuffer[INPUTSIZE];
    
    if(systemFlags.optionsShown)
    {
        return;
    }

    LCDMoveCursor(0, 0);
    stringToRam(nodeNames[currentNode->label], ramToRomBuffer);
    LCDWriteHere(ramToRomBuffer);

    if(currentNode->child == NULL)
    {
        LCDMoveCursor(1, 0);
        ///... do appropriate stuff when modules integrate
        ///... probably set as numberInput mode here
    }
    else
    {
        LCDMoveCursor(1, 0);
        stringToRam(msgArrow, ramToRomBuffer);
        LCDWriteHere(ramToRomBuffer);
        
        LCDMoveCursor(1, 1);
        stringToRam(nodeNames[(currentNode->child)->label], ramToRomBuffer);
        LCDWriteHere(ramToRomBuffer);
    }
    


    systemFlags.optionsShown = 1;
}