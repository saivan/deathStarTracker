
#include "system.h"

SystemFlags systemFlags;

/* 'highPriorityISR' function: High Priority Interrupt Action */
#pragma interrupt highPriorityISR
void highPriorityISR(void)
{

}

/* 'lowPriorityISR' function: Low Priority Interrupt Action */
#pragma interrupt lowPriorityISR
void lowPriorityISR(void)
{
    /* Start processing the received inputs if something was received */
    if(PIR1bits.RCIF == 1)
    {
        /* Read the received character into a 2 byte buffer */
        circBuffer[rcPosition] = RCREG;
        rcPosition++;
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

void toggleFactoryMode(void)
{
	// print: ask the user to confirm
	if (systemFlags.userYes)
	{
		systemFlags.factory = !systemFlags.factory;
		systemFlags.userYes = 0;
		systemFlags.optionsShown = 0;
		updateTreeStructure();
	}
}

void toggleRemoteMode(void)
{
	// print: ask the user to confirm
	if (systemFlags.userYes)
	{
		systemFlags.remote = !systemFlags.remote;
		systemFlags.userYes = 0;
	}
}

void showChildOptions(void)
{
        int i = 0;

        Node * originalOptionNode = currentNode->child;

	if (systemFlags.optionsShown)
	{
		return;
	}

	if (currentNode->child == NULL)
	{
		return;
	}

//	cout << "\n------------------------------\n";
//	cout << (string)(nodeNames[currentNode->label]);
//	cout << "\n------------------------------\n";

	if (((currentNode->child)->label == OUT_OF_RANGE_NODE) | ((currentNode->child)->label == ENTRY_SUCCESS_NODE))
	{
		return;
        }

	do
	{
		//print child name
//		cout << i << ". " << (string)(nodeNames[(currentNode->child)->label]) << "\n";
		changeChildSelectionNode();
		i++;
	} while (currentNode->child != originalOptionNode);

	systemFlags.optionsShown = 1;
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