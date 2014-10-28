#include "masterHeader.h"

void handleReceptionLocal(void)
{
    /// Check if we have not scanned everything that was received
    if(scPosition < rcPosition)
    {
        /// Scan any un-scanned characters
        char buttonNumber = userInputBuffer[scPosition];

        if(systemFlags.numberInput)
        {
            if(buttonNumber == GOBUTTON)
            {
                char *inputPtr = userInputBuffer;
                parsedNumber = 0;
                while(*inputPtr++ != GOBUTTON)
                {
                    parsedNumber += 10*parsedNumber + (*inputPtr);
                }
                systemFlags.numberParsed = 1;
                rcPosition = 0;
                scPosition = 0;
            }
            else if(buttonNumber < 10)
            {
                LCDBottomLine[scPosition + 2] = buttonNumber + '0';
                systemFlags.LCDRequiresUpdate = 1;
                scPosition++;
            }
            return;

        }
        else
        {
            //systemFlags.commandReceived = 1;
            
            if(buttonNumber == GOBUTTON)
            {
                moveToChildNode();
                stringToRam(nodeNames[currentNode->label],LCDTopLine);
                LCDBottomLine[0] = '>';
                stringToRam(nodeNames[(currentNode->child)->label], LCDBottomLine + 1);
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

                LCDBottomLine[0] = '>';
                stringToRam(nodeNames[(currentNode->child)->label], LCDBottomLine + 1);
            }
            else if(buttonNumber == DOWNBUTTON)
            {
                selectNextChild();
                LCDBottomLine[0] = '>';
                stringToRam(nodeNames[(currentNode->child)->label], LCDBottomLine + 1);
            }
            else if(buttonNumber == BACK)
            {
                moveToParentNode();
                stringToRam(nodeNames[currentNode->label],LCDTopLine);
                LCDBottomLine[0] = '>';
                stringToRam(nodeNames[(currentNode->child)->label], LCDBottomLine + 1);
            }

            rcPosition = 0;
            scPosition = 0;
            return;
        }
    }
}

void handleTransmissionLocal(void)
{
//    if(systemFlags.numberInput)
//    {
//        bottomLine[0] = '>';
//    }
//    char ramToRomBuffer[INPUTSIZE];
//
//    if(systemFlags.optionsShown)
//    {
//        return;
//    }
//
//    LCDMoveCursor(0, 0);
//    stringToRam(nodeNames[currentNode->label], ramToRomBuffer);
//    LCDWriteHere(ramToRomBuffer);
//
//    if(currentNode->child == NULL)
//    {
//        LCDMoveCursor(1, 0);
//        ///... do appropriate stuff when modules integrate
//        ///... probably set as numberInput mode here
//    }
//    else
//    {
//        LCDMoveCursor(1, 0);
//        stringToRam(msgArrow, ramToRomBuffer);
//        LCDWriteHere(ramToRomBuffer);
//
//        LCDMoveCursor(1, 1);
//        stringToRam(nodeNames[(currentNode->child)->label], ramToRomBuffer);
//        LCDWriteHere(ramToRomBuffer);
//    }
//
//
//
//    systemFlags.optionsShown = 1;
}