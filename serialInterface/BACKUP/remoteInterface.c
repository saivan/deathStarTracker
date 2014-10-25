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

char isCharSelectionDigit(char character)
{
    if( ((character - '0') < numberOfChildren) && ((character - '0') >= 0) )
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Check whether or not an up or down is pressed, and deal with it
 * 
 * @details Note that this function depends on 'scPosition' and can also change
 * 'scPosition'
 * 
 * @return Returns 1 to stop handling received bytes
 */
char checkAndDealWithUpDown(void)
{
    /// If we have scanned more than 2 characters (so that the position
    /// is equal to 2 or more), we can determine if it was an up or down
    /// character sequence
    /// Note that an up character sequence is {ESC, '[', 'A'}
    /// and a down character sequence is {ESC, '[', 'B'}
    if(scPosition >= 2)
    {
        /// Hence, to check it, check if the previous two characters are
        /// {ESC, '['}
        if((userInputBuffer[scPosition - 2] == ESC)
                && (userInputBuffer[scPosition - 1] == '['))
        {

            /// At this stage, we definitely know that the user has pressed
            /// an arrow control button. However, if the user has not
            /// chosen at least one option yet, we do not have a reference
            /// option so that the up/down buttons should take no effect
            if(!systemFlags.userChosen)
            {
                /// If that case, just ignore the user input and pretend
                /// that we have not received anything
                rcPosition = 0;
                scPosition = 0;
                return 1;
            }

            /// Otherwise, if the user has chosen something, we can check
            /// which key press it is and signal the system which one
            /// has been pressed

            else if(userInputBuffer[scPosition] == 'A')
            {
                systemFlags.upPressed = 1;
                systemFlags.downPressed = 0;
            }
            else if(userInputBuffer[scPosition] == 'B')
            {
                systemFlags.downPressed = 1;
                systemFlags.upPressed = 0;
            }
        }
    }

    return 0;
}

/**
 * @brief Echo the received bytes if it is deemed okay to
 *
 * @details Note that this function can change 'scPosition'
 *
 * @return Returns 1 to stop handling received bytes
 */
char echoIfOkay(char inChar)
{
    /// If we have received characters that lead up to the up and down
    /// character sequence, simply keep them in the buffer and stop
    if((inChar == ESC) || (inChar == '['))
    {
        scPosition++;
        return 1;
    }
    else if(systemFlags.upPressed | systemFlags.downPressed)
    {
        scPosition++;
    }
    else if (!((inChar == 'b') && (systemFlags.numberInput == 0)))
    {
        rcWord[0] = inChar;
        printRamString(rcWord);
    }

    return 0;
}

void handleReception(void)
{
    /// Check if we have not scanned everything that was received
    if(scPosition < rcPosition)
    {
        /// Scan any un-scanned characters
        char inChar = userInputBuffer[scPosition];

        if((inChar == '\r') && (scPosition == 0))
        {
            userInputBuffer[scPosition] = '0';
            inChar = '0';
        }
        else if((inChar == '\t') && (scPosition > 0))
        {
            userInputBuffer[scPosition] = '\r';
            if(tryAutoComplete(userInputBuffer))
            {
                scPosition = 0;
                rcPosition = 0;
                systemFlags.numberInput = 0;
                systemFlags.userChosen = 0;
                systemFlags.commandReceived = 1;
            }
            else
            {
                userInputBuffer[scPosition] = '\0';
                rcPosition--;             
            }
            return;        
        }
        /// This checks if we have received an up or down button and deals
        /// with it
        if(checkAndDealWithUpDown())
        {
            return;
        }

        /// If the received byte was a number and the user has chosen something,
        /// clear the prompt so that we replace the user's previous choice
        /// with the current choice later on
        if(systemFlags.userChosen & isCharSelectionDigit(inChar))
        {
            /// Note that this reprompts the user at the same line!
            prompt();
        }

        /// Echo the user input if it is okay
        if(echoIfOkay(inChar))
        {
            return;
        }

        if(inChar == '\b')
        {
            /// We can only back-space if things have been typed
            if((scPosition > 0) && (systemFlags.userChosen == 0))
            {
                /// If so, to back-space, delete the '\b' that was received
                /// and also the character to be deleted, which is a total of
                /// 2 characters
                scPosition -= 2;
                rcPosition -= 2;
                
                /// Replace the character with a space to create a back-space
                /// effect
                printRomString(msgSpaceBackSpace);
            }

            /// Otherwise, if nothing has been typed, we will treat it like a
            /// back command
            else
            {
                /// Pretend we have received the back command
                userInputBuffer[scPosition] = BACK;
                inChar = BACK;

                /// Print a space to compensate for the user's backspace
                printRomString(msgSpace);
            }
        }

        /// If we are not inputting numbers, we can respond to the special
        /// commands
        if(!systemFlags.numberInput)
        {
            /// If an up has been pressed, do the following
            if(systemFlags.upPressed)
            {
                /// 
                rcPosition = 0;
                scPosition = 0;
                userInputBuffer[rcPosition++] =
                        (userInputBuffer[0] == '0') ?
                            (numberOfChildren - 1 + '0') : (userInputBuffer[0] - 1);
                systemFlags.upPressed = 0;
                return;
            }
            else if(systemFlags.downPressed)
            {
                rcPosition = 0;
                scPosition = 0;
                userInputBuffer[rcPosition++] =
                        (userInputBuffer[0] == (numberOfChildren - 1 + '0')) ?
                            '0' : (userInputBuffer[0] + 1);
                systemFlags.downPressed = 0;
                return;
            }
            else if(isCharSelectionDigit(inChar))
            {

                Node* currentChild = currentNode->child;

                selectChild(inChar - '0');

                printRomString(msgDot);
                printRomString(nodeNames[(currentNode->child)->label]);
                printRomString(msgSpace);
                printRomString(msgDeleteInFront);
                currentNode->child = currentChild;
                systemFlags.userChosen = 1;      
            }
            else if(inChar == BACK)
            {
                if(!((scPosition > 0) && (userInputBuffer[scPosition - 1] == 'i')))
                {
                    rcPosition++;
                    scPosition++;
                    userInputBuffer[scPosition] = '\r';
                    printRomString(msgBack);
                    systemFlags.commandReceived = 1;
                }
                else
                {
                    rcWord[0] = BACK;
                    printRamString(rcWord);
                }
            }
        }
        
        /* If the character was not a carriage return, then */
        /* Note the change of position by incrementing and check whether or *
         * not we have reached the end of our buffer */
        if(scPosition++ > MAXPOS)
        {
            printRomString(msgMaxReached);
            systemFlags.commandReceived = 1;
        }

        if(inChar == '\r')
        {
            systemFlags.commandReceived = 1;
        }
        if(systemFlags.commandReceived)
        {
            /* If the character is a carriage return, or if the maximum position of *
             * the buffer was reached, then do the following:                       */

            /* Attach LF, and NULL to the end and note the change of position */
            userInputBuffer[scPosition] = '\0';

            if(checkClear(userInputBuffer)){}
            else if(checkPassword(userInputBuffer)){}
            else if(checkIfNodeNameTyped(userInputBuffer)){}
            else
            {
                checkReset(userInputBuffer);
                parseUserInput(userInputBuffer);
            }

            scPosition = 0;
            rcPosition = 0;
            systemFlags.numberInput = 0;
            systemFlags.userChosen = 0;
        }
        
    }
}

void printRamString(static char *ramMessage)
{
    romramIndicator &= !(BIT(cueIntoIndex));
    toPrintStringsRam[cueIntoIndex++] = ramMessage;
}

void printRomString(static char rom *romMessage)
{
    romramIndicator |= (BIT(cueIntoIndex));
    toPrintStringsRom[cueIntoIndex++] = romMessage;
}


void prompt(void)
{
    Node *followNodePath = &rootNode;
    
    if(systemFlags.factory)
    {
        printRomString(msgFactory);
    }
    else
    {
        printRomString(msgUser);
    }

    printRomString(nodeNames[followNodePath->label]);
    while(followNodePath != currentNode)//ASSIGNMENT NOT COMPARE
    {
        printRomString(msgSlash);
        followNodePath = followNodePath->child;
        printRomString(nodeNames[followNodePath->label]);
    }  
    
    printRomString(msgEndPrompt);
}

//char isCharDigit(static char letter)
//{
//    if((letter < '0') || (letter > '9'))
//    {
//        return 0;
//    }
//
//    return 1;
//}
//
//char isStringAllDigit(static char *inputString)
//{
//    int i = 0;
//
//    while(inputString[i] != '\r')
//    {
//        if((inputString[i] < '0') || (inputString[i] > '9'))
//        {
//            return 0;
//        }
//        i++;
//    }
//
//    return 1;
//}

int convertToNumber(static char *inputString)
{
    int number = 0;
    unsigned char i = 0;

    unsigned int maxParseLength = 3;
    
    if(inputString[i] == '-')
    {
        i++;
        maxParseLength++;
    }
    
    while(inputString[i] != '\r')
    {
        if((inputString[i] < '0') || (inputString[i] > '9') || (i > maxParseLength))
        {
            systemFlags.userError = 1;
            return i; // for error detection
        }
        number = (number * 10) + (inputString[i++] - '0');
    }

    if(inputString[i] == '-')
    {
        number = 0 - number;
    }
    
    systemFlags.userError = 0;
    return number;
    // works!
}

void parseUserInput(static char *inputString)
{

    char charToCheck = inputString[scPosition - 2];
    if(systemFlags.numberInput)
    {
        unsigned int inputValue = convertToNumber(inputString);

        //imposeValueConstraints();
        // it will grab the maximum and minimum values and turn userError on if
        // it is out of it, the min/max values are set by entering each node
        // each node has a different value
        //
        
        if(systemFlags.userError)
        {
            printRomString(msgNumberError);
            return;
        }

        //writeValue();
        // it will write inputValue to the correct variable. it will be pointed
        // by a pointer. the pointer is pointed to the number when when we enter
        // the node

        //how about systemFlags.oneTime? set at the end of each
        //node function if to be executed once and in the beginning if set don't continue

        
    }
    
//    if(inputString[1] != '\r')
//    {
//
//        printRomString(msgDigitError);
//
//        return;
//    }
   
    
    if(charToCheck == BACK)
    {
        moveToParentNode();
    }
    else
    {
        char digit = charToCheck - '0';

        if(digit >= numberOfChildren || digit < 0)
        {
            printRomString(msgDigitError);
        }
        else
        {

            moveToChildSelection(digit);
        }
    }
}

char stringCompare(static char *inputString, static char *ramString)
{
    int i = 0;
    while(ramString[i])
    {
        if(ramString[i] != inputString[i])
        {
            return 0;
        }
        i++;
    }

    if(inputString[i] != '\r')
    {
        return 0;
    }

    return 1;
}
char checkPassword(static char *inputString)
{
    if(stringCompare(inputString, password))
    {
        systemFlags.factory = 1;
        updateTreeStructure();
        printRomString(msgSeparatorLine);
        printRomString(msgWelcomeFactory);
        printRomString(msgSeparatorLine);
        systemFlags.optionsShown = 0;
        return 1;
    }
    return 0;
}

char checkClear(static char *inputString)
{
    if(stringCompare(inputString, msgClear))
    {
        clearScreen();
        systemFlags.optionsShown = 0;
        return 1;
    }
    return 0;
}

char tryAutoComplete(static char *inputString)
{
    if(numberOfChildren)
    {
        Node *originalOptionNode = currentNode->child;
        char romToRamBuffer[INPUTSIZE];
        int i = 0;
        do
        {
            selectNextChild();
            stringToRam(nodeNames[(currentNode->child)->label], romToRamBuffer);

            for(i = 0; inputString[i] == romToRamBuffer[i]; i++) 
            {
                if(inputString[i] == '\r')
                {
                    break;
                }
            }

            /// IF NULL, it means everything is the same until now
            if(inputString[i] == '\r')
            {
                prompt();
                printRamString(romToRamBuffer);
                moveToChildNode();
                systemFlags.optionsShown = 0;
                return 1;
            }          
        }
        while(currentNode->child != originalOptionNode);
    }

    return 0;
}

char checkIfNodeNameTyped(static char *inputString)
{
    if(numberOfChildren)
    {
        Node *originalOptionNode = currentNode->child;
        char romToRamBuffer[INPUTSIZE];
        do
        {
            selectNextChild();
            stringToRam(nodeNames[(currentNode->child)->label], romToRamBuffer);
            if(stringCompare(inputString, romToRamBuffer))
            {
                moveToChildNode();
                systemFlags.optionsShown = 0;
                return 1;
            }
        }
        while(currentNode->child != originalOptionNode);
    }

    return 0;
}

void checkReset(static char *inputString)
{
    if(stringCompare(inputString, msgReset))
    {
        systemReset();
    }
}

void showChildOptions(void)
{
        char i = '0';

        char iString[] = {i, '\0'};

        Node *originalOptionNode = currentNode->child;

	if (systemFlags.optionsShown)
	{
		return;
	}

        numberOfChildren = 0;

        printRomString(msgNewLine);
        printRomString(msgSeparatorLine);
        printRomString(nodeNames[currentNode->label]);
        printRomString(msgSeparatorLine);

	if (currentNode->child == NULL)
	{
		return;
	}

	if (((currentNode->child)->label == OUT_OF_RANGE_NODE) ||
                ((currentNode->child)->label == ENTRY_SUCCESS_NODE) ||
                ((currentNode->child)->label == HOME_NODE))
	{
		return;
        }

	do
	{
            iString[0] = i++;
            printRamString(iString);
            printRomString(msgDot);
            printRomString(nodeNames[(currentNode->child)->label]);
            printRomString(msgNewLine);

            selectNextChild();

	} while (currentNode->child != originalOptionNode);

        numberOfChildren = i++ - '0';
        
	systemFlags.optionsShown = 1;
}