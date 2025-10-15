#include "ArmEncoders.h"
#include "SharedMemory.h"



ArmEncoderClass::ArmEncoderClass()
	: encoderHorizontal( PIN_ENCODER_HOR_A, PIN_ENCODER_HOR_B )
	, encoderVertical( PIN_ENCODER_VER_A, PIN_ENCODER_VER_B ) { }


/**
 * @brief Initialize class
 */
void ArmEncoderClass::Begin() {

	// Configure pins
	ConfigurePins();

	delay( 250 );
	Serial.println( F( "PLATFORM:      Arm encoder interface...                Ready." ) );
}

/**
 * @brief Updates every loop
 */
void ArmEncoderClass::Loop() {

	// Poll encoders
	PollEncoders();
}

/**
 * @brief Get the newest horizontal angle
 * @return float Angle in degrees
 */
float ArmEncoderClass::GetHorizontalAngleDeg() {
	return horizontalAngleDegrees;
}



/**
 * @brief Get the newest vertical angle
 * @return float Angle in degrees
 */
float ArmEncoderClass::GetVerticalAngleDeg() {
	return verticalAngleDegrees;
}


/**
 * @brief Initialize the pins on the Teensy
 * 
 */
void ArmEncoderClass::ConfigurePins() {

	// Initialize pins
	pinMode( PIN_ENCODER_HOR_A, INPUT );
	pinMode( PIN_ENCODER_HOR_B, INPUT );
	pinMode( PIN_ENCODER_HOR_X, INPUT );
	pinMode( PIN_ENCODER_VER_A, INPUT );
	pinMode( PIN_ENCODER_VER_B, INPUT );
	pinMode( PIN_ENCODER_VER_X, INPUT );
}



/**
 * @brief Read both encoders
 */
void ArmEncoderClass::PollEncoders() {


	// Shared memory alias
	static auto System = SYSTEM_GLOBAL.GetData();

	// Read new horizontal values
	horizontalCountNew = encoderHorizontal.read();

	// Update value if count has changed
	if ( horizontalCountNew != horizontalCountOld ) {
		horizontalCountOld = horizontalCountNew;

		// Calculate angle
		horizontalAngleDegrees									= -1.0f * degrees( horizontalCountNew * ( 2.0f * M_PI ) / ( 2.0f * 4096.0f ) );
		System->Sensors.PlatformEncoders.horizontalAngleDegrees = horizontalAngleDegrees;
	}

	// Read new vertical values
	verticalCountNew = encoderVertical.read();

	// Update value if count has changed
	if ( verticalCountNew != verticalCountOld ) {
		verticalCountOld = verticalCountNew;

		// Calculate angle
		verticalAngleDegrees								  = -1.0f * degrees( verticalCountNew * ( 2.0f * M_PI ) / 5000.0f / 4.0f );
		System->Sensors.PlatformEncoders.verticalAngleDegrees = horizontalAngleDegrees;
	}
}



void ArmEncoderClass::ZeroArmEncoders() {

	// Write zeros to encoders
	encoderHorizontal.write( 0 );
	encoderVertical.write( 0 );

	// Reset action flag
	SYSTEM_GLOBAL.ResetActionFlag( SYSTEM_GLOBAL.GetData()->ActionQueue.zeroPlatformEncoders );
}