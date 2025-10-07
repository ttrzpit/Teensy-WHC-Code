/**
 * @file LoggingClass.h
 * @author Tomasz Trzpit
 * @brief Log data to internal memory, then save to SD card
 * @version 0.1
 * @date 2025-10-06
 */

#pragma once

// Standard libraries
#include <Arduino.h>	// For arduino functions
#include <SD.h>			// For SD card
#include <SPI.h>		// For SD card access


// Forward declarations
class AmplifierClass;
class EncoderClass;
class GamepadClass;



class LoggingClass {

	/*****************
	*  Constructors  *
	******************/
	public:
	// Default constructor
	explicit LoggingClass( AmplifierClass& amp, EncoderClass& enc, GamepadClass& gpad );	// Constructor

	private:
	AmplifierClass& Amplifier;	  // Stored reference to amplifiers
	EncoderClass&	Encoders;	  // Stored reference to encoders
	GamepadClass&	Gamepad;	  // Stored reference to gamepad


	/**************
	*  Accessors  *
	***************/
	public:
	void Begin();						  // Initialize class
	void Update();						  // Update, called every loop, logs items
	void LogDiscriminationTaskEntry();	  // Log a discrimination task entry
	void StopLogging();					  // Stops logging
	private:
	void InitializeSdCard();	// Initialize the SD card
	void SaveDataToSdCard();	// Saves data to SD card


	/***********
	*  Timing  *
	************/
	private:
	uint16_t	  timerLoggingFrequency = 100;	  // Frequency of logging timer
	elapsedMillis timerRuntimeMillis;			  // Running timer in milliseconds
	elapsedMillis timerTaskRuntimeMillis;		  // Running timer in milliseconds

	/************
	*  Logging  *
	*************/
	private:
	void				LogEntry();					 // Save entry to vector
	String				filename = "";				 // Filename to save as
	std::vector<String> vectorDiscriminationTask;	 // Vector to store logging entries
	bool				isLoggingStarted = false;	 // Flag to determine if logger is running
	bool				isSdCardPresent	 = false;	 // Flag for SD card


	/*****************
	*  Task Logging  *
	******************/
	private:
	String	 presentedCue		   = "";	// Cue presented
	uint32_t presentedCueStartTime = 0;		// Cue start time
	String	 userCueResponse	   = "";	// User response
	uint32_t userCueResponseTime   = 0;		// User response time
};