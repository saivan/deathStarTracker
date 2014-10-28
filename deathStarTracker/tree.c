
#include "masterHeader.h"

Node rootNode = {NULL, NULL, NULL, NULL, NULL};

Node targetStatusNode = {NULL, NULL, NULL, NULL, NULL};
Node temperatureNode = {NULL, NULL, NULL, NULL, NULL};
Node gotoPositionNode = {NULL, NULL, NULL, NULL, NULL};
Node setLimitsNode = {NULL, NULL, NULL, NULL, NULL};
Node modeSwitchNode = {NULL, NULL, NULL, NULL, NULL};

Node azimNode = {NULL, NULL, NULL, NULL, NULL};
Node elevNode = {NULL, NULL, NULL, NULL, NULL};

Node azimManualNode = {NULL, NULL, NULL, NULL, NULL};
Node azimSetAngleNode = {NULL, NULL, NULL, NULL, NULL};

Node elevManualNode = {NULL, NULL, NULL, NULL, NULL};
Node elevSetAngleNode = {NULL, NULL, NULL, NULL, NULL};

Node distMaxNode = {NULL, NULL, NULL, NULL, NULL};
Node distMinNode = {NULL, NULL, NULL, NULL, NULL};
Node azimMaxNode = {NULL, NULL, NULL, NULL, NULL};
Node azimMinNode = {NULL, NULL, NULL, NULL, NULL};
Node elevMaxNode = {NULL, NULL, NULL, NULL, NULL};
Node elevMinNode = {NULL, NULL, NULL, NULL, NULL};

//Node outOfRangeNode = {NULL, NULL, NULL, NULL, NULL};
//Node entrySuccessNode = {NULL, NULL, NULL, NULL, NULL};

Node calibrationNode = {NULL, NULL, NULL, NULL, NULL};
Node setUSNode = {NULL, NULL, NULL, NULL, NULL};
Node setIRNode = {NULL, NULL, NULL, NULL, NULL};
Node showRawNode = {NULL, NULL, NULL, NULL, NULL};
Node showStatNode = {NULL, NULL, NULL, NULL, NULL};

Node caliTempNode = {NULL, NULL, NULL, NULL, NULL};
Node caliAzimNode = {NULL, NULL, NULL, NULL, NULL};
Node caliElevNode = {NULL, NULL, NULL, NULL, NULL};

Node usSamplePerEstNode = {NULL, NULL, NULL, NULL, NULL};
Node usSampleRateNode = {NULL, NULL, NULL, NULL, NULL};

Node irSamplePerEstNode = {NULL, NULL, NULL, NULL, NULL};
Node irSampleRateNode = {NULL, NULL, NULL, NULL, NULL};

Node* currentNode = &rootNode;
Node* previousNode = &rootNode;

rom char rom *nodeNames[] = {
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

//	/* Under end of both GOTO_POSITION_NODE and SET_LIMITS_NODE */
//	"Out of Range", //OUT_OF_RANGE_NODE,
//	"Entry Success", //ENTRY_SUCCESS_NODE, /* Child is always home */

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

//	outOfRangeNode.label = OUT_OF_RANGE_NODE;
//	entrySuccessNode.label = ENTRY_SUCCESS_NODE;

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
void treeNodeStructureSetup(void)
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

//	azimSetAngleNode.child = &outOfRangeNode;
//	elevSetAngleNode.child = &outOfRangeNode;

	/* Set Limits */
	setLimitsNode.child = &distMaxNode;

	distMaxNode.sibling = &distMinNode;
	distMinNode.sibling = &azimMaxNode;
	azimMaxNode.sibling = &azimMinNode;
	azimMinNode.sibling = &elevMaxNode;
	elevMaxNode.sibling = &elevMinNode;
	elevMinNode.sibling = &distMaxNode;

//	distMaxNode.child = &outOfRangeNode;
//	distMinNode.child = &outOfRangeNode;
//	azimMaxNode.child = &outOfRangeNode;
//	azimMinNode.child = &outOfRangeNode;
//	elevMaxNode.child = &outOfRangeNode;
//	elevMinNode.child = &outOfRangeNode;
//
//	/* At the end of Goto Position and Set Limits Subtree */
//	outOfRangeNode.sibling = &entrySuccessNode;
//	entrySuccessNode.sibling = &outOfRangeNode;

	/* Factory */

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

void treeNodeFunctionSetup(void)
{
    targetStatusNode.stateFunction = &showTargetStatus;
    temperatureNode.stateFunction = &showTemperature;
    azimSetAngleNode.stateFunction = &azimSetAngle;
    azimManualNode.stateFunction = &azimManual;
    elevSetAngleNode.stateFunction = &elevSetAngle;
    elevManualNode.stateFunction = &elevManual;
    distMaxNode.stateFunction = &distMax;
    distMinNode.stateFunction = &distMin;
    azimMaxNode.stateFunction = &azimMax;
    azimMinNode.stateFunction = &azimMin;
    elevMaxNode.stateFunction = &elevMax;
    elevMinNode.stateFunction = &elevMin;
    caliTempNode.stateFunction = &caliTemp;
    caliAzimNode.stateFunction = &caliAzim;
    caliElevNode.stateFunction = &caliElev;
    usSamplePerEstNode.stateFunction = &usSamplePerEst;
    usSampleRateNode.stateFunction = &usSampleRate;
    irSamplePerEstNode.stateFunction = &irSamplePerEst;
    irSampleRateNode.stateFunction = &irSampleRate;
    showRawNode.stateFunction = &showRaw;
    showStatNode.stateFunction = &showStat;
    modeSwitchNode.stateFunction = &modeSwitch;

    // remember to set systemFlags.numberInput = 1; in modes with number inputs
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

void treeSetup(void)
{
	treeNodeLabelSetup();
	treeNodeStructureSetup();
	treeNodeFunctionSetup();
}