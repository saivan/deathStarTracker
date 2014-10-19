/*
* File:   interface.h
* Author: kkeke_000
*
* Created on 16 October 2014, 9:14 PM
*/

#ifndef INTERFACE_H
#define	INTERFACE_H
enum{
	PRESS_GO,
	NOT_FOUND,
	SEARCHING,
	ACQURIED,
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
	ANGLES,
	RE_ENTER,
	DIST_MIN,
	DIST_MAX,
	AZIM_MIN,
	AZIM_MAX,
	ELEV_MIN,
	ELEV_MAX,
	VALUE,
	EXCLAIM,
	QUESTION
};
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
	AZIM_METHOD_SELECT_NODE,
	ELEV_METHOD_SELECT_NODE,

	AZIM_MANUAL_CONTROL_NODE,
	AZIM_SET_ANGLE_CONTROL_NODE,

	ELEV_MANUAL_CONTROL_NODE,
	ELEV_SET_ANGLE_CONTROL_NODE,

	/* Under SET_LIMITS_NODE */
	DIST_MAX_NODE,
	DIST_MIN_NODE,
	AZIM_MAX_NODE,
	AZIM_MIN_NODE,
	ELEV_MAX_NODE,
	ELEV_MIN_NODE,

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

	US_SAMPLE_PER_EST_NODE,
	US_SAMPLE_RATE_NODE,

	IR_SAMPLE_PER_EST_NODE,
	IR_SAMPLE_RATE_NODE,

	TOTAL_NODE_LABELS
};

typedef struct Node Node;
typedef void(*NodeFunction)(void);

struct Node {

	char label;
	Node *parent;
	Node *child;    /* child is the only thing that changes */
	Node *sibling;
	NodeFunction entryFunction;
	NodeFunction ownFunction;
};

void treeNodeLabelSetup(void);
void treeStructureSetup(void);
void updateTreeStructure(void);
void moveToParentNode(void);
void moveToChildNode(void);
void changeChildSelectionNode(void);
void jumpToSelection(char jumps);
void executeChildFunction(void);
void executeOwnFunction(void);
void linkNodeFunctions(void);
void interfaceSetup(void);
void stringToRam(static char rom *source, static char *destination);

extern Node* currentNode;
extern Node* previousNode;

#endif	/* INTERFACE_H */

