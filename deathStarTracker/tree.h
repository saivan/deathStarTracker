/* 
 * File:   treeStructure.h
 * Author: kkeke_000
 *
 * Created on 22 October 2014, 3:41 PM
 */

#ifndef TREESTRUCTURE_H
#define	TREESTRUCTURE_H

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

//	/* Under end of both GOTO_POSITION_NODE and SET_LIMITS_NODE */
//	OUT_OF_RANGE_NODE,
//	ENTRY_SUCCESS_NODE, /* Child is always home */

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

extern rom char rom *nodeNames[];

typedef struct Node Node;
typedef void(*NodeFunction)(void);

struct Node {

	char label;
	Node *parent;
	Node *child;
	Node *sibling;
	NodeFunction stateFunction;
};

extern Node  rootNode;
extern Node* currentNode;
extern Node* previousNode;

extern void updateTreeStructure(void);
extern void treeSetup(void);

#endif	/* TREESTRUCTURE_H */

