/**
 * @file main.cpp
 * @author Tomasz Trzpit
 * @brief NURing firmware suite
 * @version 0.1
 * @date 2025-10-02
 * 
 */



// === PROJECT HEADERS ============================================================================

// Pre-built libraries
#include <Arduino.h>

// Custom libraries
#include "Amplifier.h"			// BLDC amplifiers
#include "ArmEncoders.h"		// Arm encoders on test platform
#include "Gamepad.h"			// Gamepad buttons
#include "SerialInterface.h"	// Keyboard serial input
#include "SharedMemory.h"		// Shared memory management
#include "TaskManager.h"		// Task manager



// === Class Objects ==============================================================================

AmplifierClass			Amplifier;			   // Control amplifier interactions
ArmEncoderClass			ArmEncoders;		   // Read experimental platform encoders
SerialInterfaceClass	SerialInterface;	   // Software serial I/O handling
GamepadClass			Gamepad;			   // Read experimental platform gamepad
TaskManagerRuntimeClass TaskManagerRuntime;	   // Task manager



// === INTERVAL TIMERS AND CALLBACKS ===========================================================
IntervalTimer IT_ReadAmplifierSensorsTimer;
IntervalTimer IT_AmplifierOutputTimer;
IntervalTimer IT_DisplaySerialOutputTimer;	  // Serial scroll timer (2 hz)

void ITCALLBACK_DisplaySerialOutput();	   // Prints the system serial scroll
void ITCALLBACK_ReadAmplifierSensors();	   // Update the amplifer throught he interval timer
void ITCALLBACK_AmplifierOutput();

// === Forward Declarations =======================================================================

void ToggleGlobalFlags();		 // Collection of flags to toggle
void ActionQueueManager();		 // Handles waiting actions
void RunSystemStateMachine();	 // System-wide State machine
void RunTaskStateMachine();		 // Task-specific State machine

// void CheckPeripherals();				   // Update all peripherals (spins every loop)
// void CheckGamepadInput();				   // Parse gamepad inputs
// void ITCALLBACK_DriveAmplifiers();					   // Send drive command to amplifiers
// void StateMachine();					   // System state machine
// void State_MeasuringROM();				   // Measure ROM limits for encoders
// void State_MeasuringCurrent();			   // Measure current limits for encoders



// // === Global Structs =============================================================================
// KeyboardGlobals	  varKeyboard;		 // Keyboard input variables
// AmplifierGlobals  varAmplifier;		 // Amplifier variables
// ArmEncoderGlobals varArmEncoders;	 // Encoder variables
// GamepadGlobals	  varGamepad;		 // Gamepad variables



/**
 * @brief Collection of flags to toggle
 */
void ToggleGlobalFlags() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Toggle flags
	Shared->Interface.SWSerial.isScrollingLineEnabled = false;

	// Serial output flags
	Shared->Interface.SWSerial.Toggle.showMotorAngles	   = true;
	Shared->Interface.SWSerial.Toggle.showMotorCurrents	   = true;
	Shared->Interface.SWSerial.Toggle.showMotorPwmOutputs  = true;
	Shared->Interface.SWSerial.Toggle.showPlatformEncoders = true;
	Shared->Interface.SWSerial.Toggle.showTaskOutput	   = true;
}



/**
 * @brief Initialize teensy parameters (Arduino style)
 *
 */
void setup() {

	// Start software serial port (over USB)
	Serial.begin( 9600 );
	while ( !Serial );	  // Waiting until serial connected

	// Allow time for Serial interface to spin up
	delay( 1000 );
	Serial.println( "Initializing subsystems..." );

	// Configure debug output
	// Amplifier.EnableDebugOutput( false );

	// Onboard LED
	pinMode( 13, OUTPUT );

	// Initialize serial interface
	SerialInterface.Output.Begin();	   // Software serial interface for outputting text
	SerialInterface.Input.Begin();	   // Software serial interface getting keyboard input

	// Initialize amplifier
	Amplifier.Begin();	  // Amplifier controls for BLDC

	// Initialize platform encoders
	ArmEncoders.Begin();	// Experimental platform encoders

	// Initiasdalize gamepad
	Gamepad.Begin();	// Experimental platform gamepad

	// Start interval timers
	IT_AmplifierOutputTimer.begin( ITCALLBACK_AmplifierOutput, 1000000 / 1000 );
	IT_ReadAmplifierSensorsTimer.begin( ITCALLBACK_ReadAmplifierSensors, 1000000 / 300 );
	IT_DisplaySerialOutputTimer.begin( ITCALLBACK_DisplaySerialOutput, 1000000 / 2 );

	// Delay to clear everything
	delay( 1000 );

	Serial.println( "ALL SYSTEMS NOMINAL." );
	Serial.println();

	// Toggle global flags
	ToggleGlobalFlags();

	// Display state and menu
	// SerialInterface.PrintSystemState();
	// SerialInterface.PrintMenu();
}


/**
 * @brief Main program loop
 *
 */
void loop() {

	// Run actions manager
	ActionQueueManager();

	// Update arm encoders
	ArmEncoders.Loop();

	// Update amplifiers
	Amplifier.Loop();

	// Update gamepad
	Gamepad.Loop();

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// State machine
	RunSystemStateMachine();


	// Shared Memory Alias
	// Check state machine
	// StateMachine();
}



void RunSystemStateMachine() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	switch ( Shared->State.systemState ) {

		// Idling
		case EnumsClass::SystemStateEnum::DISABLED: {

			break;
		}

		case EnumsClass::SystemStateEnum::IDLING: {

			// Enable motor output if safety switch is engaged
			if ( Shared->Drive.Flags.isSafetySwitchEngaged ) {
				Shared->Drive.Flags.isMotorOutputEnabled = true;
			}

			break;
		}

		case EnumsClass::SystemStateEnum::RUNNING_TASK: {

			RunTaskStateMachine();
			break;
		}


		default: {

			break;
		}
	}
}


void RunTaskStateMachine() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Switch to active task
	switch ( Shared->Tasks.activeTask ) {

		case EnumsClass::TaskSelectionEnum::TESTING_CARDINAL_DIRECTIONS: {

			// Callback to task loop
			TaskManagerRuntime.DiscriminationTaskRuntime.CardinalDirectionsRuntime.Loop();

			break;
		}

		case EnumsClass::TaskSelectionEnum::TESTING_OCTANT_DIRECTIONS: {

			// Callback to task loop
			TaskManagerRuntime.DiscriminationTaskRuntime.OctantDirectionsRuntime.Loop();

			break;
		}

		default: {

			Serial.println( F( "Unknown task selected." ) );
			break;
		}
	}
}


// void CheckPeripherals() {



// 	// Read encoders
// 	ArmEncoders.Check( varArmEncoders.horizontalAngleDegrees, varArmEncoders.VerticalAngleDegrees );

// 	// Read gamepad buttons
// 	Gamepad.Check( varGamepad.isButtonPressed, varGamepad.buttonPressedIndex, varGamepad.buttonPressedName );

// 	// Pass gamepad into serial
// 	if ( varGamepad.isButtonPressed ) {
// 		Serial.println( "BUTTON!" );
// 		SerialInterface.UpdateButtonState( varGamepad.isButtonPressed, varGamepad.buttonPressedIndex, varGamepad.buttonPressedName );
// 		TaskManager.EnterResponse( varGamepad.buttonPressedIndex );
// 	}
// }


// /**
//  * @brief Handles gamepad inputs
//  */
// void CheckGamepadInput() {

// 	if ( varGamepad.isButtonPressed ) {
// 		Serial.print( F( "Received gamepad input:\n  " ) );
// 		Serial.print( varGamepad.buttonPressedIndex );
// 		Serial.print( " (" );
// 		Serial.print( varGamepad.buttonPressedName );
// 		Serial.println( F( ")" ) );
// 	}
// }



// // /*  ===================================================================================
// //  *  ===================================================================================
// //  *
// //  *   IIIIII  TTTTTTT      CCCCC    AAA    LL      LL      BBBBB     AAA     CCCCC  KK  KK   SSSSS
// //  *     II       T        CC       AA AA   LL      LL      BB  BB   AA AA   CC      KK KK   SS
// //  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KKKK    SS
// //  *     II       T        CC      AAAAAAA  LL      LL      BBBBB   AAAAAAA  CC      KKKK     SSSSS
// //  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KK KK        SS
// //  *     II       T        CC      AA   AA  LL      LL      BB  BB  AA   AA  CC      KK  KK       SS
// //  *   IIIIII     T         CCCCC  AA   AA  LLLLLL  LLLLLL  BBBBB   AA   AA   CCCCC  KK  KK   SSSSS
// //  *
// //  *  ===================================================================================
// //  *  ===================================================================================*/



/**
 * @brief IntervalTimer callback to read amplifier encoders and current sensors
 */
void ITCALLBACK_ReadAmplifierSensors() {

	// Read sensors
	Amplifier.ReadSensors();
}


/**
 * @brief IntervalTimer callback to drive motor output
 */
void ITCALLBACK_AmplifierOutput() {
	Amplifier.DriveMotorOutputs();
}


/**
 * @brief IntervalTimer callback to show serial scroll
 */
void ITCALLBACK_DisplaySerialOutput() {

	// Shared memory alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Check if scrolling is enabled
	if ( Shared->Interface.SWSerial.isScrollingLineEnabled ) {

		// Print the scroll line
		SerialInterface.Output.PrintStatusLine();
	}
}


// // /*  ===================================================================================
// //  *  ===================================================================================
// //  *
// //  *   SSSSS  YY    YY  SSSSS  TTTTTTTT  EEEEEE  MM    MM       SSSSS  TTTTTTTT    AAA    TTTTTTTT  EEEEEE
// //  *  SS       YY  YY  SS         TT     EE      MMM  MMM      SS         TT      AA AA      TT     EE
// //  *  SS        YYYY   SS         TT     EE      MM MM MM      SS         TT     AA   AA     TT     EE
// //  *   SSSS      YY     SSSS      TT     EEEE    MM    MM       SSSS      TT     AAAAAAA     TT     EEEE
// //  *      SS     YY         SS    TT     EE      MM    MM           SS    TT     AA   AA     TT     EE
// //  *      SS     YY         SS    TT     EE      MM    MM           SS    TT     AA   AA     TT     EE
// //  *   SSSS      YY     SSSSS     TT     EEEEEE  MM    MM       SSSSS     TT     AA   AA     TT     EEEEEE
// //  *
// //  *  ===================================================================================
// //  *  ===================================================================================*/


// /**
//  * @brief Main state machine
//  */
// void StateMachine() {

// 	// System is idle
// 	if ( SYSTEM_STATE == systemStateEnum::IDLE ) {

// 		// Serial.println( "IDLE" );
// 	}
// 	// System is measuring range of motion
// 	if ( SYSTEM_STATE == systemStateEnum::MEASURING_RANGE_OF_MOTION ) {

// 		// Serial.println( "MEASURING_ROM" );
// 	}
// 	// System is measuring current limits
// 	if ( SYSTEM_STATE == systemStateEnum::DISCRIMINATION_TASK_TESTING ) {

// 		// Run update
// 		Serial.print( "o" );
// 		TaskManager.UpdateCardinalDiscriminationTask();
// 	}
// }



// /********************
//  *  STATE HANDLERS  *
//  ********************/

// /**
//  * @brief Handles Idle state
//  */
// void State_Idle();	  // Measure current limits for encoders


// /**
//  * @brief Handles MeasuringROM state
//  */
// void State_MeasuringROM() { }


// /**
//  * @brief Handles MeasuringCurrent state
//  */
// void State_MeasuringCurrent() { }


void ActionQueueManager() {

	// Shared Memory Alias
	static auto Shared = SYSTEM_GLOBAL.GetData();

	// Actions
	if ( Shared->ActionQueue.zeroPlatformEncoders ) ArmEncoders.ZeroArmEncoders();	  // Zero encoders
	if ( Shared->ActionQueue.zeroMotorEncoders ) Amplifier.ZeroMotorEncoders();		  // Zero motor encoders
	if ( Shared->ActionQueue.setMotorTension ) Amplifier.SetTension();				  // Set tension
}