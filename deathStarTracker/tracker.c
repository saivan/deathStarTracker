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
int lastKnownElevation =0;

int trackPositions[8][2] = { { 2*A, 0 }, { B, 2*B }, { 0, A }, { -2*B, B }, { -2*A, 0 }, { -2*B, -B }, { 0, -A }, { 2*B, -B } };


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

    if (IRFlags.idle){
        getNewIRDistance();
    }

    IRSensor();
}

/**
 * @brief follows the target and stores the distance 
 *
 */
void follow(void){
    /// Program state variables
    static unsigned char followState = 0;
    static unsigned char subFollowState = 0;
    static unsigned char gotNewLow = 0;
    static unsigned char ballStill = 0;
    /// Index references
    static int lastIndex = 0; 
    static int lowestIndex = 0;
    static int dir;
    /// Itteration variables to move through possibilities
    static int currentMinimum = 0;
    static int point = 0;
    static int depth;

    trackFlags.targetFound = 0;
    return;

    /// Setup a new measurement and check if the ball is still where it last was
    if( followState == CHECK_STATIONARY ){               
        /// Run a measurement to find a new minimum
        if( eventDue( &servoFinished )){                     ///< Give the servo 10ms to travel                         
            getDistance();
            if ( USFlags.distanceReady ){
                if( ballStill ){
                    ballStill = FALSE;                           ///< IF the ball was still, discard the last reading
                    USFlags.distanceReady = FALSE;
                } else {
                    if( USValues.distance ){
                        currentMinimum = USValues.distance;          ///< The current position of the ball is given
                    } else {
                        currentMinimum = USValues.maxRange; 
                    }
                    lastKnownAzimuth = currentAzimuth;           ///< Store the current position
                    lastKnownElevation = currentElevation;       ///< Store the current elevation         
                    followState = CHECK_NEIGHBOURS;              ///< If we missed the ball, check our surroundings                
                    depth = 1;                                   ///< Start searching nearest neighbours                
                    point = 0;                                   ///< Restart point
                }
            } else if ( IRFlags.distanceReady ){                 ///< IR sensor has a new distance to process
                IRFlags.distanceReady = FALSE;                   ///< Start a new reading
                if ( IRDistance ) {                              ///< If we see the ball, stay where we are
                    ballStill = TRUE;                                 
                    return;                                 
                }
            } 
        }
  
    } else if ( followState == CHECK_NEIGHBOURS ){

        if ( depth < 5 ){
            if ( point < 8 ){
                if( subFollowState == CALCULATE_NEXT_POSITION ){               ///< Calculate a new position
                    dir = ( lastIndex + point ) & 0x07;                        ///< Calculate which direction to move to 
                    updateCCPServoAngle( depth*trackPositions[dir][AZIM] + lastKnownAzimuth, 
                                         depth*trackPositions[dir][ELEV] + lastKnownElevation);  ///< Move the servo to the next position
                    setTimeTag( 40, &servoFinished );                          ///< Give the servo 10ms to move
                    subFollowState = SENSOR_MEASUREMENT;                       ///< Move to purgatory
                } else if( subFollowState == SENSOR_MEASUREMENT ){ 
                    if( eventDue( &servoFinished )){                           ///< Give the servo 10ms to travel                         
                        if (eventDue(&echoCanFire))                     
                            fireEcho();
                        if (IRFlags.idle)
                            getNewIRDistance();                        
                        testUSState();
                        IRSensor();

                        if ( USFlags.distanceReady ){
                            if( (USValues.distance) && (USValues.distance < currentMinimum) ){
                                gotNewLow = TRUE;                               ///< We record the fact that we got a new low position
                                currentMinimum = USValues.distance;             ///< We got a lower ultrasound reading, so record it
                                lowestIndex = dir;                              ///< And store where it occured
                            }
                            point++;                                            ///< We check the next point
                            subFollowState = CALCULATE_NEXT_POSITION;           ///< We calculate the next servo place

                        } else if ( IRFlags.distanceReady ){                    ///< A new measurement is available
                            if( IRDistance > 30 ){                              ///< Check if the IRSensor saw something
                                lastIndex = dir;                                ///< Store dir as the new lastIndex
                                followState = CHECK_STATIONARY;                 ///< Move back to CHECK_STATIONARY 
                                lastKnownAzimuth = currentAzimuth;              ///< We have found the ball, so store its position
                                lastKnownElevation = currentElevation;
                            } else {                                            ///< The ball is not here
                                IRFlags.idle = 1;                               ///< Look again
                            }
                        }
                    }   
                }                     
            } else {
                if( gotNewLow ){                                                ///< Move to the lowest US reading if one was available
                    lastIndex = dir;                                            ///< Store dir as the new lastIndex
                    followState = CHECK_STATIONARY;                             ///< We found the ball, so check if it's still again 
                    /// Store the minimum positions as the new ball location
                    lastKnownAzimuth = depth*trackPositions[lowestIndex][AZIM] + lastKnownAzimuth;       
                    lastKnownElevation = depth*trackPositions[lowestIndex][ELEV] + lastKnownElevation;
                    /// Move the servos to that location
                    updateCCPServoAngle( lastKnownAzimuth, lastKnownElevation );
                } 
                gotNewLow = FALSE;                                          ///< We need to look in the next tier
                point = 0;                                                      ///< We start itterating again
                depth++;                                                        ///< We move out further
            }
        } else {                                                                ///< The ball is completely lost
            clearUSSamplePerEstimate();                                         ///< Move to single sample to find
            trackFlags.targetFound = 0;                                         ///< We lost the ball
            depth = 1;                                                          ///< Reset depth for next search
        }
    }
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
            USValues.currentSampleSize = USValues.sampleSize;       /// Sets sample rate to user defined rate
        } 

    }
}