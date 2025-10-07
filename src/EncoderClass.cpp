#include "EncoderClass.h"

EncoderClass::EncoderClass()
	: encoderHorizontal( PIN_ENCODER_HOR_A, PIN_ENCODER_HOR_B )
	, encoderVertical( PIN_ENCODER_VER_A, PIN_ENCODER_VER_B ) { }


/**
 * @brief Initialize class
 */
void EncoderClass::Begin() {

	// Configure pins
	ConfigurePins();

	delay( 250 );
	Serial.println( F( "Platform encoder initialization...            success!" ) );
}


/**
 * @brief Get the newest horizontal angle
 * @return float Angle in degrees
 */
float EncoderClass::GetHorizontalAngleDeg() {
	return horizontalAngleDegrees;
}



/**
 * @brief Get the newest vertical angle
 * @return float Angle in degrees
 */
float EncoderClass::GetVerticalAngleDeg() {
	return verticalAngleDegrees;
}


/**
 * @brief Initialize the pins on the Teensy
 * 
 */
void EncoderClass::ConfigurePins() {

	// Initialize pins
	pinMode( PIN_ENCODER_HOR_A, INPUT );
	pinMode( PIN_ENCODER_HOR_B, INPUT );
	pinMode( PIN_ENCODER_HOR_X, INPUT );
	pinMode( PIN_ENCODER_VER_A, INPUT );
	pinMode( PIN_ENCODER_VER_B, INPUT );
	pinMode( PIN_ENCODER_VER_X, INPUT );
}


/**
 * @brief Runs every loop to update encoder class
 */
void EncoderClass::Update() {

	// Poll encoders
	PollEncoders();
}



/**
 * @brief Read both encoders
 */
void EncoderClass::PollEncoders() {

	// Read new horizontal values
	horizontalCountNew = encoderHorizontal.read();

	// Update value if count has changed
	if ( horizontalCountNew != horizontalCountOld ) {
		horizontalCountOld = horizontalCountNew;

		// Calculate angle
		horizontalAngleDegrees = -1.0f * degrees( horizontalCountNew * ( 2.0f * M_PI ) / ( 2.0f * 4096.0f ) );
	}

	// Read new vertical values
	verticalCountNew = encoderVertical.read();

	// Update value if count has changed
	if ( verticalCountNew != verticalCountOld ) {
		verticalCountOld = verticalCountNew;

		// Calculate angle
		verticalAngleDegrees = -1.0f * degrees( verticalCountNew * ( 2.0f * M_PI ) / 5000.0f / 4.0f );
	}
}



void EncoderClass::ZeroEncoders() {

	// Write zeros to encoders
	encoderHorizontal.write( 0 );
	encoderVertical.write( 0 );
}