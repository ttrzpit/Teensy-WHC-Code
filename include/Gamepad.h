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


	/*****************
	*  Constructors  *
	******************/
	public:
	GamepadClass();												// Default constructor
	GamepadClass( const GamepadClass& )			   = delete;	// Prevent duplicate instances
	GamepadClass& operator=( const GamepadClass& ) = delete;	// Prevent duplicate instances
	GamepadClass( GamepadClass&& )				   = delete;	// Prevent duplicate instances
	GamepadClass& operator=( GamepadClass&& )	   = delete;	// Prevent duplicate instances

	/******************
	*  Configuration  *
	*******************/
	private:
	void	ConfigurePins();			  // Initialize Teensy pins
	uint8_t PIN_GAMEPAD_CARDINAL = 41;	  // Analog input pin for cardinal inputs
	uint8_t PIN_GAMEPAD_DIAGONAL = 40;	  // Analog input pin for diagonal inputs
	uint8_t PIN_GAMEPAD_BUTTONS	 = 39;	  // Analog input pin for button inputs

	public:
	void Loop();	// Runs every loop
	private:
	void ParseButtonPress();	// Parses the button press into its output and name
	/**************
	*  Accessors  *
	***************/
	public:
	void   Check( bool& isPressed, int8_t& buttonIndex, String& buttonName );	 // Call from main every loop
	void   Begin();																 // Start class and initialize
	void   PollButtons();														 // Reads the state of the buttons
	int8_t GetCardinalState();													 // Get indexed button state
	String GetCardinalStateString();											 // Get button state string
	int8_t GetDiagonalState();													 // Get indexed button state
	String GetDiagonalStateString();											 // Get button state string
	int8_t GetCombinedState();													 // Get indexed button state
	String GetCombinedStateString();											 // Get button state string
	int8_t GetButtonState();													 // Get indexed button state
	String GetButtonStateString();												 // Get button state string
	int8_t GetGamepadState();													 // Get state for ALL buttons
	String GetGamepadStateString();												 // Get state string name for ALL buttons

	/*********************
	*  Gamepad Elements  *
	**********************/
	private:
	void				  InitializeThresholdArrays();															// Initializes the threshold array values
	void				  MapButtonValues();																	// Map the analog button values into their outputs
	bool				  isNewStateReady			 = false;													// Whether button press is ready
	float				  analogReadTolerance		 = 0.1f;													// Analog read tolerance
	int8_t				  cardinalStateValue		 = -1;														// Value of cardinal button pressed
	int8_t				  diagonalStateValue		 = -1;														// Value of diagonal button pressed
	int8_t				  buttonStateValue			 = -1;														// Value of options button pressed
	int8_t				  combinedStateValue		 = -1;														// Value of cardinal+diagonal button pressed
	int8_t				  combinedStateValuePrev	 = -1;														// Value of cardinal+diagonal button pressed
	String				  cardinalStateString		 = "";														// String name of cardinal button pressed
	String				  diagonalStateString		 = "";														// String name of diagonal button pressed
	String				  buttonStateString			 = "";														// String name of options button pressed
	String				  combinedStateString		 = "";														// String name of cardinal+diagonal button pressed
	String				  combinedStateStringPrev	 = "";														// String name of cardinal+diagonal button pressed
	uint16_t			  cardinalRawValue			 = 0;														// Raw value of cardinal analog read
	uint16_t			  diagonalRawValue			 = 0;														// Raw value of diagonal analog read
	uint16_t			  buttonRawValue			 = 0;														// Raw value of button analog read
	inline static int16_t cardinalThresholdArray[4]	 = { 0, 245, 517, 777 };									// Array of thresholds
	inline static int16_t diagonalThresholdArray[4]	 = { 0, 245, 517, 777 };									// Array of thresholds
	inline static int16_t buttonThresholdArray[2]	 = { 0, 777 };												// Array of thresholds
	inline static int8_t  cardinalIndexValueArray[4] = { 1, 0, 2, 3 };											// Array of button index values
	inline static int8_t  diagonalIndexValueArray[4] = { 2, 3, 0, 1 };											// Array of button index values
	inline static int8_t  buttonIndexValueArray[2]	 = { 0, 1 };												// Array of button index values
	inline static String  cardinalStringArray[4]	 = { "RIGHT", "UP", "DOWN", "LEFT" };						// Array of string names
	inline static String  diagonalStringArray[4]	 = { "DOWN+LEFT", "UP+LEFT", "UP+RIGHT", "DOWN+RIGHT" };	// Array of string names
	inline static String  buttonStringArray[2]		 = { "RED", "GREEN" };										// Array of string names
	int16_t				  cardinalThresholdMinArray[4];															// Array of minimum threshold values
	int16_t				  diagonalThresholdMinArray[4];															// Array of minimum threshold values
	int16_t				  buttonThresholdMinArray[2];															// Array of minimum threshold values
	int16_t				  cardinalThresholdMaxArray[4];															// Array of maximum threshold values
	int16_t				  diagonalThresholdMaxArray[4];															// Array of maximum threshold values
	int16_t				  buttonThresholdMaxArray[2];															// Array of maximum threshold values

	/*********************
	*  Debounce Elements  *
	**********************/
	private:
	int8_t		 lastStableState = -1;
	int8_t		 debounceCounter = 0;
	const int8_t debounceLimit	 = 3;
};