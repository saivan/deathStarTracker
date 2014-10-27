
#include "masterHeader.h"

/**
 * @brief Handles the servos, timer and 
 * @details 
 * 
 */
#pragma interrupt highPriorityISR
void highPriorityISR( void ){    
	/// Handling the servos 
    if( PIR1bits.CCP1IF ){
         PIR1bits.CCP1IF = 0;                    	///< Clear the flag for the next interrupt
         if( ~servoFlags.azimuthFired ){	
             AZIMUTH_PIN = 1;                    	///< Turn on the azimuth servo
             CCPR1 = azimuthServoOnTime;         	///< Set CCP1 to the Azimuth on time
             servoFlags.azimuthFired = 1;        	///< Note that the azimuth servo was triggered
         } else if ( ~servoFlags.elevationFired ){
             AZIMUTH_PIN = 0;                    	///< Turn off the Azimuth servo
             ELEVATION_PIN = 1;                  	///< Turn on the elevation servo
             CCPR1 = elevationServoOnTime;       	///< Set CCP1 to the Elevation on time
             servoFlags.elevationFired = 1;      	///< Note that the elevation servo was triggered
         } else {	
             ELEVATION_PIN = 0;                  	///< Turn off the Elevation servo
             CCPR1 = servoOffTime;               	///< Move the off time to the CCP1
             servoFlags.azimuthFired = 0;        	///< Restart the azimuth cycle
             servoFlags.elevationFired = 0;      	///< Restart the elevation cycle
         }
    /// Handling the timer
    } else if( INTCONbits.TMR0IF ){  
        time.updatesRequired++;                     ///< Flag another update for the main
        INTCONbits.TMR0IF = 0;                      ///< Clear the interrupt flag
    }
    /// Handing the 
}


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param d [description]
 */

void lowPriorityISR( void ){

}




