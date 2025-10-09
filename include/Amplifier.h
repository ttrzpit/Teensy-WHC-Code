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
#define HWSerialA Serial5	 // AdEx
#define HWSerialB Serial4	 // AbEx
#define HWSerialC Serial3	 // Flex

// Hardware amplifier pins
#define PIN_AMPLIFIER_ENABLE_A 35	 // Enables amplfier A (and corresponding LED)
#define PIN_AMPLIFIER_ENABLE_B 34	 // Enables amplfier B (and corresponding LED)
#define PIN_AMPLIFIER_ENABLE_C 33	 // Enables amplfier C (and corresponding LED)
#define PIN_AMPLIFIER_PWM_A 7		 // Sends PWM commands to amplifier A
#define PIN_AMPLIFIER_PWM_B 8		 // Sends PWM commands to amplifier B
#define PIN_AMPLIFIER_PWM_C 25		 // Sends PWM commands to amplifier C
#define PIN_AMPLIFIER_LED_A 30		 // Serial interface LED
#define PIN_AMPLIFIER_LED_B 31		 // Serial interface LED
#define PIN_AMPLIFIER_LED_C 32		 // Serial interface LED
#define PIN_AMPLIFIER_SAFETY 9		 // Safety switch input (can be overriden on control board as well)


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



// === HWSERIAL ==================================================================================


/**
 * @brief Struct containing ASCII serial commands
 */
struct AsciiStruct {

	String setReset			 = "r\r";				  // Reset
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
 * @brief Struct for packets
 */
struct PacketsStuct {
	String outgoingQueryA	= "";	 // ASCII Query being sent out
	String outgoingQueryB	= "";	 // ASCII Query being sent out
	String outgoingQueryC	= "";	 // ASCII Query being sent out
	String respondingQueryA = "";	 // ASCII Query being received
	String respondingQueryB = "";	 // ASCII Query being received
	String respondingQueryC = "";	 // ASCII Query being received
};


/**
 * @brief Struct for HWSerial info
 */
struct PropertiesStruct {
	uint32_t baudRateA = 0;		// Amp A baud rate
	uint32_t baudRateB = 0;		// Amp B baud rate
	uint32_t baudRateC = 0;		// Amp C baud rate
	String	 ampNameA  = "";	// Amplifier name
	String	 ampNameB  = "";	// Amplifier name
	String	 ampNameC  = "";	// Amplifier name
};


/**
 * @brief Struct of amplifier hardware serial properties
 */
struct HWSerialStruct {

	// Connection properties
	bool isConnectedA = false;
	bool isConnectedB = false;
	bool isConnectedC = false;

	// Sub-structs
	AsciiStruct		 Ascii;			// Contains ascii commands
	PacketsStuct	 Packets;		// Packets being transmitted
	PropertiesStruct Properties;	// Baud rate, names, etc
};


// === COMMAND ====================================================================================


/**
 * @brief Struct for state flags
 */
struct FlagsStruct {

	bool isSafetySwitchEnabled = false;	   // Safety switch flag
	bool isOutputEnabled	   = false;	   // Amplifier output enabled flag
	bool isCurrentCOmmandedA   = false;	   // Flag to tell if amp in current control mode
	bool isCurrentCOmmandedB   = false;	   // Flag to tell if amp in current control mode
	bool isCurrentCOmmandedC   = false;	   // Flag to tell if amp in current control mode
};


/**
 * @brief Struct of PWM elements and values
 */
struct PWMStruct {

	// PWM elements
	int16_t CONST_PWM_ZERO = 2047;				// Zero output value
	int16_t CONST_PWM_MAX  = 1;					// Maximum PWM value
	int16_t totalOutgoingA = CONST_PWM_ZERO;	// Total commanded PWM value for amp A
	int16_t totalOutgoingB = CONST_PWM_ZERO;	// Total commanded PWM value for amp B
	int16_t totalOutgoingC = CONST_PWM_ZERO;	// Total commanded PWM value for amp C
	int16_t tensionA	   = 0;					// Tension to add to commanded PWM
	int16_t tensionB	   = 0;					// Tension to add to commanded PWM
	int16_t tensionC	   = 0;					// Tension to add to commanded PWM
	int16_t outgoingA	   = CONST_PWM_ZERO;	// Commanded PWM
	int16_t outgoingB	   = CONST_PWM_ZERO;	// Commanded PWM
	int16_t outgoingC	   = CONST_PWM_ZERO;	// Commanded PWM
	int16_t outgoingOldA   = CONST_PWM_ZERO;	// Previous commanded PWM value for amp A
	int16_t outgoingOldB   = CONST_PWM_ZERO;	// Previous commanded PWM value for amp B
	int16_t outgoingOldC   = CONST_PWM_ZERO;	// Previous commanded PWM value for amp C
};


/**
 * @brief Struct of command elements 
 */
struct CommandStruct {

	// Sub-structs
	FlagsStruct Flags;	  // Container for flags
	PWMStruct	Pwm;	  // Container of pwm elements
};



// === SENSORS ====================================================================================

/**
 * @brief Struct of telemetry sensors elements
 */
struct SensorsStruct {

	// Telemetry values
	int16_t measuredCurrentA	  = 0;	  // Measured current
	int16_t measuredCurrentB	  = 0;	  // Measured current
	int16_t measuredCurrentC	  = 0;	  // Measured current
	int32_t measuredEncoderCountA = 0;	  // Encoder count
	int32_t measuredEncoderCountB = 0;	  // Encoder count
	int32_t measuredEncoderCountC = 0;	  // Encoder count
	float angleDegA			  = 0.0f;	  // Angle in degrees
	float angleDegB			  = 0.0f;	  // Angle in degrees
	float angleDegC			  = 0.0f;	  // Angle in degrees
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
	AmplifierClass( const AmplifierClass& )				 = delete;
	AmplifierClass& operator=( const AmplifierClass& )	 = delete;
	AmplifierClass( AmplifierClass&& )					 = delete;
	AmplifierClass&		   operator=( AmplifierClass&& ) = delete;
	static AmplifierClass* instance;	// Singleton-style hook for global ISR shim


	/*************
	*  Controls  *
	**************/
	public:
	CommandStruct Command;
	void		  Begin();							 // Initialize class
	void		  Update();							 // Update (called every loop)
	void		  SetTension( uint8_t tenA, uint8_t tenB, uint8_t tenC );	 // Set tension value
	bool		  GetSafetySwitchState();			 // Returns the state of the safety switch state
	private:
	void CommandPWM();	 // Send PWM signals to amplifiers
	void CommandZero();	 // Send zero signal to amplifiers
	void Reset();		 // Reset amplifier
	void Enable();		 // Enable amplifier
	void Disable();		 // Disables amplifier (for emergencies, requires system restart)
	void DisableA();	 // Disable amplfier and clear memory
	void EnableA();		 // Enable amplifier


	/************
	*  HWSerial *
	*************/
	private:
	HWSerialStruct HWSerial;
	void		   InitializeAmplifierA();	  // Initialize HWSerialA
	void		   InitializeAmplifierB();	  // Initialize HWSerialB
	void		   InitializeAmplifierC();	  // Initialize HWSerialC

	void SendQueryA( String newQuery );	   // Send a query to HWSerialA
	void SendQueryB( String newQuery );	   // Send a query to HWSerialB
	void SendQueryC( String newQuery );	   // Send a query to HWSerialC
	void ParseQueryA();					   // Parse queryA
	void ParseQueryB();					   // Parse queryB
	void ParseQueryC();					   // Parse queryC

	public:
	// void SendEncoderZeroCommand();	  // Send command to reset encoders
	void OnHWSerialAEvent();	// Instance handler for HWSerialA
	void OnHWSerialBEvent();	// Instance handler for HWSerialB
	void OnHWSerialCEvent();	// Instance handler for HWSerialC


	/*******************
	*  Initialization  *
	*******************/
	private:
	void ConfigurePins();	 // Configure and initialize hardware pins


	/***********
	*  SENSORS *
	************/
	public:
	SensorsStruct Sensors;


	/*************
	*  Accessors *
	**************/
	private:
	// Nothing

	public:
	String	GetAmplifierNameA();	// Returns name of amplifier
	String	GetAmplifierNameB();	// Returns name of amplifier
	String	GetAmplifierNameC();	// Returns name of amplifier
	int32_t GetBaudA();				// Returns baud rate
	int32_t GetBaudB();				// Returns baud rate
	int32_t GetBaudC();				// Returns baud rate



	int32_t GetEncoderCountA();		 // Measured encoder count
	int32_t GetEncoderCountB();		 // Measured encoder count
	int32_t GetEncoderCountC();		 // Measured encoder count
	int16_t GetCurrentReadingA();	 // Measured current reading
	int16_t GetCurrentReadingB();	 // Measured current reading
	int16_t GetCurrentReadingC();	 // Measured current reading
	float GetAngleDegA();			 // Measured motor angle
	float GetAngleDegB();			 // Measured motor angle
	float GetAngleDegC();			 // Measured motor angle
};



/*** OLD CODE */
/************************************************* */
/************************************************* */
/************************************************* */
/************************************************* */
// /************************************************* */


// // class AmplifierClass {

// // 	/***************************
// // 	*  Amplifier Constructors  *
// // 	****************************/
// // 	public:
// // 	// Default constructor and safeguards to prevent multiple amplifier instances
// // 	AmplifierClass();
// // 	AmplifierClass( const AmplifierClass& )			   = delete;
// // 	AmplifierClass& operator=( const AmplifierClass& ) = delete;
// // 	AmplifierClass( AmplifierClass&& )				   = delete;
// // 	AmplifierClass& operator=( AmplifierClass&& )	   = delete;


// // 	/***********
// // 	*  Timing  *
// // 	************/
// // 	private:
// // 	IntervalTimer IT_HWSerial;				 // Interval timer for HWSerial checks
// // 	void		  IT_HWSerial_Callback();	 // Callback function for HWSerial checks
// // 	uint16_t	  freqHWSerialHz  = 10;		 // Frequency for HWSerial
// // 	uint16_t	  freqPWMDriverHz = 1000;	 // Frequency to drive amplifier at
// // 	elapsedMillis timerHWSerialMillis;		 // Running timer in milliseconds
// // 	elapsedMillis timerPWMDriverMillis;		 // Running timer in milliseconds


// /**************
// 	*  Accessors  *
// 	***************/
// public:
// // void UpdatePWM();	 // Update (called every loop)
// // void UpdateEncoders();
// // // void UpdateEncoders( float& phiA, float& phiB, float& phiC );
// // void Begin( uint16_t newFreqPWMDriverHz, uint16_t newFreqHWSerialHz );	  // Initialize class


// /*************************************
// 	*  Amplifier Configuration Elements  *
// 	**************************************/
// private:
// // void CONFIGURE_ConfigurePins();	   // Configure and initialize hardware pins
// // int8_t PIN_AMPLIFIER_ENABLE_A = 35;	   // Enables amplfier A (and corresponding LED)
// // int8_t PIN_AMPLIFIER_ENABLE_B = 34;	   // Enables amplfier B (and corresponding LED)
// // int8_t PIN_AMPLIFIER_ENABLE_C = 33;	   // Enables amplfier C (and corresponding LED)
// // int8_t PIN_AMPLIFIER_PWM_A	  = 7;	   // Sends PWM commands to amplifier A
// // int8_t PIN_AMPLIFIER_PWM_B	  = 8;	   // Sends PWM commands to amplifier B
// // int8_t PIN_AMPLIFIER_PWM_C	  = 25;	   // Sends PWM commands to amplifier C
// // int8_t PIN_AMPLIFIER_LED_A	  = 30;	   // Serial interface LED
// // int8_t PIN_AMPLIFIER_LED_B	  = 31;	   // Serial interface LED
// // int8_t PIN_AMPLIFIER_LED_C	  = 32;	   // Serial interface LED
// // int8_t PIN_AMPLIFIER_SAFETY	  = 9;	   // Safety switch input (can be overriden on control board as well)


// /*********************
// 	*  Command Elements  *
// 	**********************/
// // public:
// // AmpCommandStruct Command;	 // Struct of command elements
// // private:
// // void COMMAND_SetZeroOutput();		// Send zero output to PWM
// // void COMMAND_SendCommandedPWM();	// Send commanded PWM value to amplifiers
// // void COMMAND_Reset();				// Reset amplifiers
// // public:
// // void COMMAND_Disable();																	   // Disable amplifiers
// // void COMMAND_Enable();																	   // Enable amplifiers
// // void COMMAND_DrivePolarRT( int8_t percentageR, float theta );							   // Drive amplifier using polar coordinates
// // void COMMAND_DriveCoordXY( int8_t percentageX, int8_t percentageY );					   // Drive amplifiers through XY commands
// // void COMMAND_DriveABC( uint8_t percentageA, uint8_t percentageB, uint8_t percentageC );	   // Drive amplifiers through ABC commands
// // void COMMAND_SetTension( uint8_t tensionPercentage );									   // Set the tension to the given percentage


// /*******************
// 	*  State Elements  *
// 	********************/
// // AmplifierFlagsStruct Flags;	   // Struct of amplifier state elements
// // public:
// // bool   FLAGS_GetAmplifierState();		// Get the amplifier state
// // bool   FLAGS_GetSafetySwitchState();	// Get the state of the safety switch
// // String FLAGS_GetStateStringA();			// Returns a string of the current device status
// // String FLAGS_GetStateStringB();			// Returns a string of the current device status
// // String FLAGS_GetStateStringC();			// Returns a string of the current device status


// /*****************************
//  	*  Hardware Serial Elements  *
//  	******************************/
// HWSerialStruct HWSerial;	// Struct containing HWSerial items
// public:
// void HWSERIAL_SetEncodersZero();	// Set motor encoders to zero
// private:
// // HWSerial initialize, get, set
// void HWSERIAL_InitializeHardwareSerial();		// Initialize hardware serial interface
// void HWSERIAL_IncreaseHardwareSerialSpeed();	// Updates the HWSerial speed to 115237 bps
// void HWSERIAL_GetAmplifierBaudRates();			// Get the current amplifier baud rate
// void HWSERIAL_GetAmplifierNames();				// Get amplifier names
// void HWSERIAL_GetEncoderValues();				// Read encoder counts via serial
// void HWSERIAL_GetAmplifierCurrents();			// Read current values via serial
// void HWSERIAL_SetPwmCurrentMode();				// Reset amplifiers into pwm current-control mode
// // HWSerial helper functions
// void HWSERIAL_EnqueueQueryA( const String& cmd );	 // Add query to serialA queue
// void HWSERIAL_EnqueueQueryB( const String& cmd );	 // Add query to serialB queue
// void HWSERIAL_EnqueueQueryC( const String& cmd );	 // Add query to serialC queue
// void HWSERIAL_PushQueryA();							 // Process next query in queue A
// void HWSERIAL_PushQueryB();							 // Process next query in queue B
// void HWSERIAL_PushQueryC();							 // Process next query in queue C
// void HWSERIAL_ReadQueryResponseA();					 // Read response to query A
// void HWSERIAL_ReadQueryResponseB();					 // Read response to query B
// void HWSERIAL_ReadQueryResponseC();					 // Read response to query C
// void HWSERIAL_ParseResponseA();						 // Parse response to query A
// void HWSERIAL_ParseResponseB();						 // Parse response to query B
// void HWSERIAL_ParseResponseC();						 // Parse response to query C
// void HWSERIAL_PushAllAndWaitForResponses();			 // Waits for responses before proceeding
// bool HWSERIAL_WaitForPortA( uint32_t ms );			 // Wait for port A
// bool HWSERIAL_WaitForPortB( uint32_t ms );			 // Wait for port A
// bool HWSERIAL_WaitForPortC( uint32_t ms );			 // Wait for port A


// void HWSERIAL_InitializeHardwareSerial();	 // Container for hardware serial initialization
// void HWSERIAL_InitializePortA();			 // Initialize HW Serial port A
// void HWSERIAL_InitializePortB();			 // Initialize HW Serial port B
// void HWSERIAL_InitializePortC();			 // Initialize HW Serial port C


// public:
// // None


// /*************************
//  	*  Read Values Elements  *
//  	**************************/
// ReadValuesStruct Read;	  // Struct containing encoder elements
// private:
// // Nothing
// public:
// int32_t READ_GetCountA();		// Encoder count
// int32_t READ_GetCountB();		// Encoder count
// int32_t READ_GetCountC();		// Encoder count
// int16_t READ_GetCurrentA();		// Current measurement
// int16_t READ_GetCurrentB();		// Current measurement
// int16_t READ_GetCurrentC();		// Current measurement
// float	READ_GetAngleDegA();	// Encoder angle in degrees
// float	READ_GetAngleDegB();	// Encoder angle in degrees
// float	READ_GetAngleDegC();	// Encoder angle in degrees

// /*************************
//  	*  Read Values Elements  *
//  	**************************/
// }
// ;
