/*
 * File:   tracker.c
 * Author: Jimmy
 *
 * Created on October 25, 2014, 10:00 PM
 */

#include "masterHeader.h"
///Declaration of variables
trackFlagType trackFlags = {0,0,0,0};   ///< Flags for the tracking module
timeTag servoFinished = {0,0,0,0};  ///< When servo is next allowed to move
unsigned int distance = 0;          ///< Current estimated distance of target
int nextScanAzimuth = 0;            ///< Next azimuth in scan pattern
int nextScanElevation = 0;          ///< Next elevation in scan pattern
/// Size of azimuth increment in scanning mode (in tenths of a degree)
int deltaScanAzimuth = 50;
/// Size of azimuth increment in scanning mode (in tenths of a degree)
int deltaScanElevation = 50;
int lastKnownAzimuth = 0;  ///< Azimuth of the last known location of target
int lastKnownElevation = 0;///< Elevation of the last known location of target
unsigned int lastKnownDistance = 100;   ///< Last known distance of the target
/// Size of angle step used in tracking (in degrees)
unsigned int stepDegrees = 3;

#define CHECK_STATIONARY 0
#define CHECK_NEIGHBOURS 1
#define CALCULATE_NEXT_POSITION 0
#define IR_MEASUREMENT 1

#define AZIM 0
#define ELEV 1
#define A 10
#define B 7

/**A collection of the 8 point coordinates used in follow() expressed as
 *  relative position
 */
int trackPositions[8][2] = { { A, 0 }, { B, B }, { 0, A }, { -B, B }, { -A, 0 },
{ -B, -B }, { 0, -A }, { B, -B } };

/**
 * @brief Executes the scanning and following algorithms
 *
 * @details Whenever a target is found, track() utilizes the follow() algorithm.
 *          Otherwise it implements the scan() algorithm.
 */
void track(void){
   if(trackFlags.targetFound){
       follow();
   } else {
        scan();
   }
}

/**
 * @brief Scans for the target using a raster scan
 *
 * @details Performs a raster scan prioritizing horizontal movement. Upon
 *          arriving at the next step location getDistance will be called until
 *          an estimate is made, and then the next scan location will be
 *          calculated and moved towards.
 */
void scan(void){

    if (eventDue(&servoFinished)){  ///< If alotted servo movement time is over
        if ( trackFlags.distanceReady ){    ///< if we have a distance reading

            /// Get next azimuth for scan
            nextScanAzimuth = currentAzimuth + deltaScanAzimuth;
            /// If that point would be out of bounds
            if (nextScanAzimuth > azimuthMax || nextScanAzimuth < azimuthMin){
                /// Reverse the scanning direction and move back
                deltaScanAzimuth = -deltaScanAzimuth;
                nextScanAzimuth += deltaScanAzimuth*2;

                ///Get next elevation for scan
                nextScanElevation = currentElevation + deltaScanElevation;
                /// If that would be out of bounds
                if (nextScanElevation > elevationMax
                        || nextScanElevation < elevationMin){
                    /// Reverse the direction and move back
                    deltaScanElevation = -deltaScanElevation;
                    nextScanElevation += deltaScanElevation*2;
                }
            }

            /// Go to next location
            updateCCPServoAngle(nextScanAzimuth,nextScanElevation);
            /// Allow servos 20ms to move to the location
            setTimeTag(20,&servoFinished);

            ///Allow a new distance measurement to be made
            trackFlags.distanceReady = 0;

        } else {
            getDistance();
        }
        return;
    }
}

/**
 * @brief Follows the target and updates the last known location
 *
 * @details follow() operates in one of two states
 * CHECK_STATIONARY - check the last known location for the target
 *                  - if the target is found, remain stationary and start again
 *                  - else move to CHECK_NEIGHBOURS state
 * CHECK_NEIGHBOURS - First check in the direction that target was last moving
 *                  - If found set that position to new last known location
 *                    and return to check stationary mode
 *                  - Else move around the last known location counter-clockwise
 *                  - If found set that position to new last known location
 *                    and return to check stationary mode
 *                  - Else repeat these steps at a greater angle
 *                  - If target is not found after 2 angle increases, target
 *                    is deemed lost, return to scan mode
 * 
 * The points to check are mapped out as 8 points surrounding a center 
 * "reference" point, and are stored as relative positions in trackPositions
 *
 * Note: The size of the angle at which the CHECK_NEIGHBOURS search takes place
 * is inversely proportional to the last known distance. This is because as the
 * distance to the target changes, the angle required to subtend the same
 * perpendicular distance changes
 */
void follow(void){
    /// State variables
    static unsigned char followState = 0;
    static unsigned char subFollowState = 0;
    static int lastIndex = 0; ///< direction the target was last seen travelling
    static int depth;   ///< Depth of spiral search
    /// Relative direction in which the neighbour being searched lies
    static int dir;
    static int point = 0;   ///< Counter for ith value in current spiral

    if( followState == CHECK_STATIONARY ){
        /// Run a measurement if the IR is not busy
        if (IRFlags.idle){
            getNewIRDistance();
        }
        IRSensor();

        if ( IRFlags.distanceReady ){   ///< If IR has a new distance to process
            if ( IRDistance != 0 ) {    ///< If target seen, stay where we are
                getDistance();
                return;
            } else {
                /// Store current position
                lastKnownAzimuth = currentAzimuth;
                lastKnownElevation = currentElevation; 

                ///Change size of our search area based on distance to target
                if (lastKnownDistance >150){
                    stepDegrees = 2;
                } else if (lastKnownDistance >100){
                    stepDegrees = 3;
                } else {
                    stepDegrees = 5;
                }
                /// If target missed, check surroundings
                followState = CHECK_NEIGHBOURS;
                depth = 1;              ///< Start searching nearest neighbours
                point = 0;              ///< Reset point
            }
        } else { ///< else, we keep calculating distance and exit
            getDistance();
            return;
        }
    } else if ( followState == CHECK_NEIGHBOURS ){

        if ( depth < 3 ){
            if ( point < 8 ){
                if( subFollowState == CALCULATE_NEXT_POSITION ){
                    /// Calculate which direction to move to
                    dir = ( lastIndex + point ) & 0x07;
                    /// Move the servo to the next position
                    updateCCPServoAngle( stepDegrees*depth*
                            trackPositions[dir][AZIM] + lastKnownAzimuth
                            , stepDegrees*depth*trackPositions[dir][ELEV]
                            + lastKnownElevation);

                    /// Give the servo 80ms to move
                    setTimeTag( 80, &servoFinished );
                    subFollowState = IR_MEASUREMENT;
                } else if( subFollowState == IR_MEASUREMENT ){
                    if( eventDue( &servoFinished )){
                        /// Get a new IR distance estimate
                        if( IRFlags.idle ){
                            getNewIRDistance();
                        }
                        IRSensor();

                        if ( IRFlags.distanceReady ){
                             /// If the IRSensor saw the target
                            if( IRDistance ){
                                lastIndex = dir; ///<Record last known direction
                                followState = CHECK_STATIONARY;
                                ///< Store its position
                                lastKnownAzimuth = currentAzimuth;             
                                lastKnownElevation = currentElevation;
                            } else {  ///< The ball is not here
                                point++;  ///< Check the next point
                                /// Calculate the next servo place
                                subFollowState = CALCULATE_NEXT_POSITION;
                            }
                        }
                    }
                }
            } else {
                point = 0;      ///< We start itterating again
                depth++;        ///< We move out further
            }
        } else {                            ///< The ball is completely lost
            clearUSSamplePerEstimate(); ///< Return to single sample for scan
            trackFlags.targetFound = 0;     ///< We lost the ball
            depth = 1;                      ///< Reset depth for next search
        }
    }
}

/**
 * @brief Generates an estimated distance of the target
 *
 * @details It calls for a new estimate from both the IR and US modules, and
 *          then makes a more accurate estimate by taking both measurements into
 *          account, discarding inaccurate or invalid readings
 */
void getDistance(void){

    /// Send US pulse if able
    if (eventDue(&echoCanFire)){
        fireEcho();
    }
    /// Initialise IR if required
    if (IRFlags.idle){
        getNewIRDistance();
    }

    testUSState();  ///< Update ultrasound
    IRSensor();     ///< Update Infrared

    /// Once both modules have a reading
    if(USFlags.distanceReady && IRFlags.distanceReady){
        trackFlags.IRAccuracy = 1;
        trackFlags.USAccuracy = 1;

        /// Set IRAccuracy to low if IR reading is not in accurate range
        if (IRDistance < 300 || IRDistance > 800){
            trackFlags.IRAccuracy = 0;
        }
        /// Set USAccuracy to low if US reading is not in accurate range
        if (USValues.distance < 400){
            trackFlags.USAccuracy = 0;
        }

        /// Combine weighted distance readings
        distance = trackFlags.USAccuracy*USValues.distance
                + trackFlags.IRAccuracy*IRDistance;

        /// Average distance reading in case of equal accuracy
        if (trackFlags.USAccuracy == trackFlags.IRAccuracy){
            distance = distance>>1;
        }

        ///Reset flags
        IRFlags.distanceReady = 0;
        USFlags.distanceReady = 0;

        /// Distance estimate has been formulated
        trackFlags.distanceReady = 1;

        if (distance!=0){
            lastKnownDistance = distance;   ///< Record last known distance
            trackFlags.targetFound = 1;     ///< Declare target has been found
            /// Record last known location
            lastKnownAzimuth = currentAzimuth;
            lastKnownElevation = currentElevation;
            /// Sets sample rate to user defined rate
            USValues.currentSampleSize = USValues.sampleSize;
        }

    }
}
