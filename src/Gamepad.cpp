#include "Gamepad.h"

/**
 * @brief Construct a new Gamepad Class:: Gamepad Class object
 * 
 */
GamepadClass::GamepadClass() { }


/**
 * @brief Begin an initialize class
 * 
 */
void GamepadClass::Begin() {

	// Initialize hardware IO
	ConfigurePins();

	// Initialize threshold array values
	InitializeThresholdArrays();

	delay( 250 );
	Serial.println( F( "Platform gamepad initialization...            Success!" ) );
}


/**
 * @brief Called by main
 */
void GamepadClass::Check( bool& isPressed, int8_t& buttonIndex, String& buttonName ) {

	// Poll buttons and process analog data
	PollButtons();

	// Update button index and name
	isPressed	= isNewStateReady;
	buttonIndex = combinedStateValue;
	buttonName	= combinedStateString;
}

/**
 * @brief Return the cardinal direction, clockwise from Up
 * 
 */
int8_t GamepadClass::GetCardinalState() {

	return cardinalStateValue;
}

/**
 * @brief Return the cardinal direction string name, clockwise from Up
 * 
 */
String GamepadClass::GetCardinalStateString() {

	return cardinalStateString;
}


/**
 * @brief Return the diagonal direction, clockwise from Up+Right
 * 
 */
int8_t GamepadClass::GetDiagonalState() {

	return diagonalStateValue;
}

/**
 * @brief Return the diagonal direction string name, clockwise from Up+Right
 * 
 */
String GamepadClass::GetDiagonalStateString() {

	return diagonalStateString;
}



/**
 * @brief Return from all possible directions, clockwise from Up
 * 
 */
int8_t GamepadClass::GetCombinedState() {

	return combinedStateValue;
}


/**
 * @brief Return name string from all possible directions, clockwise from Up
 * 
 */
String GamepadClass::GetCombinedStateString() {

	return combinedStateString;
}



/**
 * @brief Return the button state
 * 
 */
int8_t GamepadClass::GetButtonState() {

	return buttonStateValue;
}

/**
 * @brief Return the button state string name
 * 
 */
String GamepadClass::GetButtonStateString() {

	return buttonStateString;
}


/**
 * @brief Initialize hardware pins
 * 
 */
void GamepadClass::ConfigurePins() {

	// Initialize the pins on the Teensy
	pinMode( PIN_GAMEPAD_CARDINAL, INPUT );
	pinMode( PIN_GAMEPAD_DIAGONAL, INPUT );
	pinMode( PIN_GAMEPAD_BUTTONS, INPUT );
}



void GamepadClass::InitializeThresholdArrays() {

	// Initialize direction threshold min/max arrays
	for ( int i = 0; i < 4; i++ ) {
		cardinalThresholdMinArray[i] = cardinalThresholdArray[i] - ( cardinalThresholdArray[i] * analogReadTolerance );
		cardinalThresholdMaxArray[i] = cardinalThresholdArray[i] + ( cardinalThresholdArray[i] * analogReadTolerance );
		diagonalThresholdMinArray[i] = diagonalThresholdArray[i] - ( diagonalThresholdArray[i] * analogReadTolerance );
		diagonalThresholdMaxArray[i] = diagonalThresholdArray[i] + ( diagonalThresholdArray[i] * analogReadTolerance );
	}

	// Initialize button threshold min/max arrays
	for ( int j = 0; j < 2; j++ ) {
		buttonThresholdMinArray[j] = buttonThresholdArray[j] - ( buttonThresholdArray[j] * analogReadTolerance );
		buttonThresholdMaxArray[j] = buttonThresholdArray[j] + ( buttonThresholdArray[j] * analogReadTolerance );
	}
}



/**
 * @brief Maps the analog button values into their outputs
 * 
 */
void GamepadClass::MapButtonValues() {

	// Reset state values and strings
	cardinalStateValue	= -1;
	diagonalStateValue	= -1;
	buttonStateValue	= -1;
	combinedStateValue	= -1;
	cardinalStateString = "";
	diagonalStateString = "";
	buttonStateString	= "";
	combinedStateString = "";



	// Iterate over cardinal directions
	for ( int i = 0; i < 4; i++ ) {

		// Check if raw value within range
		if ( cardinalRawValue >= cardinalThresholdMinArray[i] && cardinalRawValue <= cardinalThresholdMaxArray[i] ) {
			cardinalStateValue	= cardinalIndexValueArray[i];
			cardinalStateString = cardinalStringArray[i];
		}
	}

	// Iterate over diagonal directions
	for ( int j = 0; j < 4; j++ ) {

		// Check if raw value within range
		if ( diagonalRawValue >= diagonalThresholdMinArray[j] && diagonalRawValue <= diagonalThresholdMaxArray[j] ) {
			diagonalStateValue	= diagonalIndexValueArray[j];
			diagonalStateString = diagonalStringArray[j];
		}
	}

	// Iterate over option buttons
	for ( int k = 0; k < 2; k++ ) {

		// Check if raw value within range
		if ( buttonRawValue >= buttonThresholdMinArray[k] && buttonRawValue <= buttonThresholdMaxArray[k] ) {
			buttonStateValue  = buttonIndexValueArray[k];
			buttonStateString = buttonStringArray[k];
		}
	}

	// Map combined values
	if ( cardinalStateValue != -1 ) {
		combinedStateValue	= cardinalStateValue * 2;
		combinedStateString = cardinalStateString;
	} else if ( diagonalStateValue != -1 ) {
		combinedStateValue	= diagonalStateValue * 2 + 1;
		combinedStateString = diagonalStateString;
	} else if ( buttonStateValue != -1 ) {
		combinedStateValue	= buttonStateValue + 8;
		combinedStateString = buttonStateString;
	} else {
		combinedStateValue	= -1;
		combinedStateString = "";
	}

	// Debounce check
	static int8_t lastRawState	  = -1;
	int8_t		  currentRawState = combinedStateValue;

	isNewStateReady = false;

	if ( currentRawState == lastRawState ) {
		debounceCounter++;
		if ( debounceCounter >= debounceLimit ) {

			// Stable reading
			if ( currentRawState != lastStableState ) {
				lastStableState = currentRawState;

				// Edge detection
				if ( lastStableState != -1 ) {
					
					combinedStateValue = lastStableState;
					isNewStateReady	   = true;
					// Serial.print( "IN CLASS Button: " );
					// Serial.print( combinedStateValue );
					// Serial.print( " , " );
					// Serial.println( combinedStateString );
				} else {
				}
			}
		}
	} else {
		debounceCounter = 0;
	}
	lastRawState = currentRawState;
}


/**
 * @brief Poll buttons using analog reads
 * 
 */
void GamepadClass::PollButtons() {

	// Read button analog values
	cardinalRawValue = analogRead( PIN_GAMEPAD_CARDINAL );
	diagonalRawValue = analogRead( PIN_GAMEPAD_DIAGONAL );
	buttonRawValue	 = analogRead( PIN_GAMEPAD_BUTTONS );

	// Map readings into button values
	MapButtonValues();
}