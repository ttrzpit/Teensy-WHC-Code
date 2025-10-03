/**
 * @file Amplifier.h
 * @author Tomasz Trzpit
 * @brief Control copley nano amplifiers
 * @version 0.1
 * @date 2025-10-02
 * 
 */

#pragma once

// Pre-built libraries
#include <Arduino.h>	// For arduino functions

// Hardware serial ports
#define HWSerialA Serial5   // AdEx
#define HWSerialB Serial4   // AbEx
#define HWSerialC Serial3   // Flex

class AmplifierClass {

private:
	// Initialize pins
	void InitializePins();
	void InitializeSerialInterface();


	// Amplifier hardware pins
	int8_t PIN_AMPLIFIER_ENABLE_A = 35;	   // Enables amplfier A (and corresponding LED)
	int8_t PIN_AMPLIFIER_ENABLE_B = 34;	   // Enables amplfier B (and corresponding LED)
	int8_t PIN_AMPLIFIER_ENABLE_C = 33;	   // Enables amplfier C (and corresponding LED)
	int8_t PIN_AMPLIFIER_PWM_A	  = 7;	   // Sends PWM commands to amplifier A
	int8_t PIN_AMPLIFIER_PWM_B	  = 8;	   // Sends PWM commands to amplifier B
	int8_t PIN_AMPLIFIER_PWM_C	  = 25;	   // Sends PWM commands to amplifier C
	int8_t PIN_AMPLIFIER_LED_A	  = 30;	   // Blue LED
	int8_t PIN_AMPLIFIER_LED_B	  = 31;	   // Blue LED
	int8_t PIN_AMPLIFIER_LED_C	  = 32;	   // Blue LED
	int8_t PIN_AMPLIFIER_SAFETY	  = 9;	   // Safety switch input (can be overriden on control board as well)

public:
	// Public functions
	bool GetAmplifierState();														   // Get the amplifier state
	bool GetSafetySwitchState();													   // Get the state of the safety switch
	void Disable();																	   // Disable amplifiers
	void Enable();																	   // Enable amplifiers
	void DriveXY( int8_t percentageX, int8_t percentageY );							   // Drive amplifiers through XY commands
	void DriveABC( uint8_t percentageA, uint8_t percentageB, uint8_t percentageC );	   // Drive amplifiers through ABC commands
};