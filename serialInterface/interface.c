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

/**
* These strings are stored in an array in program
* memory, call stringToRam to move these functions
* from program memory to data memory.
*/
rom const char rom *LCDStrings[] = {
	/*00*/ "DeathStarTracker",  /* SYSTEM_NAME */
	/*01*/ "Press <GO>",        /* PRESS_GO */
	/*02*/ "Main Menu",         /* MAIN_MENU */
	/*03*/ "Target Status",     /* TARGET_STATUS */
	/*04*/ "Temperature",       /* TEMPERATURE */
	/*05*/ "Goto Position",     /* GOTO_POSITION */
	/*06*/ "Set Limits",        /* SET_LIMITS */
	/*07*/ "Remote Mode",       /* REMOTE_MODE */
	/*08*/ "Target Not Found",  /* NOT_FOUND */
	/*09*/ "  Searching...",    /* SEARCHING */
	/*10*/ "Target Acquired",   /* ACQURIED */
	/*11*/ "Dist",              /* DIST */
	/*12*/ "Azim",              /* AZIM */
	/*13*/ "Elev",              /* ELEV */
	/*14*/ "Azimuth",           /* AZIMUTH */
	/*15*/ "Elevation",         /* ELEVATION */
	/*16*/ "Manually",          /* MANUALLY */
	/*17*/ "Set ",              /* SET */
	/*18*/ "Goto ",             /* GOTO */
	/*19*/ "Manual Move",       /* MANUAL_MOVE*/
	/*20*/ "Use Arrows",        /* USE_ARROWS */
	/*21*/ "Angle: ",           /* ANGLES */
	/*22*/ "Entry Successful",  /* SUCCESS */
	/*23*/ "Out of Range",      /* OUT_OF_RANGE */
	/*24*/ "Re-enter Input",    /* RE_ENTER */
	/*25*/ "Distance Min",      /* DIST_MIN */
	/*26*/ "Distance Max",      /* DIST_MAX */
	/*27*/ "Azimuth Min",       /* AZIM_MIN */
	/*28*/ "Azimuth Max",       /* AZIM_MAX */
	/*29*/ "Elevation Min",     /* ELEV_MIN */
	/*30*/ "Elevation Max",     /* ELEV_MAX */
	/*31*/ "Value:"             /* VALUE */
};

Node rootNode;

Node targetStatusNode;
Node temperatureNode;
Node gotoPositionNode;
Node setLimitsNode;
Node modeSwitchNode;

Node gotoMethodSelectNode;
Node manualControlNode;
Node setAngleControlNode;

Node enterLimitsNode;

Node outOfRangeNode;
Node entrySuccessNode;

Node calibrationNode;
Node setUSNode;
Node setIRNode;
Node showRawNode;
Node showStatNode;

Node caliTempNode;
Node caliAzimNode;
Node caliElevNode;

Node samplePerEstNode;
Node sampleRateNode;

Node* currentNode = &rootNode;
Node* previousNode = &rootNode;

void treeNodeLabelSetup(void)
{
	rootNode.label = HOME_NODE;
	targetStatusNode.label = TARGET_STATUS_NODE;
	temperatureNode.label = TEMPERATURE_NODE;
	gotoPositionNode.label = GOTO_POSITION_NODE;
	setLimitsNode.label = SET_LIMITS_NODE;
	modeSwitchNode.label = MODE_SWITCH_NODE;

	gotoMethodSelectNode.label = GOTO_METHOD_SELECT_NODE;
	manualControlNode.label = MANUAL_CONTROL_NODE;
	setAngleControlNode.label = SET_ANGLE_CONTROL_NODE;

	enterLimitsNode.label = ENTER_LIMITS_NODE;

	outOfRangeNode.label = OUT_OF_RANGE_NODE;
	entrySuccessNode.label = ENTRY_SUCCESS_NODE;

	calibrationNode.label = CALIBRATION_NODE;
	setUSNode.label = SET_US_NODE;
	setIRNode.label = SET_IR_NODE;
	showRawNode.label = SHOW_RAW_NODE;
	showStatNode.label = SHOW_STAT_NODE;

	caliTempNode.label = CALI_TEMP_NODE;
	caliAzimNode.label = CALI_AZIM_NODE;
	caliElevNode.label = CALI_ELEV_NODE;

	samplePerEstNode.label = SAMPLE_PER_EST_NODE;
	sampleRateNode.label = SAMPLE_RATE_NODE;
}

/* Don't need to setup parents! Just default child and fixed sibling! */
void treeStructureSetup(void)
{
	rootNode.child = &targetStatusNode;

	targetStatusNode.sibling = &temperatureNode;
	temperatureNode.sibling = &gotoPositionNode;
	gotoPositionNode.sibling = &setLimitsNode;
	setLimitsNode.sibling = &modeSwitchNode;

	/* setLimitsNode's sibling determines whether or not we can access
	* factory mode modules or not */
	updateTreeStructure();

	/* Everything else in factory mode tree structure is constant */
	calibrationNode.sibling = &setUSNode;
	setUSNode.sibling = &setIRNode;
	setIRNode.sibling = &showRawNode;
	showRawNode.sibling = &showStatNode;
	showStatNode.sibling = &modeSwitchNode;

	/* Mode switch is the last accessible module in either case */
	modeSwitchNode.sibling = &targetStatusNode;

	/* Goto Position */
	gotoPositionNode.child = &gotoMethodSelectNode;

	gotoMethodSelectNode.sibling = NULL; /* Just to make sure */
	gotoMethodSelectNode.child = &setAngleControlNode;

	manualControlNode.sibling = &setAngleControlNode;
	setAngleControlNode.sibling = &manualControlNode;

	manualControlNode.child = NULL; /* Just to make sure */
	setAngleControlNode.child = &outOfRangeNode;

	/* Set Limits */
	setLimitsNode.child = &enterLimitsNode;
	enterLimitsNode.child = &outOfRangeNode;

	/* At the end of Goto Position and Set Limits Subtree */
	outOfRangeNode.sibling = &entrySuccessNode;
	entrySuccessNode.sibling = &outOfRangeNode;

	/* Factory */

	/* Calibration */
	calibrationNode.child = &caliTempNode;

	caliTempNode.sibling = &caliAzimNode;
	caliAzimNode.sibling = &caliElevNode;
	caliElevNode.sibling = &caliTempNode;

	/* Set US */
	setUSNode.child = &samplePerEstNode;

	/* Set IR */
	setIRNode.child = &samplePerEstNode;

	/* End of Both US and IR */
	samplePerEstNode.sibling = &sampleRateNode;
	sampleRateNode.sibling = &samplePerEstNode;
}

void updateTreeStructure(void)
{
	if (systemFlags.factory)
	{
		setLimitsNode.sibling = &calibrationNode;
	}
	else
	{
		setLimitsNode.sibling = &modeSwitchNode;
	}
}

void moveToParentNode(void)
{
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
