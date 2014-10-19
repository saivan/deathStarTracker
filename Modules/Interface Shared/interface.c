
/**********************************************************
**  File: interface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/
#include "system.h"

rom const char rom *nodeNames[] = {
	/* User Mode */
	"DeathStarTracker", //HOME_NODE,
	"Target Status", //TARGET_STATUS_NODE,
	"Temperature", //TEMPERATURE_NODE,
	"Goto Position", //GOTO_POSITION_NODE, /* Select Azim or Elev */
	"Set Limits", //SET_LIMITS_NODE,
	"Mode Switch", //MODE_SWITCH_NODE,

	/* Under GOTO_POSITION_NODE */
	"Azimuth", //AZIM_METHOD_SELECT_NODE,
	"Elevation", //ELEV_METHOD_SELECT_NODE,

	"Manual (Azim)", //AZIM_MANUAL_CONTROL_NODE,
	"Set Azim Angle", //AZIM_SET_ANGLE_CONTROL_NODE,

	"Manual (Elev)", //ELEV_MANUAL_CONTROL_NODE,
	"Set Elev Angle", //ELEV_SET_ANGLE_CONTROL_NODE,

	/* Under SET_LIMITS_NODE */
	"Distance Max", //DIST_MAX_NODE,
	"Distance Min", //DIST_MIN_NODE,
	"Azimuth Max", //AZIM_MAX_NODE,
	"Azimuth Min", //AZIM_MIN_NODE,
	"Elevation Max", //ELEV_MAX_NODE,
	"Elevation Min", //ELEV_MIN_NODE,

	/* Under end of both GOTO_POSITION_NODE and SET_LIMITS_NODE */
	"Out of Range", //OUT_OF_RANGE_NODE,
	"Entry Success", //ENTRY_SUCCESS_NODE, /* Child is always home */

	/* Factory Mode */
	"Calibration", //CALIBRATION_NODE,
	"Set Ultrasound", //SET_US_NODE,
	"Set Infrared", //SET_IR_NODE,
	"Show Raw Data", //SHOW_RAW_NODE,
	"Show Statistics", //SHOW_STAT_NODE,

	/* Under CALIBRATION_NODE */
	"Calibrate Temp", //CALI_TEMP_NODE,
	"Calibrate Azim", //CALI_AZIM_NODE,
	"Calibrate Elev", //CALI_ELEV_NODE,

	"US: Sample/Est", //US_SAMPLE_PER_EST_NODE,
	"US: Sample Rate", //US_SAMPLE_RATE_NODE,

	"IR: Sample/Est", //IR_SAMPLE_PER_EST_NODE,
	"IR: Sample Rate", //IR_SAMPLE_RATE_NODE,
};

/**
* These strings are stored in an array in program
* memory, call stringToRam to move these functions
* from program memory to data memory.
*/
rom const char rom *descripStrings[] = {
	"Press <GO>",        /* PRESS_GO */
	"Target Not Found",  /* NOT_FOUND */
	"  Searching...",    /* SEARCHING */
	"Target Acquired",   /* ACQURIED */
	"Dist",              /* DIST */
	"Azim",              /* AZIM */
	"Elev",              /* ELEV */
	"Azimuth",           /* AZIMUTH */
	"Elevation",         /* ELEVATION */
	"Manually",          /* MANUALLY */
	"Set ",              /* SET */
	"Goto ",             /* GOTO */
	"Manual Move",       /* MANUAL_MOVE*/
	"Use Arrows",        /* USE_ARROWS */
	"Angle: ",           /* ANGLES */
	"Re-enter Input",    /* RE_ENTER */
	"Distance Min",      /* DIST_MIN */
	"Distance Max",      /* DIST_MAX */
	"Azimuth Min",       /* AZIM_MIN */
	"Azimuth Max",       /* AZIM_MAX */
	"Elevation Min",     /* ELEV_MIN */
	"Elevation Max",     /* ELEV_MAX */
	"Value:",            /* VALUE */
	"!",                 /* EXCLAIM */
	"?",                 /* QUESTION */
};



void moveToParentNode(void)
{
	systemFlags.optionsShown = 0;
	if (currentNode->parent == NULL)
	{
		return;
	}
	previousNode = currentNode;
	currentNode = currentNode->parent;
	currentNode->child = previousNode;
}

void moveToChildNode(void)
{
	systemFlags.optionsShown = 0;
	if (currentNode->child == NULL)
	{
		return;
	}
	previousNode = currentNode;
	currentNode = currentNode->child;
	currentNode->parent = previousNode; /* This means we don't have to declare parent */
}

void changeChildSelectionNode(void)
{
	/* If there is no child, then there is no selection to begin with */
	if (currentNode->child == NULL)
	{
		return;
	}

	/* If the child has no sibling, then there is no other selection */
	if ((*(currentNode->child)).sibling == NULL)
	{
		return;
	}

	currentNode->child = (*(currentNode->child)).sibling;
}

void jumpToSelection(char jumps)
{
	int i;

	for (i = 0; i < jumps; i++)
	{
		changeChildSelectionNode();
	}
}

void executeChildFunction(void)
{
	NodeFunction fp = (currentNode->child)->entryFunction;
	if (fp == NULL)
	{
		return;
	}

	/* If okay, execute it */
	(*fp)();
}

void executeOwnFunction(void)
{
	NodeFunction fp = currentNode->ownFunction;
	if (fp == NULL)
	{
		return;
	}

	/* If okay, execute it */
	(*fp)();
}

// tmr: list or extra nodes for set limits, make strings match nodes, use flags

/*
void stringToRam(static char rom *source, static char *destination)
{
	while ((*destination++ = *source++) != NULL){}
}
*/

void interfaceSetup(void)
{
	treeNodeLabelSetup();
	treeStructureSetup();
	updateTreeStructure();
	linkNodeFunctions();
}

// tmr: list or extra nodes for set limits, make strings match nodes, use flags

void stringToRam( static char rom *source, static char *destination )
{
    while( (*destination++ = *source++) != NULL ){}
}
