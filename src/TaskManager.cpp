#include <SharedMemory.h>
#include <TaskManager.h>



/*  ============================================================================================
 *  ============================================================================================
 * 
 *   CCCCC    AAA    RRRRR   DDDDD   IIIIII  NN   NN    AAA    LL
 *  CC       AA AA   RR  RR  DD  DD    II    NNN  NN   AA AA   LL
 *  CC      AA   AA  RR  RR  DD  DD    II    NN N NN  AA   AA  LL
 *  CC      AAAAAAA  RRRRR   DD  DD    II    NN  NNN  AAAAAAA  LL
 *  CC      AA   AA  RR  RR  DD  DD    II    NN   NN  AA   AA  LL
 *  CC      AA   AA  RR  RR  DD  DD    II    NN   NN  AA   AA  LL
 *   CCCCC  AA   AA  RR  RR  DDDDD   IIIIII  NN   NN  AA   AA  LLLLLL
 *
 *  ============================================================================================
 *  ============================================================================================*/



/**
 * @brief Runs every loop
 * 
 */
void CardinalDirectionsRuntimeClass::Loop() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Select action based on task state
	switch ( Shared->Tasks.DiscriminationTask.CardinalDirections.currentState ) {

		case EnumsClass::DiscriminationTaskStateEnum::IDLE: {

			// Move state forward
			Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::STARTING;

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::STARTING: {

			// Initialize random pool
			if ( !isRandomPoolInitialized ) {
				InitializeRandomCardinalPool( Shared->Tasks.DiscriminationTask.CardinalDirections.nRepetitions );
				isRandomPoolInitialized = true;
				timeTotalTask			= millis();
			}

			// Indicate trial number
			Serial.print( F( "Trial " ) );
			Serial.print( currentTrialNumber + 1 );
			Serial.print( F( " of " ) );
			Serial.print( userResponses.size() );
			Serial.print( F( "... " ) );

			// Move state forward
			Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_DELAY;

			// Start delay timer
			timeDelayStartMs = millis();
			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_DELAY: {

			// Check if delay time has elapsed
			if ( millis() - timeDelayStartMs >= userResponses.at( currentTrialNumber ).promptDelayTimeMs ) {

				// Move to rendering prompt state
				Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::RENDERING_PROMPT;

				// Record prompt start time
				timePromptStartMs = millis();
			}
			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::RENDERING_PROMPT: {

			// Clear button pressed flag
			Shared->Interface.Gamepad.isInputWaiting = false;

			// Print prompt for now
			Serial.print( F( "Prompt: " ) );
			Serial.print( userResponses.at( currentTrialNumber ).promptString );

			// Move to waiting for response state
			Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_RESPONSE;

			// Record trial start time
			timeTrialStartMs = millis();

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_RESPONSE: {

			// Check if response has been entered
			if ( Shared->Interface.Gamepad.isInputWaiting ) {

				// Record response time
				userResponses.at( currentTrialNumber ).responseTimeMs = millis() - timePromptStartMs;

				// Record response value
				userResponses.at( currentTrialNumber ).responseVal	  = Shared->Interface.Gamepad.buttonPressed;
				userResponses.at( currentTrialNumber ).responseString = Shared->Enumerators.MapDiscriminationDirectionsToString( Shared->Interface.Gamepad.buttonPressed );

				// Check if response correct
				userResponses.at( currentTrialNumber ).isResponseCorrect = ( userResponses.at( currentTrialNumber ).responseVal == userResponses.at( currentTrialNumber ).promptVal );

				// Reset gamepad flag
				Shared->Interface.Gamepad.isInputWaiting = false;

				Serial.println( "\t\tResponse captured." );

				// Move to next state
				Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::FINISHING;
			}

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::FINISHING: {

			// Increment trial number
			currentTrialNumber++;

			// Trial set complete
			if ( currentTrialNumber >= userResponses.size() ) {

				// Record total time
				Shared->Tasks.DiscriminationTask.CardinalDirections.totalTime = millis() - timeTotalTask;

				// Print results
				PrintCardinalDirectionTable();

				// Reset flags
				isRandomPoolInitialized = false;

				// Move to idle state
				Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::IDLE;
				Shared->State.systemState										 = EnumsClass::SystemStateEnum::IDLE;
				Shared->Tasks.activeTask										 = EnumsClass::TaskSelectionEnum::NONE;

				// Update user
				Serial.print( F( "TASK MANAGER:  Cardinal discrimination task finished in " ) );
				Serial.print( float( Shared->Tasks.DiscriminationTask.CardinalDirections.totalTime / 1000.0f ), 2 );
				Serial.println( F( "s. Task complete, returning to idle." ) );


			} else {

				// Move to delay state
				Shared->Tasks.DiscriminationTask.CardinalDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::STARTING;

				// Start delay timer
				timeDelayStartMs = millis();
			}

			// Finish tests
			//if (currenttrialnum > nReps... )
			break;
		}
	}
}



/**
 * @brief Initialize the random cardinal directions pool
 * 
 */
void CardinalDirectionsRuntimeClass::InitializeRandomCardinalPool( uint16_t nRepetitions ) {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Clear previous pool
	randomPool.clear();

	// Build vector of 4 directions
	for ( int d = 0; d < 4; d++ ) {

		// Iterate over number of repetitions
		for ( int n = 0; n < nRepetitions; n++ ) {

			// Add direction to vector
			randomPool.push_back( d * 2 );	  // Multiply by two since cardinal directions are on 0, 2, 4, 6
		}
	}

	// Generate strong random seed
	uint32_t a	  = analogRead( A8 );										// Populate with floating pin if possible
	uint32_t t	  = micros();												// timer jitter
	uint32_t b	  = analogRead( A9 );										// Populate with floating pin if possible
	uint32_t seed = ( t << 16 ) ^ ( a << 1 ) ^ ( b << 17 ) ^ ( t >> 3 );	// Combination
	if ( seed == 0 ) seed = 0xA5A5F17E;										// Default to avoid zero
	std::mt19937 generator( seed );											// Mersenne Twister RNG

	// Shuffle direction vector
	std::shuffle( randomPool.begin(), randomPool.end(), generator );

	// Debug
	// PrintCardinalDirectionVector();

	// Clear response vector
	userResponses.clear();
	userResponses.resize( randomPool.size() );

	// Populate response vector with initial data
	for ( std::size_t e = 0; e < randomPool.size(); ++e ) {

		auto& entry = userResponses.at( e );
		// Populate struct
		entry.trialNumber		= e + 1;								   // Trial number
		entry.promptDelayTimeMs = timeDelayPoolMs.at( random( 0, 6 ) );	   // Delay time (in seconds)
		entry.promptVal			= randomPool.at( e );					   // Prompt value
		entry.promptString		= Shared->Enumerators.MapDiscriminationDirectionsToString( randomPool.at( e ) );
		entry.responseVal		= -1;		 // Default value
		entry.responseString	= "None";	 // Response string
		entry.responseTimeMs	= 0;		 // Response time
		entry.isResponseCorrect = false;	 // Default response
	}

	// Set current trial number
	currentTrialNumber = 0;

	// Debug
	PrintCardinalDirectionTable();
}



void CardinalDirectionsRuntimeClass::PrintCardinalDirectionTable() {

	// Print heading
	Serial.println();
	Serial.println( F( "=== Cardinal Direction Responses ============================================" ) );
	Serial.println( F( "Trial#\tPrompt\tDelayMs\t\tResponse\tTimeMs\tResult" ) );

	// Iterate over elements
	for ( std::size_t e = 0; e < userResponses.size(); e++ ) {

		Serial.print( userResponses.at( e ).trialNumber );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).promptVal );
		Serial.print( F( ", " ) );
		Serial.print( userResponses.at( e ).promptString );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).promptDelayTimeMs );
		Serial.print( F( "ms\t" ) );
		Serial.print( userResponses.at( e ).responseVal );
		Serial.print( F( ", " ) );
		Serial.print( userResponses.at( e ).responseString );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).responseTimeMs );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).isResponseCorrect ? "CORRECT" : "WRONG" );
		Serial.println();
	}

	Serial.println();
}



/*  ============================================================================================
 *  ============================================================================================
 *
 *   OOOOO   CCCCCC  TTTTTTT    AAA    NN   NN  TTTTTTT
 *  OO   OO  CC         TT     AA AA   NNN  NN    TT
 *  OO   OO  CC         TT    AA   AA  NN N NN    TT
 *  OO   OO  CC         TT    AAAAAAA  NN  NNN    TT
 *  OO   OO  CC         TT    AA   AA  NN   NN    TT
 *  OO   OO  CC         TT    AA   AA  NN   NN    TT
 *   OOOOO   CCCCCC     TT    AA   AA  NN   NN    TT
 *
 *  ============================================================================================
 *  ============================================================================================*/



/**
 * @brief Runs every loop
 * 
 */
void OctantDirectionsRuntimeClass::Loop() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Select action based on task state
	switch ( Shared->Tasks.DiscriminationTask.OctantDirections.currentState ) {

		case EnumsClass::DiscriminationTaskStateEnum::IDLE: {

			// Move state forward
			Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::STARTING;

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::STARTING: {

			// Initialize random pool
			if ( !isRandomPoolInitialized ) {
				InitializeRandomOctantPool( Shared->Tasks.DiscriminationTask.OctantDirections.nRepetitions );
				isRandomPoolInitialized = true;
				timeTotalTask			= millis();
			}

			// Indicate trial number
			Serial.print( F( "Trial " ) );
			Serial.print( currentTrialNumber + 1 );
			Serial.print( F( " of " ) );
			Serial.print( userResponses.size() );
			Serial.print( F( "... " ) );

			// Move state forward
			Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_DELAY;

			// Start delay timer
			timeDelayStartMs = millis();
			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_DELAY: {

			// Check if delay time has elapsed
			if ( millis() - timeDelayStartMs >= userResponses.at( currentTrialNumber ).promptDelayTimeMs ) {

				// Move to rendering prompt state
				Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::RENDERING_PROMPT;

				// Record prompt start time
				timePromptStartMs = millis();
			}
			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::RENDERING_PROMPT: {

			// Clear button pressed flag
			Shared->Interface.Gamepad.isInputWaiting = false;

			// Print prompt for now
			Serial.print( F( "Prompt: " ) );
			Serial.print( userResponses.at( currentTrialNumber ).promptString );

			// Move to waiting for response state
			Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_RESPONSE;

			// Record trial start time
			timeTrialStartMs = millis();

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::WAITING_FOR_RESPONSE: {

			// Check if response has been entered
			if ( Shared->Interface.Gamepad.isInputWaiting ) {

				// Record response time
				userResponses.at( currentTrialNumber ).responseTimeMs = millis() - timeTotalTask;

				// Record response value
				userResponses.at( currentTrialNumber ).responseVal	  = Shared->Interface.Gamepad.buttonPressed;
				userResponses.at( currentTrialNumber ).responseString = Shared->Enumerators.MapDiscriminationDirectionsToString( Shared->Interface.Gamepad.buttonPressed );

				// Check if response correct
				userResponses.at( currentTrialNumber ).isResponseCorrect = ( userResponses.at( currentTrialNumber ).responseVal == userResponses.at( currentTrialNumber ).promptVal );

				// Reset gamepad flag
				Shared->Interface.Gamepad.isInputWaiting = false;

				Serial.println( "\t\tResponse captured." );

				// Move to next state
				Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::FINISHING;
			}

			break;
		}

		case EnumsClass::DiscriminationTaskStateEnum::FINISHING: {

			// Increment trial number
			currentTrialNumber++;

			// Trial set complete
			if ( currentTrialNumber >= userResponses.size() ) {


				// Record total time
				Shared->Tasks.DiscriminationTask.OctantDirections.totalTime = millis() - timeTrialStartMs;

				// Print results
				PrintOctantDirectionTable();

				// Reset flags
				isRandomPoolInitialized = false;

				// Move to idle state
				Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::IDLE;
				Shared->State.systemState									   = EnumsClass::SystemStateEnum::IDLE;
				Shared->Tasks.activeTask									   = EnumsClass::TaskSelectionEnum::NONE;

				// Update user
				Serial.print( F( "TASK MANAGER:  Octant discrimination task finished in " ) );
				Serial.print( float( Shared->Tasks.DiscriminationTask.OctantDirections.totalTime / 1000.0f ), 2 );
				Serial.println( F( "s. Task complete, returning to idle." ) );

			} else {

				// Move to delay state
				Shared->Tasks.DiscriminationTask.OctantDirections.currentState = EnumsClass::DiscriminationTaskStateEnum::STARTING;

				// Start delay timer
				timeDelayStartMs = millis();
			}

			// Finish tests
			//if (currenttrialnum > nReps... )
			break;
		}
	}
}



/**
 * @brief Initialize the random octant directions pool
 * 
 */
void OctantDirectionsRuntimeClass::InitializeRandomOctantPool( uint16_t nRepetitions ) {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Clear previous pool
	randomPool.clear();

	// Build vector of 4 directions
	for ( int d = 0; d < 8; d++ ) {

		// Iterate over number of repetitions
		for ( int n = 0; n < nRepetitions; n++ ) {

			// Add direction to vector
			randomPool.push_back( d );	  // Multiply by two since octant directions are on 0, 2, 4, 6
		}
	}

	// Generate strong random seed
	uint32_t a	  = analogRead( A8 );										// Populate with floating pin if possible
	uint32_t t	  = micros();												// timer jitter
	uint32_t b	  = analogRead( A9 );										// Populate with floating pin if possible
	uint32_t seed = ( t << 16 ) ^ ( a << 1 ) ^ ( b << 17 ) ^ ( t >> 3 );	// Combination
	if ( seed == 0 ) seed = 0xA5A5F17E;										// Default to avoid zero
	std::mt19937 generator( seed );											// Mersenne Twister RNG

	// Shuffle direction vector
	std::shuffle( randomPool.begin(), randomPool.end(), generator );

	// Clear response vector
	userResponses.clear();
	userResponses.resize( randomPool.size() );

	// Populate response vector with initial data
	for ( std::size_t e = 0; e < randomPool.size(); ++e ) {

		auto& entry = userResponses.at( e );
		// Populate struct
		entry.trialNumber		= e + 1;								   // Trial number
		entry.promptDelayTimeMs = timeDelayPoolMs.at( random( 0, 6 ) );	   // Delay time (in seconds)
		entry.promptVal			= randomPool.at( e );					   // Prompt value
		entry.promptString		= Shared->Enumerators.MapDiscriminationDirectionsToString( randomPool.at( e ) );
		entry.responseVal		= -1;		 // Default value
		entry.responseString	= "None";	 // Response string
		entry.responseTimeMs	= 0;		 // Response time
		entry.isResponseCorrect = false;	 // Default response
	}

	// Set current trial number
	currentTrialNumber = 0;

	// Debug
	PrintOctantDirectionTable();
}



void OctantDirectionsRuntimeClass::PrintOctantDirectionTable() {

	// Print heading
	Serial.println();
	Serial.println( F( "=== Octant Direction Responses ============================================" ) );
	Serial.println( F( "Trial#\tPrompt\t\tDelayMs\t\tResponse\tTimeMs\tResult" ) );

	// Iterate over elements
	for ( std::size_t e = 0; e < userResponses.size(); e++ ) {

		Serial.print( userResponses.at( e ).trialNumber );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).promptVal );
		Serial.print( F( ", " ) );
		Serial.print( userResponses.at( e ).promptString );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).promptDelayTimeMs );
		Serial.print( F( "ms\t" ) );
		Serial.print( userResponses.at( e ).responseVal );
		Serial.print( F( ", " ) );
		Serial.print( userResponses.at( e ).responseString );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).responseTimeMs );
		Serial.print( F( "\t" ) );
		Serial.print( userResponses.at( e ).isResponseCorrect ? "CORRECT" : "WRONG" );
		Serial.println();
	}

	Serial.println();
}
