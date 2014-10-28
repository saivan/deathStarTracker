/*
 * File:   tracker.c
 * Author: Jimmy
 *
 * Created on October 25, 2014, 10:00 PM
 */

#include "masterHeader.h"

trackFlagType trackFlags = {0,0,0,0};
timeTag servoFinished = {0,0,0,0};
unsigned int distance = 0;
int nextScanAzimuth = 0;
int nextScanElevation = 0;
int deltaScanAzimuth = 50;
int deltaScanElevation = 50;
int lastKnownAzimuth = 0;
int lastKnownElevation = 0;

#define CHECK_STATIONARY 0
#define CHECK_NEIGHBOURS 1
#define CALCULATE_NEXT_POSITION 0
#define IR_MEASUREMENT 1

#define AZIM 0
#define ELEV 1
#define A 40
#define B 30
int trackPositions[8][2] = { { A, 0 }, { B, B }, { 0, A }, { -B, B }, { -A, 0 }, { -B, -B }, { 0, -A }, { B, -B } };
int currentMinimum = 0;
unsigned char lowestIndex = 0;

void track(void){

   if(trackFlags.targetFound){
       follow();
   } else {
        scan();
   }

}

/**
 * @brief scans for the target
 *
 *
 */
void scan(void){

    if (eventDue(&servoFinished)){
        if ( trackFlags.distanceReady ){

            nextScanAzimuth = currentAzimuth + deltaScanAzimuth;
            if (nextScanAzimuth > azimuthMax || nextScanAzimuth < azimuthMin){
                deltaScanAzimuth = -deltaScanAzimuth;
                nextScanAzimuth += deltaScanAzimuth*2;

                nextScanElevation = currentElevation + deltaScanElevation;
                if (nextScanElevation > elevationMax || nextScanElevation < elevationMin){
                    deltaScanElevation = -deltaScanElevation;
                    nextScanElevation += deltaScanElevation*2;
                }
            }

            updateCCPServoAngle(nextScanAzimuth,nextScanElevation);
            setTimeTag(20,&servoFinished);

            trackFlags.distanceReady = 0;
            //@todo think about this stuff

        } else {
            getDistance();           
        }
        return;
    }

//    if (IRFlags.idle){
//        getNewIRDistance();
//    }
//
//    IRSensor();
}

/**
 * @brief follows the target and stores the distance 
 *
 */
void follow(void){
    static unsigned char followState = 0;
    static unsigned char subFollowState = 0;
    static int lastIndex = 0; 
    static int depth;
    static int dir;
    static int point = 0;

    

    /// Setup
    if( followState == CHECK_STATIONARY ){               
        /// Run a measurement if the IR is not busy
        if (IRFlags.idle){
            getNewIRDistance();
        }        
        IRSensor();

        if ( IRFlags.distanceReady ){                        ///< IR sensor has a new distance to process
            if ( IRDistance != 0 ) {                         ///< If we see the ball, stay where we are
                getDistance();                
                return;                                 
            } else {          
                lastKnownAzimuth = currentAzimuth;           ///< Store the current position
                lastKnownElevation = currentElevation;       ///< Store the current elevation         
                followState = CHECK_NEIGHBOURS;         ///< If we missed the ball, check our surroundings
                depth = 1;                              ///< Start searching nearest neighbours                
                point = 0;                              ///< Restart point
            }
        } else {
            getDistance();
            return;                                     ///< If we haven't got a distance yet, we exit
        }        
    } else if ( followState == CHECK_NEIGHBOURS ){

        if ( depth < 3 ){
            if ( point < 8 ){
                if( subFollowState == CALCULATE_NEXT_POSITION ){               ///< Calculate a new position
                    dir = ( lastIndex + point ) & 0x07;                        ///< Calculate which direction to move to 
                    updateCCPServoAngle( depth*trackPositions[dir][AZIM] + lastKnownAzimuth, depth*trackPositions[dir][ELEV] + lastKnownElevation);    ///< Move the servo to the next position
                    setTimeTag( 30, &servoFinished );                          ///< Give the servo 10ms to move
                    subFollowState = IR_MEASUREMENT;                           ///< Move to purgatory
                } else if( subFollowState == IR_MEASUREMENT ){ 
                    if( eventDue( &servoFinished )){                            ///< Give the servo 10ms to travel 
                        if( IRFlags.idle ){                                    ///< The IR sensor is not busy
                            getNewIRDistance();
                        } 
                        IRSensor();
                        if ( IRFlags.distanceReady ){                          ///< A new measurement is available
                            if( IRDistance ){                                  ///< Check if the IRSensor saw something
                                lastIndex = dir;                               ///< Store dir as the new lastIndex
                                followState = CHECK_STATIONARY;                ///< Move back to CHECK_STATIONARY 
                                lastKnownAzimuth = currentAzimuth;             ///< We have found the ball, so store its position
                                lastKnownElevation = currentElevation;
                            } else {                                           ///< The ball is not here
                                point++;                                       ///< We check the next point
                                subFollowState = CALCULATE_NEXT_POSITION;      ///< We calculate the next servo place
                            }
                        }
                    }   
                }                     
            } else {
                point = 0;      ///< We start itterating again
                depth++;        ///< We move out further
            }
        } else {                                                                ///< The ball is completely lost
            clearUSSamplePerEstimate();                                         ///< Move to single sample to find
            trackFlags.targetFound = 0;                                         ///< We lost the ball
            depth = 1;                                                          ///< Reset depth for next search
        }
    }

    /// Update the distance at the prespecified rate
    //getDistance();
//    if (eventDue(&echoCanFire)){
//        fireEcho();
//    }
//    testUSState();
//    if ( USFlags.distanceReady ){
//        distance = USValues.distance;
//        USFlags.distanceReady = 0;
//        trackFlags.distanceReady = 1;
//    }
}

/**
 * @todo test
 */
void getDistance(void){

    if (eventDue(&echoCanFire)){
        fireEcho();
    }
    if (IRFlags.idle){
        getNewIRDistance();
    }

    testUSState();
    IRSensor();

    if(USFlags.distanceReady && IRFlags.distanceReady){
        trackFlags.IRAccuracy = 1; ///@todo change these to bits
        trackFlags.USAccuracy = 1;
        if (IRDistance < 300 || IRDistance > 800){
            trackFlags.IRAccuracy = 0;
        }
        if (USValues.distance < 400){
            trackFlags.USAccuracy = 0;
        }        

        distance = trackFlags.USAccuracy*USValues.distance + trackFlags.IRAccuracy*IRDistance;

        if (trackFlags.USAccuracy == trackFlags.IRAccuracy){
            distance = distance>>1;
        }
        IRFlags.distanceReady = 0;
         USFlags.distanceReady = 0;
         trackFlags.distanceReady = 1;

        if (distance!=0){
            trackFlags.targetFound = 1;
            lastKnownAzimuth = currentAzimuth;
            lastKnownElevation = currentElevation;
            USValues.currentSampleSize = USValues.sampleSize;       /// Sets sample rate to user defined rate
        } 

    }
}