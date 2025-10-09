#include <Arduino.h>

/** Keyboard variables */
struct KeyboardGlobals {

	// Constants
	uint8_t CONST_UPDATE_FREQUENCY = 10;	// Frequency to update keyboard input monitor at

	// Variables
	const char* incomingBuffer = nullptr;	 // Incoming keyboard buffer
};


/** Amplifier variables */
struct AmplifierGlobals {

	// Constants
	uint16_t CONST_UPDATE_FREQUENCY_DRIVE_AMPLIFIER = 1000;	   // Frequency to drive amplifier
	uint16_t CONST_UPDATE_FREQUENCY_SERIAL			= 10;	   // Frequency to read serial data / encoders

	// Variables
	float angleADegrees = 0.0f;
	float angleBDegrees = 0.0f;
	float angleCDegrees = 0.0f;
};

/** Arm Encoder variables */
struct ArmEncoderGlobals {

	// Constants

	// Variables
	float horizontalAngleDegrees;	 // Horizontal (x-axis) angle in degrees
	float VerticalAngleDegrees;		 // Vertical (y-axis) angle in degrees
};

/** Gamepad variables */
struct GamepadGlobals {

	// Variables
	bool   isButtonPressed = false;	   // Status of button
	int8_t buttonPressedIndex;		   // Index of pressed button
	String buttonPressedName;		   // Name of button pressed
};