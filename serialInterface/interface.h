/*
* File:   interface.h
* Author: kkeke_000
*
* Created on 16 October 2014, 9:14 PM
*/

#ifndef INTERFACE_H
#define	INTERFACE_H

/* For LCD Strings */
enum{
	SYSTEM_NAME,
	PRESS_GO,
	MAIN_MENU,
	TARGET_STATUS,
	TEMPERATURE,
	GOTO_POSITION,
	SET_LIMITS,
	REMOTE_MODE,
	NOT_FOUND,
	SEARCHING,
	ACQUIRED,
	DIST,
	AZIM,
	ELEV,
	AZIMUTH,
	ELEVATION,
	MANUALLY,
	SET,
	GOTO,
	MANUAL_MOVE,
	USE_ARROWS,
	ANGLE,
	SUCCESS,
	OUT_OF_RANGE,
	RE_ENTER,
	DIST_MIN,
	DIST_MAX,
	AZIM_MIN,
	AZIM_MAX,
	ELEV_MIN,
	ELEV_MAX,
	VALUE,
	TOTAL_STRINGS
};

extern rom const char rom *interfaceString[];                 ///< Declaring all of the strings in program memory


typedef struct Node Node;

/* For tree node labels */
enum{
	/* User Mode */
	HOME_NODE,
	TARGET_STATUS_NODE,
	TEMPERATURE_NODE,
	GOTO_POSITION_NODE, /* Select Azim or Elev */
	SET_LIMITS_NODE,
	MODE_SWITCH_NODE,

	/* Under GOTO_POSITION_NODE */
	GOTO_METHOD_SELECT_NODE,
	MANUAL_CONTROL_NODE,
	SET_ANGLE_CONTROL_NODE,

	/* Under SET_LIMITS_NODE */
	ENTER_LIMITS_NODE,

	/* Under end of both GOTO_POSITION_NODE and SET_LIMITS_NODE */
	OUT_OF_RANGE_NODE,
	ENTRY_SUCCESS_NODE, /* Child is always home */

	/* Factory Mode */
	CALIBRATION_NODE,
	SET_US_NODE,
	SET_IR_NODE,
	SHOW_RAW_NODE,
	SHOW_STAT_NODE,

	/* Under CALIBRATION_NODE */
	CALI_TEMP_NODE,
	CALI_AZIM_NODE,
	CALI_ELEV_NODE,

	/* Under both SET_US_NODE and SET_IR_NODE */
	SAMPLE_PER_EST_NODE,
	SAMPLE_RATE_NODE,

        TOTAL_NODE_LABELS
};

struct Node {

	char label;
	Node *parent;
	Node *child;    /* child is the only thing that changes */
	Node *sibling;
};

void treeNodeLabelSetup(void);
void treeStructureSetup(void);
void updateTreeStructure(void);
void moveToParentNode(void);
void moveToChildNode(void);
void changeChildSelectionNode(void);


#endif	/* INTERFACE_H */

