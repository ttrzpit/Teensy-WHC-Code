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
#include <queue>		// For queuing HWSerial commands

// Hardware serial ports
#define HWSerialA Serial5	 // AdEx
#define HWSerialB Serial4	 // AbEx
#define HWSerialC Serial3	 // Flex



/* 
 * 
 *  ===================================================================
 *  ===================================================================
 * 
 *   SSSS   TTTTTT  RRRRR    U     U   CCCCC    TTTTTT   SSSS
 *  SS        TT    RR  RR   U     U   CC         TT    SS
 *  SS        TT    RR  RR   U     U   CC         TT    SS
 *   SSSS     TT    RRRRR    U     U   CC         TT     SSSS
 *      SS    TT    RR  RR   U     U   CC         TT        SS
 *      SS    TT    RR  RR   U     U   CC         TT        SS
 *   SSSS     TT    RR   RR   UUUUU    CCCCC      TT     SSSS
 * 
 *  =================================================================== 
 *  =================================================================== 
 */

/**
 * @brief Struct of amplifier properties
 */
struct AmpPropertiesStruct {

	uint32_t baudRateA	 = 0;	  // Amplfier baud rate
	uint32_t baudRateB	 = 0;	  // Amplfier baud rate
	uint32_t baudRateC	 = 0;	  // Amplfier baud rate
	String	 nameStringA = "";	  // Amplifier name
	String	 nameStringB = "";	  // Amplifier name
	String	 nameStringC = "";	  // Amplifier name
};

/**
 * @brief Struct of amplifier flags
 */
struct AmplifierFlagsStruct {

	bool isSafetySwitchEnabled = false;	   // Safety switch flag
	bool isEnabledA			   = false;	   // Amplifier enabled flag
	bool isEnabledB			   = false;	   // Amplifier enabled flag
	bool isEnabledC			   = false;	   // Amplifier enabled flag
	bool isCurrentCommandedA   = false;	   // Flag to tell if amp has been placed into current control mode
	bool isCurrentCommandedB   = false;	   // Flag to tell if amp has been placed into current control mode
	bool isCurrentCommandedC   = false;	   // Flag to tell if amp has been placed into current control mode
};

/**
 * @brief Struct containing ASCII serial commands
 */
struct AsciiStruct {
	String getCurrentReading = "g r0x0c\r";			  // HWSerial command to get current reading
	String getBaud			 = "g r0x90\r";			  // HWSerial command to get current baud
	String getEncoderCount	 = "g r0x17\r";			  // HWSerial command to get encoder count
	String getName			 = "g f0x92\r";			  // HWSerial command to get amp name
	String setIdle			 = "";					  // HWSerial command for idle
	String setBaud115237	 = "s r0x90 115237\r";	  // HWSerial command to set baud rate to 115237
	String setCurrentMode	 = "s r0x24 3\r";		  // HWSerial command to set amplifier in PWM current mode
	String setEncoderZero	 = "s r0x17 0\r";		  // HWSerial command to reset motor encoder
};

/**
 * @brief Struct of amp command items
 */
struct AmpCommandStruct {
	// Variables
	int16_t pwmNewA = 2047;	   // Commanded PWM value for amp A
	int16_t pwmNewB = 2047;	   // Commanded PWM value for amp B
	int16_t pwmNewC = 2047;	   // Commanded PWM value for amp C
	int16_t pwmOldA = 2047;	   // Previous commanded PWM value for amp A
	int16_t pwmOldB = 2047;	   // Previous commanded PWM value for amp B
	int16_t pwmOldC = 2047;	   // Previous commanded PWM value for amp C
	int16_t pwmZero = 2047;	   // Zero output value
};

/**
 * @brief Struct for encoder items
 */
struct ReadValuesStruct {

	int16_t currentA  = 0;	  // Measured current
	int16_t currentB  = 0;	  // Measured current
	int16_t currentC  = 0;	  // Measured current
	int32_t countA	  = 0;	  // Encoder count
	int32_t countB	  = 0;	  // Encoder count
	int32_t countC	  = 0;	  // Encoder count
	int32_t angleDegA = 0;	  // Angle in degrees
	int32_t angleDegB = 0;	  // Angle in degrees
	int32_t angleDegC = 0;	  // Angle in degrees
};

/**
 * @brief Struct containing elements for the query queue
 */
struct QueryQueueStruct {

	std::queue<String> queueA;				  // Queue of queries to be sent by HWSerial to amp A
	std::queue<String> queueB;				  // Queue of queries to be sent by HWSerial to amp B
	std::queue<String> queueC;				  // Queue of queries to be sent by HWSerial to amp C
	String			   outgoingNextA = "";	  // Next outgoing query
	String			   outgoingNextB = "";	  // Next outgoing query
	String			   outgoingNextC = "";	  // Next outgoing query
	String			   responseA	 = "";	  // Response to HWSerial query to amp A
	String			   responseB	 = "";	  // Response to HWSerial query to amp A
	String			   responseC	 = "";	  // Response to HWSerial query to amp A
};


/**
 * @brief Contains elements related to the HWSerial interface
 * 
 */
struct HWSerialStruct {

	QueryQueueStruct	Query;							// Struct of query queue items
	AsciiStruct			Ascii;							// Struct of ascii items
	AmpPropertiesStruct AmpProperty;					// Struct of amp properties
	bool				isConnected			= false;	// State of HW serial connection
	bool				isAwaitingResponseA = false;	// State indicator if HWSerial is awaiting a response
	bool				isAwaitingResponseB = false;	// State indicator if HWSerial is awaiting a response
	bool				isAwaitingResponseC = false;	// State indicator if HWSerial is awaiting a response
};



/*  ===================================================================
 *  ===================================================================
 * 
 *    CCCCC      LL          AAAAA       SSSS        SSSS
 *   CC          LL         AA   AA     SS           SS
 *   CC          LL         AA   AA     SS           SS
 *   CC          LL         AAAAAAA      SSSS        SSSS
 *   CC          LL         AA   AA         SS          SS
 *   CC          LL         AA   AA         SS          SS
 *    CCCCC      LLLLL      AA   AA      SSSS        SSSS
 * 
 *  =================================================================== 
 *  =================================================================== */

class AmplifierClass {

	/***************************
	*  Amplifier Constructors  *
	****************************/
	public:
	// Default constructor and safeguards to prevent multiple amplifier instances
	AmplifierClass();
	AmplifierClass( const AmplifierClass& )			   = delete;
	AmplifierClass& operator=( const AmplifierClass& ) = delete;
	AmplifierClass( AmplifierClass&& )				   = delete;
	AmplifierClass& operator=( AmplifierClass&& )	   = delete;

	/***********
	*  Timing  *
	************/
	private:
	uint16_t	  timerHWSerialFrequencyHz = 60;	// Frequency to display timer outputs
	elapsedMillis timerRuntimeMillis;				// Running timer in milliseconds


	/**************
	*  Accessors  *
	***************/
	public:
	void Update();	  // Update (called every loop)
	void Begin();	  // Initialize class

	/*************************************
	*  Amplifier Configuration Elements  *
	**************************************/
	private:
	void   CONFIGURE_ConfigurePins();	   // Configure and initialize hardware pins
	int8_t PIN_AMPLIFIER_ENABLE_A = 35;	   // Enables amplfier A (and corresponding LED)
	int8_t PIN_AMPLIFIER_ENABLE_B = 34;	   // Enables amplfier B (and corresponding LED)
	int8_t PIN_AMPLIFIER_ENABLE_C = 33;	   // Enables amplfier C (and corresponding LED)
	int8_t PIN_AMPLIFIER_PWM_A	  = 7;	   // Sends PWM commands to amplifier A
	int8_t PIN_AMPLIFIER_PWM_B	  = 8;	   // Sends PWM commands to amplifier B
	int8_t PIN_AMPLIFIER_PWM_C	  = 25;	   // Sends PWM commands to amplifier C
	int8_t PIN_AMPLIFIER_LED_A	  = 30;	   // Serial interface LED
	int8_t PIN_AMPLIFIER_LED_B	  = 31;	   // Serial interface LED
	int8_t PIN_AMPLIFIER_LED_C	  = 32;	   // Serial interface LED
	int8_t PIN_AMPLIFIER_SAFETY	  = 9;	   // Safety switch input (can be overriden on control board as well)

	/*********************
	*  Command Elements  *
	**********************/
	AmpCommandStruct Command;	 // Struct of command elements
	private:
	void COMMAND_SetZeroOutput();													 // Send zero output to PWM
	void COMMAND_SendCommandedPWM( uint16_t pwmA, uint16_t pwmB, uint16_t pwmC );	 // Send commanded PWM value to amplifiers
	void COMMAND_Reset();															 // Reset amplifiers
	public:
	void COMMAND_Disable();																	   // Disable amplifiers
	void COMMAND_Enable();																	   // Enable amplifiers
	void COMMAND_DrivePolarRT( int8_t percentageR, float theta );							   // Drive amplifier using polar coordinates
	void COMMAND_DriveCoordXY( int8_t percentageX, int8_t percentageY );					   // Drive amplifiers through XY commands
	void COMMAND_DriveABC( uint8_t percentageA, uint8_t percentageB, uint8_t percentageC );	   // Drive amplifiers through ABC commands
	void COMMAND_SetTension( uint8_t tensionPercentage );									   // Set the tension to the given percentage

	/*******************
	*  State Elements  *
	********************/
	AmplifierFlagsStruct Flags;	   // Struct of amplifier state elements
	public:
	bool   FLAGS_GetAmplifierState();		// Get the amplifier state
	bool   FLAGS_GetSafetySwitchState();	// Get the state of the safety switch
	String FLAGS_GetStateStringA();			// Returns a string of the current device status
	String FLAGS_GetStateStringB();			// Returns a string of the current device status
	String FLAGS_GetStateStringC();			// Returns a string of the current device status


	/*****************************
 	*  Hardware Serial Elements  *
 	******************************/
	HWSerialStruct HWSerial;	// Struct containing HWSerial items
	private:
	void HWSERIAL_EnqueueQueryA( const String& cmd );	 // Add query to serialA queue
	void HWSERIAL_EnqueueQueryB( const String& cmd );	 // Add query to serialB queue
	void HWSERIAL_EnqueueQueryC( const String& cmd );	 // Add query to serialC queue
	void HWSERIAL_ProcessQueryA();						 // Process next query in queue A
	void HWSERIAL_ProcessQueryB();						 // Process next query in queue B
	void HWSERIAL_ProcessQueryC();						 // Process next query in queue C
	void HWSERIAL_ReadQueryResponseA();					 // Read response to query A
	void HWSERIAL_ReadQueryResponseB();					 // Read response to query B
	void HWSERIAL_ReadQueryResponseC();					 // Read response to query C
	void HWSERIAL_ParseResponseA();						 // Parse response to query A
	void HWSERIAL_ParseResponseB();						 // Parse response to query B
	void HWSERIAL_ParseResponseC();						 // Parse response to query C
	void HWSERIAL_SetPwmCurrentMode();					 // Reset amplifiers into pwm current-control mode
	void HWSERIAL_InitializeHardwareSerial();			 // Initialize hardware serial interface
	void HWSERIAL_SetEncodersZero();					 // Set motor encoders to zero


	public:
	// None


	/*************************
 	*  Read Values Elements  *
 	**************************/
	ReadValuesStruct Read;	  // Struct containing encoder elements
	private:
	void READ_Encoders();	  // Read encoder counts via serial
	void READ_Currents();	  // Read current values via serial
	void READ_BaudRates();	  // Read baud rates
	public:
	int32_t READ_GetCountA();		// Encoder count
	int32_t READ_GetCountB();		// Encoder count
	int32_t READ_GetCountC();		// Encoder count
	int16_t READ_GetCurrentA();		// Current measurement
	int16_t READ_GetCurrentB();		// Current measurement
	int16_t READ_GetCurrentC();		// Current measurement
	float	READ_GetAngleDegA();	// Encoder angle in degrees
	float	READ_GetAngleDegB();	// Encoder angle in degrees
	float	READ_GetAngleDegC();	// Encoder angle in degrees

	/*************************
 	*  Read Values Elements  *
 	**************************/
};
