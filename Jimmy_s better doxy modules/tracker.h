/* 
 * File:   tracker.h
 * Author: Jimmy
 *
 * This file utilises the infrared, ultrasound, and servo modules in order to
 * scan for, and follow a moving target within the field of view of the tracking
 * hardware.
 * 
 * Created on October 25, 2014, 10:00 PM
 */

#ifndef TRACKER_H
#define	TRACKER_H

#define CHECK_STATIONARY 0
#define CHECK_NEIGHBOURS 1
#define CALCULATE_NEXT_POSITION 0
#define SENSOR_MEASUREMENT 1

#define AZIM 0
#define ELEV 1

///Function Declarations
void track(void);
void scan(void);
void follow(void);
void getDistance(void);

/// Collection of the flags used during tracking
typedef struct trackFlagType{
    unsigned char targetFound : 1;  /// Whether the target is found
    ///Accuracy of each module's distance reading, 1-> accurate, 0-> inaccurate
    unsigned char IRAccuracy : 1;
    unsigned char USAccuracy : 1;
    /// Whether getDistance has a distance estimate ready to be used
    unsigned char distanceReady : 1;
}trackFlagType;

extern trackFlagType trackFlags;    ///< Flags for the tracking module
extern unsigned int distance;       ///< Current estimated distance of target
extern int lastKnownAzimuth;   ///< Azimuth of the last known location of target
extern int lastKnownElevation; ///< Elevation of last known location of target
extern unsigned int lastKnownDistance;  ///< Last known distance of the target
/// Size of angle step used in tracking (in degrees)
extern unsigned int stepDegrees;

#endif	/* TRACKER_H */

