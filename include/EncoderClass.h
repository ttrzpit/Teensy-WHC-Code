/**
 * @file EncoderClass.h
 * @author Tomasz Trzpit
 * @brief Read values from X and Y encoders
 * @version 0.1
 * @date 2025-10-02
 * 
 */

#pragma once

// Pre-built libraries
#include <Arduino.h>	// For arduino functions
#include <Encoder.h>	// For reading encoders



class EncoderClass {


private:
	// Initialize class
	void ConfigurePins();	  // Initialize Teensy pins

	// Encoder pins
	int8_t PIN_ENCODER_HOR_A = 10;	  // Horizontal encoder A channel
	int8_t PIN_ENCODER_HOR_B = 11;	  // Horizontal encoder B channel
	int8_t PIN_ENCODER_HOR_X = 12;	  // Horizontal encoder X channel
	int8_t PIN_ENCODER_VER_A = 4;	  // Vertical encoder A channel
	int8_t PIN_ENCODER_VER_B = 5;	  // Vertical encoder B channel
	int8_t PIN_ENCODER_VER_X = 6;	  // Vertical encoder X channel

	// Local variables
	int64_t horizontalCountOld	   = 0;		  // Previous horizontal encoder count [COUNTS]
	int64_t horizontalCountNew	   = 0;		  // New horizontal encoder count [COUNTS]
	int64_t verticalCountOld	   = 0;		  // Previous vertical encoder count [COUNTS]
	int64_t verticalCountNew	   = 0;		  // New vertical encoder count [COUNTS]
	float	horizontalAngleDegrees = 0.0f;	  // Horizontal encoder angle [DEG]
	float	verticalAngleDegrees   = 0.0f;	  // Vertical encoder angle [DEG]

	// Encoder objects
	Encoder encoderHorizontal;
	Encoder encoderVertical;


public:
	// Constructor with data manager handle
	EncoderClass();

	// Public functions
	void  PollEncoders();
	float GetHorizontalAngleDeg();
	float GetVerticalAngleDeg();
	void  ZeroEncoders();
};
