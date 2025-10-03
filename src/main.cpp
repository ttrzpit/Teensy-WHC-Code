/**
 * @file main.cpp
 * @author Tomasz Trzpit
 * @brief NURing firmware suite
 * @version 0.1
 * @date 2025-10-02
 * 
 */

// Pre-built libraries
#include <Arduino.h>

// Custom libraries
#include "AmplifierClass.h"
#include "EncoderClass.h"
#include "GamepadClass.h"

// Create class objects
EncoderClass   Encoders;
GamepadClass   Gamepad;
AmplifierClass Amplifier;

// Interval timers
// IntervalTimer SerialDisplayTimer;

// SETTINGS
uint16_t serialTimerFrequency = 10;	   // [Hz]

// Timing variables
elapsedMillis serialRuntimeMillis;	  // Timer for serial input/output

// Serial command buffer
static char	   inputCommandBuffer[8];
static uint8_t inputCommandIndex = 0;


// Prototypes
void		ReadSerial();
void		ShowSerial();
static void ProcessSerial( const char* buffer );


/**
 * @brief Initialize teensy parameters (Arduino style)
 * 
 */
void setup() {

	pinMode( 13, OUTPUT );

	Serial.begin( 9600 );
	Serial.println( "Starting..." );
}


/**
 * @brief Main program loop
 * 
 */
void loop() {

	// Read serial input
	ReadSerial();

	// Read encoders
	Encoders.PollEncoders();

	// Read gamepad buttons
	Gamepad.PollButtons();

	// Show serial output
	ShowSerial();
}



/**
 * @brief Process any serial inputs
 * 
 */
static void ProcessSerial( const char* buffer ) {

	// Extract key
	char cmd = buffer[0];

	// E-stop
	if ( cmd == 'x' && buffer[1] == '\0' ) {
		Serial.println( F( "Stop" ) );
		return;
	}

	// Zero encoders
	if ( cmd == 'z' && buffer[1] == '\0' ) {
		Encoders.ZeroEncoders();
		Serial.println( F( "Encoders zeroed" ) );
		return;
	}

	// Set tension value
	if ( cmd == 't' ) {
		uint8_t val = atoi( buffer + 1 );
		Serial.print( F( "Tensioning% = " ) );
		Serial.println( val );
		return;
	}

	// Unknown command
	Serial.print( F( "Command unknown: " ) );
	Serial.println( buffer );
}



/**
 * @brief Read serial input byte-by-byte
 * 
 */
void ReadSerial() {

	// Check if data is waiting
	while ( Serial.available() ) {

		// Read character
		char c = ( char )Serial.read();

		// Read end of line
		if ( c == '\n' || c == '\r' ) {
			if ( inputCommandIndex > 0 ) {
				inputCommandBuffer[inputCommandIndex] = '\0';
				ProcessSerial( inputCommandBuffer );
				inputCommandIndex = 0;
			}
			continue;
		}

		// Add char into buffer (max 7 chars + null)
		if ( inputCommandIndex < sizeof( inputCommandBuffer ) - 1 ) {
			inputCommandBuffer[inputCommandIndex++] = c;

			// Optional: auto-commit at 4 chars
			if ( inputCommandIndex >= 4 ) {
				inputCommandBuffer[inputCommandIndex] = '\0';
				ProcessSerial( inputCommandBuffer );
				inputCommandIndex = 0;
			}
		} else {
			// Overflow -> terminate, process, reset
			inputCommandBuffer[sizeof( inputCommandBuffer ) - 1] = '\0';
			ProcessSerial( inputCommandBuffer );
			inputCommandIndex = 0;
		}
	}
}



/**
 * @brief Show serial output
 * 
 */
void ShowSerial() {

	if ( serialRuntimeMillis >= ( 1000 / serialTimerFrequency ) ) {

		// Reset runtime
		serialRuntimeMillis = 0;

		// Display encoder output
		Serial.print( F( "Horizontal = " ) );
		Serial.print( Encoders.GetHorizontalAngleDeg() );
		Serial.print( F( "  " ) );
		Serial.print( F( "Vertical = " ) );
		Serial.print( Encoders.GetVerticalAngleDeg() );

		Serial.print( F( "    " ) );

		// Display gamepad output
		Serial.print( F( "Direction = " ) );
		Serial.print( Gamepad.GetCombinedState() );
		Serial.print( F( " (" ) );
		Serial.print( Gamepad.GetCombinedStateString() );
		Serial.print( F( ")" ) );
		Serial.print( F( "  " ) );

		Serial.print( F( "Button = " ) );
		Serial.print( Gamepad.GetButtonState() );
		Serial.print( F( " (" ) );
		Serial.print( Gamepad.GetButtonStateString() );
		Serial.print( F( ")" ) );

		Serial.print( F( "    " ) );

		Serial.println();

		// Toggle LED as heartbeat
		digitalToggleFast( 13 );
	}
}
