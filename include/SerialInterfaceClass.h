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

/**
  * @brief Struct of display elements to be toggled
  */
struct DisplayElementsStruct {

	// Flags to toggle in display
	bool showPlatformEncoder	 = false;
	bool showMotorEncoderCounts	 = false;
	bool showMotorEncoderDegrees = false;
	bool showMotorName			 = false;
	bool showBaudRates			 = false;
};



// Forward declarations
class AmplifierClass;
class EncoderClass;

class SerialInterfaceClass {


	/************************
	*  Serial Constructors  *
	*************************/
	public:
	// Default constructor
	explicit SerialInterfaceClass( AmplifierClass& amp, EncoderClass& enc );	// Constructor

	private:
	AmplifierClass& Amplifier;	  // Stored reference
	EncoderClass&	Encoders;

	/**************
	*  Accessors  *
	***************/
	public:
	void Update();	  // Displays serial port text and reads inputs
	void Begin();	  // Initialize class

	/*******************
	*  Debugging Info  *
	********************/

	/*******************
	*  Keyboard Input  *
	********************/
	private:
	char	inputCommandBuffer[8];					   // Buffer to store incoming commands
	uint8_t inputCommandIndex = 0;					   // Current read buffer index
	void	ReadSerialInput();						   // Read serial inputs
	void	ParseSerialInput( const char* buffer );	   // Process serial input

	/******************
	*  Serial Output  *
	*******************/
	private:
	uint16_t	  timerFrequencyHz = 1;		   // Frequency to display timer outputs
	elapsedMillis timerRuntimeMillis;		   // Running timer in milliseconds
	void		  DisplaySerialInterface();	   // Shows the serial interface
	public:
	DisplayElementsStruct Elements;

	/*********************
	*  Display Elements  *
	**********************/
	void ShowElement_PlatformEncoders();	  // Show the platform encoder values
	void ShowElement_MotorEncoderCounts();	  // Show motor encoder values
	void ShowElement_MotorEncoderAngles();	  // Show motor encoder values
	void ShowElement_BaudRate();			  // Show encoder baud rat
};