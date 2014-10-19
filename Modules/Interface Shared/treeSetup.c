#include "system.h"

Node rootNode;

Node targetStatusNode;
Node temperatureNode;
Node gotoPositionNode;
Node setLimitsNode;
Node modeSwitchNode;

Node azimNode;
Node elevNode;

Node azimManualNode;
Node azimSetAngleNode;

Node elevManualNode;
Node elevSetAngleNode;

Node distMaxNode;
Node distMinNode;
Node azimMaxNode;
Node azimMinNode;
Node elevMaxNode;
Node elevMinNode;

Node outOfRangeNode;
Node entrySuccessNode;

extern Node calibrationNode;
extern Node setUSNode;
extern Node setIRNode;
extern Node showRawNode;
extern Node showStatNode;

extern Node caliTempNode;
extern Node caliAzimNode;
extern Node caliElevNode;

extern Node usSamplePerEstNode;
extern Node usSampleRateNode;

extern Node irSamplePerEstNode;
extern Node irSampleRateNode;

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

	azimNode.label = AZIM_METHOD_SELECT_NODE;
	elevNode.label = ELEV_METHOD_SELECT_NODE;

	azimManualNode.label = AZIM_MANUAL_CONTROL_NODE;
	azimSetAngleNode.label = AZIM_SET_ANGLE_CONTROL_NODE;

	elevManualNode.label = ELEV_MANUAL_CONTROL_NODE;
	elevSetAngleNode.label = ELEV_SET_ANGLE_CONTROL_NODE;

	distMaxNode.label = DIST_MAX_NODE;
	distMinNode.label = DIST_MIN_NODE;
	azimMaxNode.label = AZIM_MAX_NODE;
	azimMinNode.label = AZIM_MIN_NODE;
	elevMaxNode.label = ELEV_MAX_NODE;
	elevMinNode.label = ELEV_MIN_NODE;

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

	usSamplePerEstNode.label = US_SAMPLE_PER_EST_NODE;
	usSampleRateNode.label = US_SAMPLE_RATE_NODE;

	irSamplePerEstNode.label = IR_SAMPLE_PER_EST_NODE;
	irSampleRateNode.label = IR_SAMPLE_RATE_NODE;
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
	gotoPositionNode.child = &azimNode;

	azimNode.sibling = &elevNode;
	elevNode.sibling = &azimNode;

	azimNode.child = &azimSetAngleNode;

	azimSetAngleNode.sibling = &azimManualNode;
	azimManualNode.sibling = &azimSetAngleNode;

	elevNode.child = &elevSetAngleNode;

	elevSetAngleNode.sibling = &elevManualNode;
	elevManualNode.sibling = &elevSetAngleNode;

	azimSetAngleNode.child = &outOfRangeNode;
	elevSetAngleNode.child = &outOfRangeNode;

	/* Set Limits */
	setLimitsNode.child = &distMaxNode;

	distMaxNode.sibling = &distMinNode;
	distMinNode.sibling = &azimMaxNode;
	azimMaxNode.sibling = &azimMinNode;
	azimMinNode.sibling = &elevMaxNode;
	elevMaxNode.sibling = &elevMinNode;
	elevMinNode.sibling = &distMaxNode;

	distMaxNode.child = &outOfRangeNode;
	distMinNode.child = &outOfRangeNode;
	azimMaxNode.child = &outOfRangeNode;
	azimMinNode.child = &outOfRangeNode;
	elevMaxNode.child = &outOfRangeNode;
	elevMinNode.child = &outOfRangeNode;

	/* At the end of Goto Position and Set Limits Subtree */
	outOfRangeNode.sibling = &entrySuccessNode;
	entrySuccessNode.sibling = &outOfRangeNode;

	/* Factory */

	modeSwitchNode.child = &rootNode;

	/* Calibration */
	calibrationNode.child = &caliTempNode;

	caliTempNode.sibling = &caliAzimNode;
	caliAzimNode.sibling = &caliElevNode;
	caliElevNode.sibling = &caliTempNode;

	/* Set US */
	setUSNode.child = &usSamplePerEstNode;

	/* Set IR */
	setIRNode.child = &irSamplePerEstNode;

	usSamplePerEstNode.sibling = &usSampleRateNode;
	usSampleRateNode.sibling = &usSamplePerEstNode;

	irSamplePerEstNode.sibling = &irSampleRateNode;
	irSampleRateNode.sibling = &irSamplePerEstNode;
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

void linkNodeFunctions(void)
{
	rootNode.ownFunction = &showChildOptions;
	targetStatusNode.ownFunction = &showTargetStatus;
	temperatureNode.ownFunction = &showTemperature;
	gotoPositionNode.ownFunction = &showChildOptions;
	modeSwitchNode.ownFunction = &toggleRemoteMode;
}