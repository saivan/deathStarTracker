/*
 * File:   tracker.c
 * Author: Jimmy
 *
 * Created on October 25, 2014, 10:00 PM
 */

#include "masterHeader.h"

trackFlagType trackFlags = {0,0,0,0};

void track(void){

    if(trackFlags.targetFound){
        follow();
    } else {
        scan();
    }

}

void scan(void);
void follow(void);

/**
 * @todo test
 */
void getDistance(void){

    if (eventdue(&echoCanFire)){
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
        if (USDistance < 40){
            trackFlags.USAccuracy = 0;
        }

        distance = trackFlags.USAccuracy*USDistance + trackFlags.IRAccuracy*IRDistance;

        if (trackFlags.USAccuracy == trackFlags.IRAccuracy){
            distance = distance>>1;
        }
        USFlags.distanceReady = 0;
        trackFlags.distanceReady = 1;

    }
}