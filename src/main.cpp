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
#include "SerialInterfaceClass.h"

// Create class objects
EncoderClass		 Encoders;							   // Read experimental platform encoders
GamepadClass		 Gamepad;							   // Read experimental platform gamepad
AmplifierClass		 Amplifier;							   // Control amplifier interactions
SerialInterfaceClass Interface { Amplifier, Encoders };	   // Software serial I/O handling

// Interval timer
IntervalTimer IT_MainControllerTimer;



// Forward declaration
void UpdatePeripherals();



/**
 * @brief Initialize teensy parameters (Arduino style)
 * 
 */
void setup() {

	// Onboard LED
	pinMode( 13, OUTPUT );

	// Initialize classes at specified frequencies (-1 = unlimited Hz)
	Interface.Begin();	  // Software serial interface for reading keyboard input
	Amplifier.Begin();	  // Amplifier controls for BLDC
	Encoders.Begin();	  // Experimental platform encoders
	Gamepad.Begin();	  // Experimental platform gamepad

	delay( 1000 );
	// IT_MainControllerTimer.begin( IT_MainController_Callback, 1000 );
}


/**
 * @brief Main program loop
 * 
 */
void loop() {

	// Update hardware peripherals
	UpdatePeripherals();

	if ( Gamepad.GetCombinedStateString() != "" ) {
		Serial.println( Gamepad.GetCombinedStateString() );
	}
}


void UpdatePeripherals() {

	// Read encoders
	Encoders.Update();

	// Read gamepad buttons
	Gamepad.Update();

	// Update amplifier state
	Amplifier.Update();

	// Update serial interface
	Interface.Update();
}
