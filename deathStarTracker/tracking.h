
/**
 * @brief Performs tracking by embedding other functions
 * @details The tracking wrapper uses a static incrementor (process)
 * 			to decide on which of the functions to run below.
 * 			The purpose is to stop the system from running all of these 
 * 			in one go whilst stoping any other system functions.
 * 			When a function has finished executing, the next will be called.
 */
void trackingWrapper( void ){
	static char processIncrementor = 0;
	if( processIncrementor == COLLECT_DATA ){
		// Collect enough measurement data
		// Average of eight IR measurements ( SHORT TIME )
		// Average of two US measurements ( LONG TIME )
		processIncrementor++;				///< On the next loop, we run the next process
	} else if ( processIncrementor == RUN_SENSE ){
		// Run a sense step 
			// Rank Measurements from 1-5 based on measurement agreement
			// Randomly Resample measurements with noise	
		processIncrementor++;				///< On the next loop, we run the next process
	} else if (	processIncrementor == RUN_MOTION ){
		// Run a move step ( Move data points based on current prediction )	
		// 
	} else if ( processIncrementor == DETERMINE_POSITON ){
		// Estimate new position and velocity
			// Average across all data points (an int will do)	
	}
}






