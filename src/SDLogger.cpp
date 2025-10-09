#include "SDLogger.h"

// Constructor (with other classes passed in for logging elements)
LoggingClass::LoggingClass( )
	{ }



/**
 * @brief Initialize class
 * 
 */
void LoggingClass::Begin() {
	InitializeSdCard();

	delay( 250 );
	Serial.println( F( "Logging system initialization...              Success!" ) );
}

/**
 * @brief Initialize the SD card and warn if not found
 */
void LoggingClass::InitializeSdCard() {



	// Make sure SD card is present
	if ( !SD.begin( BUILTIN_SDCARD ) ) {
		Serial.println( F( "SD card initialization...                     FAILED! Check if SD card is present." ) );
		isSdCardPresent = false;
	} else {
		Serial.println( F( "SD card initialization...                     success!" ) );
		isSdCardPresent = true;
	}
}



void LoggingClass::Update() {

	// Display output if timer ticks over
	if ( timerRuntimeMillis >= ( 1000 / timerLoggingFrequency ) ) {

		// Reset timer
		timerRuntimeMillis = 0;

		// Log data by adding to vector
	}
}


void LoggingClass::LogDiscriminationTaskEntry() {

	// Task logging not started, initialize and populate header
	if ( !isLoggingStarted ) {

		// Clear vector
		vectorDiscriminationTask.clear();

		// Populate header
		vectorDiscriminationTask.push_back( "TaskTime[ms] ,CueType[dir] ,CueTime[ms] ,UserResponseType[dir] ,UserResponseTime[ms] " );

		// Update flag
		isLoggingStarted = true;

		// Reset timer
		timerTaskRuntimeMillis = 0;
	}
	// Task logging already started, populate
	else {
		String newEntry = String( timerTaskRuntimeMillis ) + "," + String( presentedCue ) + "," + String( presentedCueStartTime ) + "," + String( userCueResponse ) + "," + String( userCueResponseTime );
		vectorDiscriminationTask.push_back( newEntry );
	}
}
