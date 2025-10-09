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
#include "Amplifier.h"			  // BLDC amplifiers
#include "ArmEncoders.h"		  // Arm encoders on test platform
#include "Gamepad.h"			  // Gamepad buttons
#include "KeyboardInterface.h"	  // Keyboard input
#include "_Globals.h"			  // Global variable containers


// Create class objects
EncoderClass		   ArmEncoders;			 // Read experimental platform encoders
GamepadClass		   Gamepad;				 // Read experimental platform gamepad
AmplifierClass		   Amplifier;			 // Control amplifier interactions
KeyboardInterfaceClass KeyboardInterface;	 // Software serial I/O handling


// === Forward Declarations =======================================================================
void CheckPeripherals();		 // Update all peripherals (spins every loop)
void CheckKeyboardInput();		 // Parse incoming keyboard inputs
void CheckGamepadInput();		 // Parse gamepad inputs
void DrawMenu();				 // Draw a menu on screen of options
void PrintSystemState();		 // Prints the system state
void CheckAmplifierPwm();		 // Update the amplifer throught he interval timer
void CheckAmplifierEncoder();	 // Update the amplifer throught he interval timer

// === Global Structs =============================================================================
KeyboardGlobals	  varKeyboard;		 // Keyboard input variables
AmplifierGlobals  varAmplifier;		 // Amplifier variables
ArmEncoderGlobals varArmEncoders;	 // Encoder variables
GamepadGlobals	  varGamepad;		 // Gamepad variables


// === INTERVAL TIMERS ============================================================================
IntervalTimer IT_AmplifierPwmTimer;
IntervalTimer IT_AmplifierEncoderTimer;


/**
 * @brief Initialize teensy parameters (Arduino style)
 * 
 */
void setup() {

	Serial.begin( 9600 );
	while ( !Serial );	  // Waiting until serial connected

	delay( 1000 );
	Serial.println( "Initializing..." );

	// Onboard LED
	pinMode( 13, OUTPUT );

	// Initialize classes at specified frequencies (-1 = unlimited Hz)
	KeyboardInterface.Begin( varKeyboard.CONST_UPDATE_FREQUENCY );	  // Software serial interface for reading keyboard input
	Amplifier.Begin();												  // Amplifier controls for BLDC
	ArmEncoders.Begin();											  // Experimental platform encoders
	Gamepad.Begin();												  // Experimental platform gamepad

	// IT_AmplifierPwmTimer.begin( CheckAmplifierPwm, 1000000 / 1000 );
	// IT_AmplifierEncoderTimer.begin( CheckAmplifierEncoder, 1000000 / 20 );

	delay( 1000 );

	Serial.println( "SYSTEM READY!" );
	Serial.println();

	// Display menu
	PrintSystemState();
	DrawMenu();
}


/**
 * @brief Main program loop
 * 
 */
void loop() {

	// Update hardware peripherals
	CheckPeripherals();

	// Check for keyboard input
	CheckKeyboardInput();

	// Check for gamepad input
	CheckGamepadInput();
}


void CheckPeripherals() {

	// Update serial interface
	KeyboardInterface.Update( varKeyboard.incomingBuffer );

	// // Read encoders
	ArmEncoders.Update( varArmEncoders.horizontalAngleDegrees, varArmEncoders.VerticalAngleDegrees );

	// // Read gamepad buttons
	Gamepad.Update( varGamepad.isButtonPressed, varGamepad.buttonPressedIndex, varGamepad.buttonPressedName );
}



void CheckKeyboardInput() {

	// Extract key
	if ( varKeyboard.incomingBuffer != nullptr && varKeyboard.incomingBuffer[0] != '\0' ) {
		char cmd = varKeyboard.incomingBuffer[0];
		Serial.print( F( ">> Received command '" ) );
		Serial.print( varKeyboard.incomingBuffer );
		Serial.print( F( "':" ) );
		Serial.println();
		Serial.print( F( "   " ) );

		// T = Set tension value
		if ( cmd == 't' || cmd == 'T' ) {

			// Make sure second character is a value
			if ( isdigit( varKeyboard.incomingBuffer[1] ) ) {
				uint8_t val = atoi( varKeyboard.incomingBuffer + 1 );

				// Check that tensioning isn't too high
				if ( val <= 25 ) {

					// Update tension value
					Serial.print( F( "Setting tension value to " ) );
					Serial.print( val );
					Serial.println( F( "%." ) );
					// Amplifier.COMMAND_SetTension( val );
				} else {
					Serial.println( F( "Tensioning value too high, ignoring." ) );
				}
			} else {
				Serial.println( F( "Invalid tension command, value is non-numeric." ) );
			}

		}
		// S = Read system state
		else if ( cmd == 's' || cmd == 'S' ) {

			// Print system state
			CheckPeripherals();
			PrintSystemState();

		}
		// E = Enable amplifiers
		else if ( cmd == 'e' || cmd == 'E' ) {

			// Toggle amplifier enable
			// if ( Amplifier.FLAGS_GetAmplifierState() ) {
			// 	// 	Amplifier.COMMAND_Disable();
			// 	Serial.println( F( "Disabling amplifiers." ) );
			// } else {
			// 	// Amplifier.COMMAND_Enable();
			// 	Serial.println( F( "Enabling amplifiers." ) );
			// }

		}
		// Z = Zero arm encoders
		else if ( cmd == 'z' || cmd == 'Z' ) {

			// Zero  arm encoders
			ArmEncoders.ZeroEncoders();
			Serial.println( F( "Zeroing encoders." ) );
		}
		// X = Zero motor encoders
		else if ( cmd == 'x' || cmd == 'X' ) {

			// Zero  arm encoders
			// Amplifier.HWSERIAL_SetEncodersZero();
			Serial.println( F( "Zeroing motor encoders." ) );
		}
		// Command unknown
		else {
			Serial.println( F( "Command not understood, ignoring command. " ) );
		}

		varKeyboard.incomingBuffer = nullptr;
		Serial.flush();
		Serial.clear();
		Serial.println();

		// Update displays
		PrintSystemState();
		DrawMenu();
	}
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


/**
 * @brief Draws menu of keyboard options
 */
void DrawMenu() {

	Serial.println();
	Serial.println( F( "=== INPUT MENU =====================================================================================" ) );
	Serial.println( F( "Information:   [S] Display system state      " ) );
	Serial.println( F( "Controls:      [E] Enable motor output       [Txx] Set tension to xx%" ) );
	Serial.println( F( "Platform:      [Z] Zero platform encoders" ) );
}


void PrintSystemState() {

	Serial.println( F( "=== SYSTEM STATE ===================================================================================" ) );

	// Amplifier state
	Serial.print( F( "Amplifiers:         Safety Switch: " ) );
	Serial.print( Amplifier.GetSafetySwitchState() ? "Engaged" : "Disengaged" );
	Serial.println();

	// Amplifier names
	Serial.print( F( "                    " ) );
	Serial.print( Amplifier.GetAmplifierNameA() );
	Serial.print( F( "                         " ) );
	Serial.print( Amplifier.GetAmplifierNameB() );
	Serial.print( F( "                         " ) );
	Serial.print( Amplifier.GetAmplifierNameC() );
	Serial.println();

	// Baud
	Serial.print( F( "                    BaudA: " ) );
	Serial.print( Amplifier.GetBaudA() );
	Serial.print( F( "                    BaudB: " ) );
	Serial.print( Amplifier.GetBaudB() );
	Serial.print( F( "                    BaudC: " ) );
	Serial.print( Amplifier.GetBaudC() );
	Serial.println();

	// Motor encoders
	Serial.print( F( "Motor Encoders:     PhiA:   " ) );
	Serial.print( Amplifier.GetAngleDegA() );
	Serial.print( F( " deg    PhiB:   " ) );
	Serial.print( Amplifier.GetAngleDegB() );
	Serial.print( F( " deg    PhiC:   " ) );
	Serial.print( Amplifier.GetAngleDegC() );
	Serial.println( F( " deg" ) );


	// Arm encoders
	Serial.print( F( "Platform Encoders:  ArmX:   " ) );
	Serial.print( varArmEncoders.horizontalAngleDegrees );
	Serial.print( F( " deg    ArmY:   " ) );
	Serial.print( varArmEncoders.VerticalAngleDegrees );
	Serial.println( F( " deg" ) );

	Serial.println();
}



void CheckAmplifierPwm() {

	// // Update amplifier state
	// Amplifier.UpdatePWM();
	// Amplifier.UpdateEncoders();
}

void CheckAmplifierEncoder() {

	// Amplifier.UpdateEncoders( varAmplifier.angleADegrees, varAmplifier.angleBDegrees, varAmplifier.angleCDegrees );
	// Amplifier.UpdateEncoders();
}