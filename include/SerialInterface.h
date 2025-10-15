
#pragma once

// Pre-built libraries
#include <Arduino.h>	// For arduino functions


// Forward declarations
class FlagsClass;
class OutputClass;
class InputClass;
class ResponseClass;

class FlagsClass {

	// Flags
	public:
	// bool showMotorLimits	 = false;	 // Toggle motor limits
	// bool showMotorPower		 = false;	 // Toggle motor power
	// bool showMotorEncoder	 = false;	 // Toggle motor encoder
	// bool showPlatformEncoder = false;	 // Toggle platform encoder
	// bool showGamepadState	 = false;	 // Toggle gamepad
	// bool showTaskOutput		 = false;	 // Task output
};

class OutputClass {

	public:
	FlagsClass Toggle;	  // Container for flags

	public:
	void Begin();

	void PrintStatusLine();	   // Prints the system status in a single line
};

class InputClass {


	public:
	void Begin();	 // Initialize input
	void Loop();	 // Runs every cycle

	private:
	String incomingSerialString = "";	 // Serial string being received

	// === EVENT HANDLERS =======================
	public:
	void OnSoftwareSerialEvent();	 // Callback for software serial event

	private:
	void RespondToKeyboardInput();	  // Parses and responds to keyboard inputs

	// Responses
	private:
	void SetScrollingOutputEnabled();
	void SetAmplifierOutputEnabled();
	void SetPlatformEncodersZero();
	void SetMotorEncodersZero();
	void SetMotorTension();
	void SetTensionEnabled();
	void SetDiscriminationTaskCardinalStart();
	void SetDiscriminationTaskOctantStart();
};



class SerialInterfaceClass {

	/************************
	*  Serial Constructors  *
	*************************/
	public:
	SerialInterfaceClass();	   // Default constructor
	static SerialInterfaceClass* instance;

	/*********************
	*  Public Accessors  *
	**********************/
	public:
	void Begin();	 // Initialize class



	/*********************
	*  Sub-classes  *
	**********************/
	public:
	OutputClass Output;	   // Container for output
	InputClass	Input;	   // Container for inputs
};