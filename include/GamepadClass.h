/**
 * @file GamepadClass.h
 * @author Tomasz Trzpit
 * @brief Read inputs from resistor-ladder-based gamepad
 * @version 0.1
 * @date 2025-10-02
 * 
 */

#pragma once

// Pre-built libraries
#include <Arduino.h>



class GamepadClass {

private:
	// Initialize class
	void ConfigurePins();				 // Initialize Teensy pins
	void InitializeThresholdArrays();	 // Initializes the threshold array values
	void MapButtonValues();				 // Map the analog button values into their outputs

	// Gamepad pins
	uint8_t PIN_GAMEPAD_CARDINAL = 41;
	uint8_t PIN_GAMEPAD_DIAGONAL = 40;
	uint8_t PIN_GAMEPAD_BUTTONS	 = 39;

	// Analog read tolerance
	float analogReadTolerance = 0.1f;

	// Local variables
	int8_t cardinalStateValue = -1;	   // Value of cardinal button pressed
	int8_t diagonalStateValue = -1;	   // Value of diagonal button pressed
	int8_t buttonStateValue	  = -1;	   // Value of options button pressed
	int8_t combinedStateValue = -1;	   // Value of cardinal+diagonal button pressed

	String cardinalStateString = "";	// String name of cardinal button pressed
	String diagonalStateString = "";	// String name of diagonal button pressed
	String buttonStateString   = "";	// String name of options button pressed
	String combinedStateString = "";	// String name of cardinal+diagonal button pressed

	uint16_t cardinalRawValue = 0;	  // Raw value of cardinal analog read
	uint16_t diagonalRawValue = 0;	  // Raw value of diagonal analog read
	uint16_t buttonRawValue	  = 0;	  // Raw value of button analog read

	inline static int16_t cardinalThresholdArray[4] = { 0, 245, 517, 777 };	   // Array of thresholds
	inline static int16_t diagonalThresholdArray[4] = { 0, 245, 517, 777 };	   // Array of thresholds
	inline static int16_t buttonThresholdArray[2]	= { 0, 777 };			   // Array of thresholds

	inline static int8_t cardinalIndexValueArray[4] = { 1, 0, 2, 3 };	 // Array of button index values
	inline static int8_t diagonalIndexValueArray[4] = { 2, 3, 0, 1 };	 // Array of button index values
	inline static int8_t buttonIndexValueArray[2]	= { 0, 1 };			 // Array of button index values

	inline static String cardinalStringArray[4] = { "RIGHT", "UP", "DOWN", "LEFT" };					   // Array of string names
	inline static String diagonalStringArray[4] = { "DOWN+LEFT", "UP+LEFT", "UP+RIGHT", "DOWN+RIGHT" };	   // Array of string names
	inline static String buttonStringArray[2]	= { "RED", "GREEN" };									   // Array of string names


	int16_t cardinalThresholdMinArray[4];	 // Array of minimum threshold values
	int16_t diagonalThresholdMinArray[4];	 // Array of minimum threshold values
	int16_t buttonThresholdMinArray[2];		 // Array of minimum threshold values

	int16_t cardinalThresholdMaxArray[4];	 // Array of maximum threshold values
	int16_t diagonalThresholdMaxArray[4];	 // Array of maximum threshold values
	int16_t buttonThresholdMaxArray[2];		 // Array of maximum threshold values



public:
	// Constructor
	GamepadClass();

	// Public functions
	void   PollButtons();
	int8_t GetCardinalState();
	String GetCardinalStateString();
	int8_t GetDiagonalState();
	String GetDiagonalStateString();
	int8_t GetCombinedState();
	String GetCombinedStateString();
	int8_t GetButtonState();
	String GetButtonStateString();
};