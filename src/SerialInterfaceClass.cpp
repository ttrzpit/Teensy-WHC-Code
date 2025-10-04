#include "SerialInterfaceClass.h"
#include "AmplifierClass.h"
#include "EncoderClass.h"

// Constructor
SerialInterfaceClass::SerialInterfaceClass( AmplifierClass& amp, EncoderClass& enc )
	: Amplifier( amp )
	, Encoders( enc ) {
	Serial.begin( 9600 );
	Serial.println( "Starting serial interface..." );
	delay( 500 );
}


/**
 * @brief Function that gets called every loop to update serial interface
 * 
 */
void SerialInterfaceClass::Update() {

	// Read input
	ReadSerialInput();

	// Display output if timer ticks over
	if ( timerRuntimeMillis >= ( 1000 / timerFrequencyHz ) ) {

		// Reset timer
		timerRuntimeMillis = 0;

		// Show interface
		DisplaySerialInterface();
	}
}



/**
 * @brief Parses serial input commands
 * @param buffer commands to be parsed
 */
void SerialInterfaceClass::ParseSerialInput( const char* buffer ) {

	// Extract key
	char cmd = buffer[0];
	Serial.print( F( "Parsing received command [" ) );
	Serial.print( buffer );
	Serial.print( F( "] --> " ) );

	// Enable amplifier
	if ( ( cmd == 'e' || cmd == 'E' ) && buffer[1] == '\0' ) {

		// Toggle amplifier enable
		if ( Amplifier.FLAGS_GetAmplifierState() ) {
			Amplifier.COMMAND_Disable();
			Serial.println( F( "disabling amplifiers." ) );
		} else {
			Amplifier.COMMAND_Enable();
			Serial.println( F( "enabling amplifiers." ) );
		}
		return;
	}

	// Zero encoders
	if ( ( cmd == 'z' || cmd == 'Z' ) && buffer[1] == '\0' ) {
		Encoders.ZeroEncoders();
		Serial.println( F( "zeroing encoders." ) );
		return;
	}

	// Set tension value
	if ( cmd == 't' || cmd == 'T' ) {
		uint8_t val = atoi( buffer + 1 );

		// Check that tensioning isn't too high
		if ( val <= 25 ) {

			// Update tension value
			Amplifier.COMMAND_SetTension( val );
			Serial.print( F( "setting tension value to " ) );
			Serial.print( val );
			Serial.println( F( "%." ) );
		} else {
			Serial.println( F( "tensioning value too high, ignoring." ) );
		}

		return;
	}

	// Unknown command
	Serial.println( F( "command not understood, ignoring command. " ) );
}


/**
  * @brief Reads serial port input
  */
void SerialInterfaceClass::ReadSerialInput() {

	// Check if data is waiting
	while ( Serial.available() ) {

		// Read character
		char c = ( char )Serial.read();

		// Read end of line
		if ( c == '\n' || c == '\r' ) {
			if ( inputCommandIndex > 0 ) {
				inputCommandBuffer[inputCommandIndex] = '\0';
				ParseSerialInput( inputCommandBuffer );
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
				ParseSerialInput( inputCommandBuffer );
				inputCommandIndex = 0;
			}
		} else {
			// Overflow -> terminate, process, reset
			inputCommandBuffer[sizeof( inputCommandBuffer ) - 1] = '\0';
			ParseSerialInput( inputCommandBuffer );
			inputCommandIndex = 0;
		}
	}
}


/**
 * @brief Displays the serial interface
 */
void SerialInterfaceClass::DisplaySerialInterface() {

	// Select elements to show

	// Platform encoder
	if ( Elements.showPlatformEncoder ) {
		ShowElement_PlatformEncoders();
	}

	// Motor encoders
	if ( Elements.showMotorEncoder ) {
		ShowElement_MotorEncoders();
	}

    // Baud rates
    if ( Elements.showBaudRates ) { 
        ShowElement_BaudRate(); 
    }
}


/*  ============================================================================
 *  ============================================================================
 * 
 *  EEEEEE   LL        EEEEEE   MM      MM   EEEEEE   NN   NN   TTTTTT    SSSS
 *  EE       LL        EE       MMMM  MMMM   EE       NNN  NN     TT      SS
 *  EE       LL        EE       MM MMMM MM   EE       NN N NN     TT      SS
 *  EEEE     LL        EEEE     MM  MM  MM   EEEE     NN  NNN     TT       SSSS
 *  EE       LL        EE       MM      MM   EE       NN   NN     TT          SS
 *  EE       LL        EE       MM      MM   EE       NN   NN     TT          SS
 *  EEEEEE   LLLLL     EEEEEE   MM      MM   EEEEEE   NN   NN     TT        SSSS
 * 
 *  ============================================================================ 
 *  ============================================================================*/

/**
  * @brief Show experimental platform encoder values
  */
void SerialInterfaceClass::ShowElement_PlatformEncoders() {

	// Display encoder output
	Serial.print( F( "Platform Encoders " ) );
	Serial.print( F( "x: " ) );
	Serial.print( Encoders.GetHorizontalAngleDeg() );
	Serial.print( F( " deg,  " ) );
	Serial.print( F( "y: " ) );
	Serial.print( Encoders.GetVerticalAngleDeg() );
	Serial.print( F( " deg" ) );
	Serial.println();
}



/**
  * @brief Show motor encoder values
  */
void SerialInterfaceClass::ShowElement_MotorEncoders() {

	// Display encoder output
	Serial.print( F( "Motor Encoder Counts " ) );
	Serial.print( F( "A: " ) );
	Serial.print( Amplifier.READ_GetCountA() );
	Serial.print( F( ", B: " ) );
	Serial.print( Amplifier.READ_GetCountB() );
	Serial.print( F( ", C: " ) );
	Serial.print( Amplifier.READ_GetCountC() );
	Serial.println();
}



/**
  * @brief Show amplifier baud rates
  */
void SerialInterfaceClass::ShowElement_BaudRate() {

	// Display encoder output
	Serial.print( F( "Amplifier Baud Rate " ) );
	Serial.print( F( "A: " ) );
	Serial.print( Amplifier.HWSerial.AmpProperty.baudRateA );

	Serial.println();
}