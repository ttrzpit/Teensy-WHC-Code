/**
 * @file main.cpp
 * @author Tomasz Trzpit
 * @brief NURing firmware suite
 * @version 0.1
 * @date 2025-10-02
 * 
 */



// === Project Headers ============================================================================
// Pre-built libraries
#include <Arduino.h>

// Custom libraries
#include "Amplifier.h"					// BLDC amplifiers
#include "ArmEncoders.h"				// Arm encoders on test platform
#include "Gamepad.h"					// Gamepad buttons
#include "SoftwareSerialInterface.h"	// Keyboard serial input
#include "_Globals.h"					// Global variable containers



// === System State ===============================================================================
// Enumberation of system states
// enum class stateEnum { IDLE, MEASURING_RANGE_OF_MOTION, MEASURING_CURRENT_LIMITS };
systemStateEnum SYSTEM_STATE	  = systemStateEnum::IDLE;
String			systemStateString = "";



// === Class Objects ==============================================================================
// Create class objects
ArmEncoderClass				 ArmEncoders;												 // Read experimental platform encoders
GamepadClass				 Gamepad;													 // Read experimental platform gamepad
AmplifierClass				 Amplifier;													 // Control amplifier interactions
SoftwareSerialInterfaceClass SerialInterface( SYSTEM_STATE, Amplifier, ArmEncoders );	 // Software serial I/O handling



// === Forward Declarations =======================================================================
void CheckPeripherals();		  // Update all peripherals (spins every loop)
void CheckGamepadInput();		  // Parse gamepad inputs
void CheckAmplifierSensors();	  // Update the amplifer throught he interval timer
void DriveAmplifiers();			  // Send drive command to amplifiers
void PrintSerialScroll();		  // Prints the system serial scroll
void StateMachine();			  // System state machine
void State_MeasuringROM();		  // Measure ROM limits for encoders
void State_MeasuringCurrent();	  // Measure current limits for encoders



// === Global Structs =============================================================================
KeyboardGlobals	  varKeyboard;		 // Keyboard input variables
AmplifierGlobals  varAmplifier;		 // Amplifier variables
ArmEncoderGlobals varArmEncoders;	 // Encoder variables
GamepadGlobals	  varGamepad;		 // Gamepad variables

// === INTERVAL TIMERS ============================================================================
IntervalTimer IT_AmplifierOutputTimer;
IntervalTimer IT_AmplifierEncoderTimer;
IntervalTimer IT_SerialScollTimer;


/**
 * @brief Initialize teensy parameters (Arduino style)
 * 
 */
void setup() {

	// Start software serial port (over USB)
	Serial.begin( 9600 );
	while ( !Serial );	  // Waiting until serial connected

	// Allow time for Serial interface to spin up
	delay( 1000 );
	Serial.println( "Initializing..." );

	// Configure debug output
	Amplifier.EnableDebugOutput( false );

	// Onboard LED
	pinMode( 13, OUTPUT );

	// Initialize classes
	SerialInterface.Begin();	// Software serial interface for reading keyboard input
	Amplifier.Begin();			// Amplifier controls for BLDC
	ArmEncoders.Begin();		// Experimental platform encoders
	Gamepad.Begin();			// Experimental platform gamepad

	IT_AmplifierOutputTimer.begin( DriveAmplifiers, 1000000 / 1000 );
	IT_AmplifierEncoderTimer.begin( CheckAmplifierSensors, 1000000 / 300 );
	IT_SerialScollTimer.begin( PrintSerialScroll, 1000000 / 2 );

	// Delay to clear everything
	delay( 1000 );

	Serial.println( "SYSTEM READY!" );
	Serial.println();

	// Display state and menu
	SerialInterface.PrintSystemState();
	SerialInterface.PrintMenu();
}


/**
 * @brief Main program loop
 * 
 */
void loop() {

	// Check state machine
	StateMachine();

	// Update hardware peripherals
	CheckPeripherals();
}


void CheckPeripherals() {

	// Read encoders
	ArmEncoders.Check( varArmEncoders.horizontalAngleDegrees, varArmEncoders.VerticalAngleDegrees );

	// Read gamepad buttons
	Gamepad.Check( varGamepad.isButtonPressed, varGamepad.buttonPressedIndex, varGamepad.buttonPressedName );

	// Check for gamepad input
	CheckGamepadInput();
}


/**
 * @brief Handles gamepad inputs
 */
void CheckGamepadInput() {

	if ( varGamepad.isButtonPressed ) {
		Serial.print( F( "Received gamepad input:\n  " ) );
		Serial.print( varGamepad.buttonPressedIndex );
		Serial.print( " (" );
		Serial.print( varGamepad.buttonPressedName );
		Serial.println( F( ")" ) );
	}
}



// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   IIIIII  TTTTTTT      CCCCC    AAA    LL      LL      BBBBB     AAA     CCCCC  KK  KK   SSSSS
//  *     II       T        CC       AA AA   LL      LL      BB  BB   AA AA   CC      KK KK   SS
//  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KKKK    SS
//  *     II       T        CC      AAAAAAA  LL      LL      BBBBB   AAAAAAA  CC      KKKK     SSSSS
//  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KK KK        SS
//  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KK  KK       SS
//  *   IIIIII     T         CCCCC  AA   AA  LLLLLL  LLLLLL  BBBBB   AA   AA   CCCCC  KK  KK   SSSSS
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/



/**
 * @brief IntervalTimer callback to read amplifier encoders and current sensors
 */
void CheckAmplifierSensors() {

	Amplifier.ReadSensors();
}


/**
 * @brief IntervalTimer callback to drive motor output
 */
void DriveAmplifiers() {
	Amplifier.DriveMotorOutputs();
}


/**
 * @brief IntervalTimer callback to show serial scroll 
 */
void PrintSerialScroll() {

	// Check if scrolling is enabled
	if ( SerialInterface.isScrollingOutputEnabled ) {

		// Print the scroll line
		SerialInterface.PrintScrollingOutput();
	}
}


// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   SSSSS  YY    YY  SSSSS  TTTTTTTT  EEEEEE  MM    MM       SSSSS  TTTTTTTT    AAA    TTTTTTTT  EEEEEE
//  *  SS       YY  YY  SS         TT     EE      MMM  MMM      SS         TT      AA AA      TT     EE
//  *  SS        YYYY   SS         TT     EE      MM MM MM      SS         TT     AA   AA     TT     EE
//  *   SSSS      YY     SSSS      TT     EEEE    MM    MM       SSSS      TT     AAAAAAA     TT     EEEE
//  *      SS     YY         SS    TT     EE      MM    MM           SS    TT     AA   AA     TT     EE
//  *      SS     YY         SS    TT     EE      MM    MM           SS    TT     AA   AA     TT     EE
//  *   SSSS      YY     SSSSS     TT     EEEEEE  MM    MM       SSSSS     TT     AA   AA     TT     EEEEEE
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/


/**
 * @brief Main state machine
 */
void StateMachine() {

	// System is idle
	if ( SYSTEM_STATE == systemStateEnum::IDLE ) {

		// Serial.println( "IDLE" );

	}
	// System is measuring range of motion
	else if ( SYSTEM_STATE == systemStateEnum::MEASURING_RANGE_OF_MOTION ) {

		// Handled by SoftwareSerialInterface
		

	}
	// System is measuring current limits
	else if ( SYSTEM_STATE == systemStateEnum::MEASURING_CURRENT_LIMITS ) {

		// Serial.println( "CURR" );
	}
}



/********************
 *  STATE HANDLERS  *
 ********************/

/**
 * @brief Handles Idle state
 */
void State_Idle();	  // Measure current limits for encoders


/**
 * @brief Handles MeasuringROM state
 */
void State_MeasuringROM() { }


/**
 * @brief Handles MeasuringCurrent state
 */
void State_MeasuringCurrent() { }



// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   OOOOO   LL      DDDDD
//  *  OO   OO  LL      DD  DD
//  *  OO   OO  LL      DD  DD
//  *  OO   OO  LL      DD  DD
//  *  OO   OO  LL      DD  DD
//  *  OO   OO  LL      DD  DD
//  *   OOOOO   LLLLLL  DDDDD
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/


// void CheckKeyboardInput() {

// 	// Extract key
// 	if ( varKeyboard.incomingBuffer != nullptr && varKeyboard.incomingBuffer[0] != '\0' ) {
// 		char cmd = varKeyboard.incomingBuffer[0];
// 		Serial.print( F( ">> Received command '" ) );
// 		Serial.print( varKeyboard.incomingBuffer );
// 		Serial.print( F( "':" ) );
// 		Serial.println();
// 		Serial.print( F( "   " ) );

// 		// T = Set tension value
// 		if ( cmd == 't' || cmd == 'T' ) {

// 			// Make sure second character is a value
// 			if ( isdigit( varKeyboard.incomingBuffer[1] ) ) {
// 				uint8_t val = atoi( varKeyboard.incomingBuffer + 1 );

// 				// Check that tensioning isn't too high
// 				if ( val <= 25 ) {

// 					// Update tension value
// 					Serial.print( F( "Setting tension value to " ) );
// 					Serial.print( val );
// 					Serial.println( F( "%." ) );
// 					// Amplifier.COMMAND_SetTension( val );
// 				} else {
// 					Serial.println( F( "Tensioning value too high, ignoring." ) );
// 				}
// 			} else {
// 				Serial.println( F( "Invalid tension command, value is non-numeric." ) );
// 			}

// 		}
// 		// S = Read system state
// 		else if ( cmd == 's' || cmd == 'S' ) {

// 			// Print system state
// 			CheckPeripherals();
// 			PrintSystemState();

// 		}
// 		// E = Enable amplifiers
// 		else if ( cmd == 'e' || cmd == 'E' ) {

// 			// Toggle amplifier enable
// 			// if ( Amplifier.FLAGS_GetAmplifierState() ) {
// 			// 	// 	Amplifier.COMMAND_Disable();
// 			// 	Serial.println( F( "Disabling amplifiers." ) );
// 			// } else {
// 			// 	// Amplifier.COMMAND_Enable();
// 			// 	Serial.println( F( "Enabling amplifiers." ) );
// 			// }

// 		}
// 		// Z = Zero arm encoders
// 		else if ( cmd == 'z' || cmd == 'Z' ) {

// 			// Zero  arm encoders
// 			ArmEncoders.ZeroEncoders();
// 			Serial.println( F( "Zeroing encoders." ) );
// 		}
// 		// X = Zero motor encoders
// 		else if ( cmd == 'x' || cmd == 'X' ) {

// 			// Zero  arm encoders
// 			// Amplifier.HWSERIAL_SetEncodersZero();
// 			Serial.println( F( "Zeroing motor encoders." ) );
// 		}
// 		// Command unknown
// 		else {
// 			Serial.println( F( "Command not understood, ignoring command. " ) );
// 		}

// 		varKeyboard.incomingBuffer = nullptr;
// 		Serial.flush();
// 		Serial.clear();
// 		Serial.println();

// 		// Update displays
// 		PrintSystemState();
// 		DrawMenu();
// 	}
// }
