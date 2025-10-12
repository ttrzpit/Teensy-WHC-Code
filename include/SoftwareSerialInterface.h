/**
 * @file SerialOutputClass.h
 * @author Tomasz Trzpit
 * @brief Handle serial output
 * @version 0.1
 * @date 2025-10-03
 * 
 */

#pragma once

// Pre-built libraries
#include <Arduino.h>	// For arduino functions

// Libraries for other classes passed by reference
#include "Amplifier.h"
#include "ArmEncoders.h"



/*  ==========================================================
 *  ==========================================================
 * 
 *   SSSS   TTTTTT  RRRRR    U     U   CCCCC    TTTTTT   SSSS
 *  SS        TT    RR  RR   U     U   CC         TT    SS
 *  SS        TT    RR  RR   U     U   CC         TT    SS
 *   SSSS     TT    RRRRR    U     U   CC         TT     SSSS
 *      SS    TT    RR  RR   U     U   CC         TT        SS
 *      SS    TT    RR  RR   U     U   CC         TT        SS
 *   SSSS     TT    RR   RR   UUUUU    CCCCC      TT     SSSS
 * 
 *  ==========================================================
 *  ==========================================================
 */



// Forward declarations
class AmplifierClass;	  // Amplifier
class ArmEncoderClass;	  // Arm encoders

// System state
enum class systemStateEnum { IDLE, MEASURING_RANGE_OF_MOTION, MEASURING_CURRENT_LIMITS };	 // Enum of states


class SoftwareSerialInterfaceClass {

	/************************
	*  Serial Constructors  *
	*************************/
	public:
	SoftwareSerialInterfaceClass( systemStateEnum& systemStateRef, AmplifierClass& ampRef, ArmEncoderClass& armEncRef );	// Default constructor
	static SoftwareSerialInterfaceClass* instance;																			// Singleton-style hook for global ISR shim

	private:
	systemStateEnum& SystemStateReference;
	AmplifierClass&	 AmplifierReference;	 // Reference to amplifier
	ArmEncoderClass& ArmEncoderReference;	 // Reference to arm encoder



	/*******************
	*  Initialization  *
	********************/
	public:
	void Begin();	 // Initialize class


	/**************
	*  Accessors  *
	***************/
	public:
	void GetKeyboardInput();

	/**************************
	*  Keyboard Serial Input  *
	***************************/
	private:
	void RespondToKeyboardCommands();								// Main function to parse commands into responses
	void Respond_SetTension();										// Set tension
	void Respond_PrintSystemStateBlock();							// Print system state
	void Respond_ToggleScrollingState();							// Toggle system state scroll
	void Respond_ZeroPlatformEncoders();							// Zero platform encoders
	void Respond_ZeroMotorEncoders();								// Zero motor encoders
	void Respond_ToggleAmplifierOutput();							// Toggle system output
	void Respond_ToggleCurrentLimits();								// Toggle current limiter
	void Respond_MeasureAmplifierRangeOfMotionLimits(  );	// Toggle ROM measurement

	String incomingSerialString = "";	 // Serial string being received

	public:
	void OnSoftwareSerialEvent();	 // Instance handler for software serial

	/******************
	*  Serial Output  *
	*******************/
	public:
	void PrintMenu();						  // Draw on-screen menu
	void PrintSystemState();				  // Prints the system state once
	void PrintScrollingOutput();			  // Prints a scrolling system state line
	bool isScrollingOutputEnabled = false;	  // Flag for scrolling output



	/****************
	*  SystemState  *
	*****************/
	private:
	systemStateEnum SYSTEM_STATE = systemStateEnum::IDLE;	 // Enumeration of states
	String			currentState = "IDLE";					 // String of current system state
	// systemStateEnum& SystemStateReference;					  // System state reference



	/*********************
	*  Display Elements  *
	**********************/
	private:
	void ShowElement_PlatformEncoders();	  // Show the platform encoder values
	void ShowElement_MotorEncoderCounts();	  // Show motor encoder values
	void ShowElement_MotorEncoderAngles();	  // Show motor encoder values
	void ShowElement_BaudRate();			  // Show encoder baud rat
};