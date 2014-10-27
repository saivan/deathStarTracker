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
unsigned int nextScanAzimuth = 0;
unsigned int nextScanElevation = 0;
int deltaScanAzimuth = 50;
int deltaScanElevation = 50;


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
        if (trackFlags.distanceReady){

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
 * @brief follows the target and prints the distance reading
 *
 */
void follow(void){
    trackFlags.targetFound = 0; // placeholder
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
        if (IRDistance < 30 || IRDistance > 80){
            trackFlags.IRAccuracy = 0;
        }
        if (USValues.distance < 40){
            trackFlags.USAccuracy = 0;
        }

        distance = trackFlags.USAccuracy*USValues.distance + trackFlags.IRAccuracy*IRDistance;

        if (trackFlags.USAccuracy == trackFlags.IRAccuracy){
            distance = distance>>1;
        }
        USFlags.distanceReady = 0;
        trackFlags.distanceReady = 1;

        if (distance){
            trackFlags.targetFound = 1;
        } else {
            trackFlags.targetFound = 0;
        }

    }
}