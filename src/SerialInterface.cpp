#include "SerialInterface.h"
#include "SharedMemory.h"



/*  ============================================================================================
 *  ============================================================================================
 *
 *  IIIIII  NN   NN  PPPPP   UU   UU  TTTTTTT
 *    II    NNN  NN  PP  PP  UU   UU     TT
 *    II    NN N NN  PP  PP  UU   UU     TT
 *    II    NN  NNN  PPPPP   UU   UU     TT
 *    II    NN   NN  PP      UU   UU     TT
 *    II    NN   NN  PP      UU   UU     TT
 *  IIIIII  NN   NN  PP       UUUUU      TT
 *
 *  ============================================================================================
 *  ============================================================================================*/


void InputClass::Begin() {

	Serial.println( F( "SERIAL:        Serial input interface...               Ready." ) );
}



void InputClass::Loop() { }



/*  ============================================================================================
 *  ============================================================================================
 *
 *   OOOOO   UU   UU  TTTTTTT  PPPPP    UU   UU  TTTTTTT
 *  OO   OO  UU   UU     T     PP  PP   UU   UU     T
 *  OO   OO  UU   UU     T     PP  PP   UU   UU     T
 *  OO   OO  UU   UU     T     PPPPP    UU   UU     T
 *  OO   OO  UU   UU     T     PP       UU   UU     T
 *  OO   OO  UU   UU     T     PP       UU   UU     T
 *   OOOOO    UUUUU      T     PP        UUUUU      T
 *
 *  ============================================================================================
 *  ============================================================================================*/



void OutputClass::Begin() {

	Serial.println( F( "SERIAL:        Serial output interface...              Ready." ) );
}



// === CONSTRUCTOR ================================================================================

// Constructor
SerialInterfaceClass::SerialInterfaceClass() {
	SerialInterfaceClass::instance = this;
}

// Global hook initialization
SerialInterfaceClass* SerialInterfaceClass::instance = nullptr;


// === PUBLIC ACCESSORS ===========================================================================

/**
 * @brief Print single-line status
 */
void OutputClass::PrintStatusLine() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Placeholder
	String tab = "    ";

	// System state
	Serial.print( F( "State: " ) );
	Serial.print( Shared->Enumerators.MapSystemStateEnumToString( static_cast<int8_t>( Shared->State.systemState ) ) );
	Serial.print( tab );

	// Serial connection
	Serial.print( F( "Serial: " ) );
	Serial.print( Shared->Interface.HWSerial.Connection.baudRateA );
	Serial.print( F( " bps" ) );
	Serial.print( tab );

	// Safety switch
	Serial.print( F( "Safety: " ) );
	Serial.print( Shared->Drive.Flags.isSafetySwitchEngaged ? "Engaged" : "Off" );
	Serial.print( tab );

	// Motor output
	Serial.print( F( "Output: " ) );
	Serial.print( Shared->Drive.Flags.isMotorOutputEnabled ? "Enabled" : "Off" );
	Serial.print( tab );

	// Tension
	Serial.print( F( "Tension: " ) );
	if ( Shared->Drive.Tension.isEnabled ) {
		Serial.print( F( "ON, Pwm = " ) );
		Serial.print( Shared->Drive.Tension.valueInteger );
		Serial.print( F( "%" ) );
	} else {
		Serial.print( F( "Off" ) );
	}

	Serial.print( tab );

	// Motor PWM
	Serial.print( F( "PowerABC: " ) );
	Serial.print( Shared->Drive.Pwm.totalOutgoingA );
	Serial.print( F( " | " ) );
	Serial.print( Shared->Drive.Pwm.totalOutgoingB );
	Serial.print( F( " | " ) );
	Serial.print( Shared->Drive.Pwm.totalOutgoingC );
	Serial.print( tab );

	// Motor current
	if ( Shared->Interface.SWSerial.Toggle.showMotorCurrents ) {

		Serial.print( F( "Current: " ) );
		Serial.print( Shared->Sensors.MotorCurrents.measuredCurrentAmpsA );
		Serial.print( F( "A | " ) );
		Serial.print( Shared->Sensors.MotorCurrents.measuredCurrentAmpsB );
		Serial.print( F( "A | " ) );
		Serial.print( Shared->Sensors.MotorCurrents.measuredCurrentAmpsC );
		Serial.print( F( "A" ) );
		Serial.print( tab );
	}

	// Motor angles in degrees
	if ( Shared->Interface.SWSerial.Toggle.showMotorAngles ) {

		Serial.print( F( "Motor Angles: " ) );
		Serial.print( Shared->Sensors.MotorEncoders.measuredAngleDegA );
		Serial.print( F( "° | " ) );
		Serial.print( Shared->Sensors.MotorEncoders.measuredAngleDegB );
		Serial.print( F( "° | " ) );
		Serial.print( Shared->Sensors.MotorEncoders.measuredAngleDegC );
		Serial.print( F( "°" ) );
	}

	// Platform arm encoders in degrees
	if ( Shared->Interface.SWSerial.Toggle.showPlatformEncoders ) {

		Serial.print( F( "    " ) );
		Serial.print( F( "ThetaXY: " ) );
		Serial.print( Shared->Sensors.PlatformEncoders.horizontalAngleDegrees );
		Serial.print( F( "° | " ) );
		Serial.print( Shared->Sensors.PlatformEncoders.verticalAngleDegrees );
		Serial.print( F( "°" ) );
		Serial.print( tab );
	}

	// Active task
	if ( Shared->State.systemState == EnumsClass::SystemStateEnum::RUNNING_TASK ) {

		Serial.print( F( "Task: " ) );
		Serial.print( Shared->Enumerators.MapTaskSelectionEnumToString( static_cast<int8_t>( Shared->Tasks.activeTask ) ) );
	}



	// 	if ( SYSTEM_STATE == systemStateEnum::IDLE ) {
	// 		Serial.print( "IDLE" );
	// 	} else if ( SYSTEM_STATE == systemStateEnum::DISCRIMINATION_TASK_TESTING ) {
	// 		Serial.print( "DISC_TESTING" );
	// 	} else if ( SYSTEM_STATE == systemStateEnum::MEASURING_RANGE_OF_MOTION ) {
	// 		Serial.print( "MEASURING_ROM" );
	// 	}
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "Output: " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierOutputState() ? "ON" : "Off" );
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "Safety: " ) );
	// 	Serial.print( AmplifierReference.GetSafetySwitchState() ? "ON" : "Off" );
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "Baud: " ) );
	// 	Serial.print( AmplifierReference.GetBaudA() );



	// 	// Motor Limits
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "Motor Limit: " ) );
	// 	Serial.print( AmplifierReference.GetIsAmplifierPowerLimitSet() ? "SET" : "Not Set" );
	// 	Serial.print( F( ", " ) );
	// 	Serial.print( AmplifierReference.GetIsAmplifierPowerLimitApplied() ? "ON" : "Off" );
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "LimitsABC: " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierPwmPowerLimitA() );
	// 	Serial.print( F( " | " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierPwmPowerLimitB() );
	// 	Serial.print( F( " | " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierPwmPowerLimitC() );


	// 	// Encoder Limits
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "Encoder Limit: " ) );
	// 	Serial.print( AmplifierReference.GetIsAmplifierEncoderLimitSet() ? "SET" : "Not Set" );
	// 	Serial.print( F( ", " ) );
	// 	Serial.print( AmplifierReference.GetIsAmplifierEncoderLimitApplied() ? "ON" : "Off" );
	// 	Serial.print( F( "    " ) );
	// 	Serial.print( F( "LimitsABC: " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierEncoderLimitA() );
	// 	Serial.print( F( "° | " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierEncoderLimitB() );
	// 	Serial.print( F( "° | " ) );
	// 	Serial.print( AmplifierReference.GetAmplifierEncoderLimitC() );
	// 	Serial.print( F( "°" ) );



	// 	// Gamepad
	// 	Serial.print( F( "   " ) );
	// 	Serial.print( F( "Gamepad: " ) );
	// 	Serial.print( gamepadButton );
	// 	Serial.print( F( " (" ) );
	// 	Serial.print( gamepadString );
	// 	Serial.print( F( ")" ) );

	Serial.println();
}



/*  ============================================================================================
 *  ============================================================================================
 *
 *   EEEEEE   VV   VV  EEEEEE  NN   NN  TTTTTTT  SSSSS
 *   EE       VV   VV  EE      NNN  NN     TT    SS
 *   EE       VV   VV  EE      NN N NN     TT    SS
 *   EEEE      VV VV   EEEE    NN  NNN     TT     SSSS
 *   EE        VV VV   EE      NN   NN     TT        SS
 *   EE         VVV    EE      NN   NN     TT        SS
 *   EEEEEE     VVV    EEEEEE  NN   NN     TT    SSSSS
 *
 *  ============================================================================================
 *  ============================================================================================*/



/**
 * @brief Serial event handler for software serial
 */
void serialEvent() {

	// Connect hook to class function
	if ( SerialInterfaceClass::instance ) {
		SerialInterfaceClass::instance->Input.OnSoftwareSerialEvent();
	}
}


/**
 * @brief Callback for software serial
 */
void InputClass::OnSoftwareSerialEvent() {

	// Read data while buffer is populated
	while ( Serial.available() > 0 ) {

		// Read each byte
		char incomingChar = ( char )Serial.read();

		// Look for terminating character
		if ( incomingChar == '\r' || incomingChar == '\n' ) {

			// Make sure packet isn't empty
			if ( incomingSerialString != "" ) {

				// Parse packet and respond
				RespondToKeyboardInput();
			}
		} else {
			incomingSerialString += incomingChar;
		}
	}
}


/**
 * @brief Parse keyboard inputs and respond accordingly
 * 
 */
void InputClass::RespondToKeyboardInput() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Check that incoming serial string is not empty
	if ( incomingSerialString != nullptr && incomingSerialString != "" ) {

		// Print command received
		Serial.print( F( ">> Received command [" ) );
		Serial.print( incomingSerialString );
		Serial.println( "]" );

		// Extract command key
		char cmd = incomingSerialString.charAt( 0 );

		// Set idle state
		if ( cmd == '`' ) {

			// Update state
			Shared->State.systemState = EnumsClass::SystemStateEnum::IDLE;
		}

		// Set measure ROM state
		if ( cmd == '1' ) {

			// Respond_MeasureAmplifierRangeOfMotionLimits();
		}

		// Test RoM
		if ( cmd == '2' ) {

			// Respond_TestRangeOfMotionLimits();
		}

		// Discrimination Task: Cardinal Direction
		if ( cmd == '3' ) {
			SetDiscriminationTaskCardinalStart();
		}

		// Discrimination Task: Octant Direction
		if ( cmd == '4' ) {
			SetDiscriminationTaskOctantStart();
		}

		// Set tension value
		if ( cmd == 't' ) {
			SetMotorTension();
		}

		// Toggle tension enable / disable
		if ( cmd == 'T' ) {
			SetTensionEnabled();
		}

		// Print system state
		if ( cmd == 's' ) {

			// Respond_PrintSystemStateBlock();
		}

		// Set debug scroll state
		if ( cmd == 'S' ) {
			SetScrollingOutputEnabled();
		}

		// Zero arm encoders
		if ( cmd == 'z' ) {
			SetPlatformEncodersZero();
		}

		// Zero amplifier encoders
		if ( cmd == 'Z' ) {

			SetMotorEncodersZero();
		}

		// Toggle amplfier output state
		if ( cmd == 'e' || cmd == 'E' ) {
			SetAmplifierOutputEnabled();
		}

		// Toggle amp limits
		if ( cmd == 'l' || cmd == 'L' ) {

			// Respond_ToggleCurrentLimits();
			// }
		}

		// Cancel all tasks and return to idle
		if ( cmd == 'X' || cmd == 'x' ) {

			Shared->State.systemState = EnumsClass::SystemStateEnum::IDLE;
			Shared->Tasks.activeTask  = EnumsClass::TaskSelectionEnum::NONE;
			Serial.println( F( "   >> Cancelling all tasks and returning to idle." ) );
		}

		// Reset incoming serial string
		incomingSerialString = "";
		Serial.println();

		// Re-load menu and state
		// PrintSystemState();
		// PrintMenu();
	}
}



/*  ===================================================================================
 *  ===================================================================================
 *
 *  RRRRR    EEEEEE  SSSSS   PPPPP    OOOOO   NN   NN  SSSSS   EEEEEE 
 *  RR  RR   EE      SS      PP  PP  OO   OO  NNN  NN  SS      EE
 *  RR  RR   EE      SS      PP  PP  OO   OO  NN N NN  SS      EE
 *  RRRRR    EEEE     SSSS   PPPPP   OO   OO  NN  NNN   SSSS   EEEE
 *  RR  RR   EE          SS  PP      OO   OO  NN   NN      SS  EE
 *  RR  RR   EE          SS  PP      OO   OO  NN   NN      SS  EE
 *  RR  RR   EEEEEE  SSSSS   PP       OOOOO   NN   NN  SSSSS   EEEEEE
 *
 *  ===================================================================================
 *  ===================================================================================*/



/**
 * @brief Set the scrolling output state
 * 
 * @param newState New state
 */
void InputClass::SetScrollingOutputEnabled() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Update state
	bool oldState									  = Shared->Interface.SWSerial.isScrollingLineEnabled;
	Shared->Interface.SWSerial.isScrollingLineEnabled = !oldState;

	// Debug text
	Serial.println( F( "   >> Toggling system scroll." ) );
}


/**
 * @brief Set amplifier output state
 * 
 * @param newState 
 */
void InputClass::SetAmplifierOutputEnabled() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Update state
	bool oldState							 = Shared->Drive.Flags.isMotorOutputEnabled;
	Shared->Drive.Flags.isMotorOutputEnabled = !oldState;

	// Debug text
	Serial.println( F( "   >> Toggling motor output." ) );
}

/**
 * @brief Zero platform encoders
 * 
 */
void InputClass::SetPlatformEncodersZero() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Update action queue
	Shared->ActionQueue.zeroPlatformEncoders = true;

	// Debug text
	Serial.println( F( "   >> Zeroing platform encoders." ) );
}


/**
 * @brief Zero motor encoders
 * 
 */
void InputClass::SetMotorEncodersZero() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Update action queue
	Shared->ActionQueue.zeroMotorEncoders = true;

	// Debug text
	Serial.println( F( "   >> Zeroing motor encoders." ) );
}



/**
 * @brief Sets motor tension
 * 
 * @param newTensionValue 
 */
void InputClass::SetMotorTension() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// incomingSerialString

	// Check: is the next character a number
	if ( isdigit( incomingSerialString.charAt( 1 ) ) ) {

		// Extract value
		uint8_t newTensionValue = incomingSerialString.substring( 1 ).toInt();

		// Make sure tension is limited to 20 percent
		if ( newTensionValue <= 20 ) {

			// Store value
			Shared->Drive.Tension.valueInteger = newTensionValue;

			// Update action queue
			Shared->ActionQueue.setMotorTension = true;

			// Serial response
			Serial.print( F( "   >> Setting tension value to " ) );
			Serial.print( Shared->Drive.Tension.valueInteger );
			Serial.println( F( "%" ) );

		} else {

			// Serial response
			Serial.println( F( "   >> Tensioning value is too high, ignoring command." ) );
		}
	} else {

		// Non-numberic input
		Serial.println( F( "   >> Invalid tension command! Value is non-numeric." ) );
	}
}



/**
 * @brief Set amplifier output state
 * 
 * @param newState 
 */
void InputClass::SetTensionEnabled() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Update state
	bool oldState					= Shared->Drive.Tension.isEnabled;
	Shared->Drive.Tension.isEnabled = !oldState;

	// Debug text
	Serial.println( F( "   >> Toggling motor output." ) );
}



void InputClass::SetDiscriminationTaskCardinalStart() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Check: is the next character a number
	if ( isdigit( incomingSerialString.charAt( 1 ) ) ) {

		// Extract value
		uint8_t nReps = incomingSerialString.substring( 1 ).toInt();

		// Make sure tension is limited to 20 percent
		if ( nReps > 0 && nReps <= 10 ) {

			// Store value
			Shared->Tasks.DiscriminationTask.CardinalDirections.nRepetitions = nReps;

			// Serial response
			Serial.print( F( "SERIAL:        Starting Discrimination Task (Cardinal) with " ) );
			Serial.print( nReps );
			Serial.println( F( " repetitions" ) );

			Shared->State.systemState = EnumsClass::SystemStateEnum::RUNNING_TASK;
			Shared->Tasks.activeTask  = EnumsClass::TaskSelectionEnum::TESTING_CARDINAL_DIRECTIONS;


		} else {

			// Serial response
			Serial.println( F( "   >> Repetition value is too high, ignoring command." ) );
		}
	} else {

		// Non-numberic input
		Serial.println( F( "   >> Invalid repetition value! Value is non-numeric." ) );
	}
}



void InputClass::SetDiscriminationTaskOctantStart() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Check: is the next character a number
	if ( isdigit( incomingSerialString.charAt( 1 ) ) ) {

		// Extract value
		uint8_t nReps = incomingSerialString.substring( 1 ).toInt();

		// Make sure tension is limited to 20 percent
		if ( nReps > 0 && nReps <= 10 ) {

			// Store value
			Shared->Tasks.DiscriminationTask.OctantDirections.nRepetitions = nReps;

			// Serial response
			Serial.print( F( "SERIAL:        Starting Discrimination Task (Octant) with " ) );
			Serial.print( nReps );
			Serial.println( F( " repetitions" ) );

			Shared->State.systemState = EnumsClass::SystemStateEnum::RUNNING_TASK;
			Shared->Tasks.activeTask  = EnumsClass::TaskSelectionEnum::TESTING_OCTANT_DIRECTIONS;


		} else {

			// Serial response
			Serial.println( F( "   >> Repetition value is too high, ignoring command." ) );
		}
	} else {

		// Non-numberic input
		Serial.println( F( "   >> Invalid repetition value! Value is non-numeric." ) );
	}
}