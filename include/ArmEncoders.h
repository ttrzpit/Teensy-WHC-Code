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

	/*****************
	*  Constructors  *
	******************/
	public:
	EncoderClass();	   // Default constructor and safeguards to prevent multiple amplifier instances
	EncoderClass( const EncoderClass& )			   = delete;
	EncoderClass& operator=( const EncoderClass& ) = delete;
	EncoderClass( EncoderClass&& )				   = delete;
	EncoderClass& operator=( EncoderClass&& )	   = delete;

	/******************
	*  Configuration  *
	*******************/
	private:
	void   ConfigurePins();			  // Initialize Teensy pins
	int8_t PIN_ENCODER_HOR_A = 10;	  // Horizontal encoder A channel
	int8_t PIN_ENCODER_HOR_B = 11;	  // Horizontal encoder B channel
	int8_t PIN_ENCODER_HOR_X = 12;	  // Horizontal encoder X channel
	int8_t PIN_ENCODER_VER_A = 4;	  // Vertical encoder A channel
	int8_t PIN_ENCODER_VER_B = 5;	  // Vertical encoder B channel
	int8_t PIN_ENCODER_VER_X = 6;	  // Vertical encoder X channel

	/**************
	*  Accessors  *
	***************/
	public:
	void  Begin();										   // Initializes class
	void  Update( float& horizontal, float& vertical );	   // Run every loop
	float GetHorizontalAngleDeg();						   // Get horizontal angle in degrees
	float GetVerticalAngleDeg();						   // Get vertical angle in degrees
	void  ZeroEncoders();								   // Zero encoder values
	private:
	void PollEncoders();	// Poll encoder values


	/*********************
	*  Encoder Elements  *
	**********************/
	private:
	int64_t horizontalCountOld	   = 0;		  // Previous horizontal encoder count [COUNTS]
	int64_t horizontalCountNew	   = 0;		  // New horizontal encoder count [COUNTS]
	int64_t verticalCountOld	   = 0;		  // Previous vertical encoder count [COUNTS]
	int64_t verticalCountNew	   = 0;		  // New vertical encoder count [COUNTS]
	float	horizontalAngleDegrees = 0.0f;	  // Horizontal encoder angle [DEG]
	float	verticalAngleDegrees   = 0.0f;	  // Vertical encoder angle [DEG]
	Encoder encoderHorizontal;				  // Horizontal encoder objects
	Encoder encoderVertical;				  // Vertical encoder objects
};
