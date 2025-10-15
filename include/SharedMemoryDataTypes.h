#pragma once
#include <Arduino.h>
#include <cstdint>
#include <memory>
#include <unordered_map>



#pragma once
#include <Arduino.h>
#include <cstdint>
#include <memory>
#include <unordered_map>



// // === FORWARD DECLARATIONS =======================================================================
// class DriveTensionClass;
// class DriveFlagsClass;
// class PWMClass;
// class DriveMappingClass;
// class DriveClass;
// class GamepadInputClass;
// class PacketClass;
// class ConnectionClass;
// class HardwareSerialClass;
// class KeyboardInputClass;
// class MappingClass;
// class SoftwareSerialClass;
// class InterfaceClass;
// class PlatformEncodersClass;
// class EncoderLimitsClass;
// class CurrentLimitsClass;
// class MotorEncodersClass;
// class MotorCurrentsClass;
// class SensorsClass;
// class DiscriminationTaskResults;
// class CardinalDirectionsClass;
// class AllDirectionsClass;
// class DiscriminationTaskClass;
// class TasksClass;
// class ActionsQueueClass;
// class EnumsClass;
// class SystemStateClass;
// class ManagedSystemDataClass;


/*  ============================================================================================
 *  ============================================================================================
 *
 *  DDDDD   RRRRR   IIIIII  VV    VV  EEEEEE
 *  DD  DD  RR  RR    II    VV    VV  EE
 *  DD  DD  RR  RR    II    VV    VV  EE
 *  DD  DD  RRRRR     II     VV  VV   EEEE
 *  DD  DD  RR  RR    II      VVVV    EE
 *  DD  DD  RR  RR    II      VVVV    EE
 *  DDDDD   RR  RR  IIIIII     VV     EEEEEE
 *
 *  - DriveClass
 *    - DriveFlagsClass
 * 
 *  ============================================================================================
 *  ============================================================================================*/


class DriveTensionClass {

	public:
	uint8_t	 valueInteger = 0;		  // Tension value (percentage)
	uint16_t valuePwm	  = 0;		  // Tension PWM value
	bool	 isEnabled	  = false;	  // Tension flag
};

class DriveFlagsClass {
	public:
	bool isSafetySwitchEngaged = false;
	bool isMotorOutputEnabled  = false;
	bool isCurrentControlled   = false;
};

class PWMClass {
	public:
	uint16_t CONST_PWM_ZERO = 2047;	   // Zero drive
	uint16_t CONST_PWM_MAX	= 4;	   // Max drive

	public:
	int16_t rawOutgoingA	   = CONST_PWM_ZERO;	// Raw PWM without tension
	int16_t rawOutgoingB	   = CONST_PWM_ZERO;	// Raw PWM without tension
	int16_t rawOutgoingC	   = CONST_PWM_ZERO;	// Raw PWM without tension
	int16_t totalOutgoingA	   = CONST_PWM_ZERO;	// Total PWM
	int16_t totalOutgoingB	   = CONST_PWM_ZERO;	// Total PWM
	int16_t totalOutgoingC	   = CONST_PWM_ZERO;	// Total PWM
	int16_t totalOutgoingPrevA = CONST_PWM_ZERO;	// Total previous PWM
	int16_t totalOutgoingPrevB = CONST_PWM_ZERO;	// Total previous PWM
	int16_t totalOutgoingPrevC = CONST_PWM_ZERO;	// Total previous PWM
};

class DriveMappingClass {

	public:
	float		targetRadius   = 0.0f;				   // Radius for polar conversion
	float		targetAngleDeg = 0.0f;				   // Angle of actuation
	float		contributionA  = 0.0f;				   // Contribution for motor A
	float		contributionB  = 0.0f;				   // Contribution for motor B
	float		contributionC  = 0.0f;				   // Contribution for motor C
	const float thetaA		   = radians( 35.0f );	   // Motor A angle (35)
	const float thetaB		   = radians( 145.0f );	   // Motor B angle (145)
	const float thetaC		   = radians( 270.0f );	   // Motor C angle (270)
	const float angleAB		   = radians( 110.0f );	   // Sweep angle between angles A and B (110)
	const float angleBC		   = radians( 125.0f );	   // Sweep angle between angles B and C (125)
	const float angleAC		   = radians( 125.0f );	   // Sweep angle between angles A and C (125)
};


class DriveClass {
	public:
	DriveMappingClass MappingClass;
	DriveFlagsClass	  Flags;
	DriveTensionClass Tension;
	PWMClass		  Pwm;
};



/*  ============================================================================================
 *  ============================================================================================
 *
 *   EEEEEE  NN   NN  UU   UU  MM    MM  SSSSS
 *   EE      NNN  NN  UU   UU  MMM  MMM  SS
 *   EE      NN N NN  UU   UU  MM MM MM  SS
 *   EEEE    NN  NNN  UU   UU  MM    MM   SSSS
 *   EE      NN   NN  UU   UU  MM    MM      SS
 *   EE      NN   NN  UU   UU  MM    MM      SS
 *   EEEEEE  NN   NN   UUUUU   MM    MM  SSSSS
 *
 *  ============================================================================================
 *  ============================================================================================*/


class EnumsClass {

	public:
	enum class SystemStateEnum : int8_t { IDLE, DISABLED, IDLING, RUNNING_TASK };
	enum class TaskSelectionEnum : int8_t { NONE, MEASURING_RANGE_OF_MOTION, TESTING_CARDINAL_DIRECTIONS, TESTING_OCTANT_DIRECTIONS, TESTING_TARGET_ANGLE };
	enum class DiscriminationTaskStateEnum : uint8_t { IDLE, STARTING, WAITING_FOR_DELAY, RENDERING_PROMPT, WAITING_FOR_RESPONSE, FINISHING };

	public:
	String MapSystemStateEnumToString( int8_t state );
	String MapDiscriminationTaskStateEnumToString( int8_t state );
	String MapTaskSelectionEnumToString( int8_t state );
	String MapDiscriminationDirectionsToString( int8_t direction );


	private:
	std::unordered_map<int8_t, String> mapOfSystemStateEnumToString				= { { -1, "NONE" }, { 0, "DISABLED" }, { 1, "IDLING" }, { 2, "RUNNING_TASK" } };
	std::unordered_map<int8_t, String> mapOfTaskSelectionEnumToString			= { { -1, "NONE" }, { 0, "NONE" }, { 1, "MEASURING_RANGE_OF_MOTION" }, { 2, "TESTING_CARDINAL_DIRECTIONS" }, { 3, "TESTING_OCTANT_DIRECTIONS" }, { 4, "TESTING_TARGET_ANGLE" } };
	std::unordered_map<int8_t, String> mapOfDiscriminationTaskStateEnumToString = { { -1, "NONE" }, { 0, "IDLE" }, { 1, "STARTING" }, { 2, "WAITING_FOR_DELAY" }, { 3, "RENDERING_PROMPT" }, { 4, "WAITING_FOR_RESPONSE" }, { 5, "FINISHING" } };
	std::unordered_map<int8_t, String> mapOfDiscriminationDirectionsToString	= { { -1, "NONE      " }, { 0, "UP        " }, { 1, "UP+RIGHT  " }, { 2, "RIGHT     " }, { 3, "DOWN+RIGHT" }, { 4, "DOWN      " }, { 5, "DOWN+LEFT " }, { 6, "LEFT      " }, { 7, "UP+LEFT   " } };
	std::unordered_map<int8_t, String> mapOfGamepadButtonsToString
		= { { -1, "NONE      " }, { 0, "UP        " }, { 1, "UP+RIGHT  " }, { 2, "RIGHT     " }, { 3, "DOWN+RIGHT" }, { 4, "DOWN      " }, { 5, "DOWN+LEFT " }, { 6, "LEFT      " }, { 7, "UP+LEFT   " }, { 8, "RED       " }, { 9, "GREEN     " } };
};



/*  ============================================================================================
 *  ============================================================================================
 *
 *  IIIIII  NN   NN  TTTTTTT  EEEEEE  RRRRR    FFFFFF    AA     CCCCC  EEEEEE
 *    II    NNN  NN     TT    EE      RR  RR   FF       AAAA   CC      EE
 *    II    NN N NN     TT    EE      RR  RR   FF      AA  AA  CC      EE
 *    II    NN  NNN     TT    EEEE    RRRRR    FFFF    AAAAAA  CC      EEEE
 *    II    NN   NN     TT    EE      RR  RR   FF      AA  AA  CC      EE
 *    II    NN   NN     TT    EE      RR  RR   FF      AA  AA  CC      EE
 *  IIIIII  NN   NN     TT    EEEEEE  RR  RR   FF      AA  AA   CCCCC  EEEEEE
 *
 *  - InterfaceClass
 * 	  - Gamepad
 *    - HWSerial
 *    - Keyboard
 *    - Mapping
 *    - SWSerial
 *  ============================================================================================
 *  ============================================================================================*/


class GamepadInputClass {

	private:
	int8_t pressedPrev = -1;
	int8_t pressedNew  = -1;


	public:
	bool   isButtonPressed = false;
	bool   isInputWaiting  = false;	   // Flag that indicates an input needing a response
	int8_t buttonPressed   = -1;
	String buttonName	   = "None";
};


class PacketClass {
	public:
	String outgoingQueryA	= "";	 // ASCII Query being sent out
	String outgoingQueryB	= "";	 // ASCII Query being sent out
	String outgoingQueryC	= "";	 // ASCII Query being sent out
	String respondingQueryA = "";	 // ASCII Query being received
	String respondingQueryB = "";	 // ASCII Query being received
	String respondingQueryC = "";	 // ASCII Query being received
};


class ConnectionClass {
	public:
	uint32_t baudRateA = 0;		// Amp A baud rate
	uint32_t baudRateB = 0;		// Amp B baud rate
	uint32_t baudRateC = 0;		// Amp C baud rate
	String	 ampNameA  = "";	// Amplifier name
	String	 ampNameB  = "";	// Amplifier name
	String	 ampNameC  = "";	// Amplifier name
};

class HardwareSerialClass {
	public:
	ConnectionClass Connection;
	PacketClass		Packets;

	bool	 isConnected = false;
	uint16_t baudRate	 = 0;
};


class KeyboardInputClass {

	private:
	int8_t pressedPrev = -1;
	int8_t pressedNew  = -1;

	public:
	bool isKeyPressed = false;
};

class SWSerialFlagsClass {

	public:
	bool showPlatformEncoders = false;	  // Show platform encoders
	bool showMotorCurrents	  = false;	  // Show motor currents
	bool showMotorAngles	  = false;	  // Show motor angles
	bool showMotorPwmOutputs  = false;	  // Show motor PWM outputs
	bool showTaskOutput		  = false;	  // Show task output
};

class SoftwareSerialClass {

	public:
	SWSerialFlagsClass Toggle;

	public:
	bool isConnected			= false;
	bool isScrollingLineEnabled = false;
};


class InterfaceClass {
	public:
	GamepadInputClass	Gamepad;
	HardwareSerialClass HWSerial;
	KeyboardInputClass	Keyboard;
	SoftwareSerialClass SWSerial;
};



/*  ============================================================================================
 *  ============================================================================================
 *
 *   SSSSS  EEEEEE  NN   NN   SSSSS   OOOOO   RRRRR    SSSSS
 *  SS      EE      NNN  NN  SS      OO   OO  RR  RR  SS
 *  SS      EE      NN N NN  SS      OO   OO  RR  RR  SS
 *   SSSS   EEEE    NN  NNN   SSSS   OO   OO  RRRRR    SSSS
 *      SS  EE      NN   NN      SS  OO   OO  RR  RR      SS
 *      SS  EE      NN   NN      SS  OO   OO  RR  RR      SS
 *  SSSSS   EEEEEE  NN   NN  SSSSS    OOOOO   RR  RR  SSSSS
 *
 *  - SensorsClass
 *    - PlatformEncodersClass
 *    - MotorEncodersClass
 *    - MotorCurrentsClass
 *    
 *  ============================================================================================
 *  ============================================================================================*/


class PlatformEncodersClass {
	public:
	float horizontalAngleDegrees = 0.0f;
	float verticalAngleDegrees	 = 0.0f;
};

class EncoderLimitsClass {
	public:
	bool	isBeingMeasured = false;	// Is the limit being measured
	bool	isSet			= false;	// Is the limit set
	bool	isEnabled		= false;	// Is the limit enabled
	bool	isBeingTested	= false;	// Is the limit being tested
	int32_t limitCountA		= 0;		// Max value (count)
	int32_t limitCountB		= 0;		// Max value (count)
	int32_t limitCountC		= 0;		// Max value (count)
	float	limitPhiDegA	= 0.0f;		// Max value (float)
	float	limitPhiDegB	= 0.0f;		// Max value (float)
	float	limitPhiDegC	= 0.0f;		// Max value (float)
};


class CurrentLimitsClass {
	public:
	bool  isBeingMeasured = false;	  // Is the limit being measured
	bool  isSet			  = false;	  // Is the limit set
	bool  isEnabled		  = false;	  // Is the limit enabled
	float limitA		  = 0.0f;	  // Max value (float)
	float limitB		  = 0.0f;	  // Max value (float)
	float limitC		  = 0.0f;	  // Max value (float)
};


class MotorEncodersClass {

	// Motor angles
	public:
	EncoderLimitsClass Limits;
	float			   measuredAngleDegA = 0.0f;	// Measured angle with encoder zero offset
	float			   measuredAngleDegB = 0.0f;	// Measured angle with encoder zero offset
	float			   measuredAngleDegC = 0.0f;	// Measured angle with encoder zero offset
	int32_t			   rawCountA		 = 0;		// Raw  encoder count
	int32_t			   rawCountB		 = 0;		// Raw  encoder count
	int32_t			   rawCountC		 = 0;		// Raw  encoder count
	int32_t			   compensatedCountA = 0;		// Compensated encoder count
	int32_t			   compensatedCountB = 0;		// Compensated encoder count
	int32_t			   compensatedCountC = 0;		// Compensated encoder count
	int32_t			   offsetToZeroA	 = 0;		// Offset to get to zero degrees
	int32_t			   offsetToZeroB	 = 0;		// Offset to get to zero degrees
	int32_t			   offsetToZeroC	 = 0;		// Offset to get to zero degrees
};


class MotorCurrentsClass {

	// Current
	public:
	CurrentLimitsClass Limits;
	float			   measuredCurrentAmpsA		= 0.0f;	   // Measured current in amps
	float			   measuredCurrentAmpsB		= 0.0f;	   // Measured current in amps
	float			   measuredCurrentAmpsC		= 0.0f;	   // Measured current in amps
	float			   measuredCurrentAmpsPrevA = 0.0f;	   // Previously measured current in amps
	float			   measuredCurrentAmpsPrevB = 0.0f;	   // Previously measured current in amps
	float			   measuredCurrentAmpsPrevC = 0.0f;	   // Previously measured current in amps
};


class SensorsClass {
	public:
	PlatformEncodersClass PlatformEncoders;
	MotorEncodersClass	  MotorEncoders;
	MotorCurrentsClass	  MotorCurrents;
};


/*  ===============================================================================================
 *  ===============================================================================================
 *
 *   TTTTTTT    AAA     SSSSS  KK  KK   SSSSS
 *     TT      AA AA   SS      KK KK   SS
 *     TT     AA   AA  SS      KKKK    SS
 *     TT     AAAAAAA   SSSS   KKKK     SSSS
 *     TT     AA   AA      SS  KK KK       SS
 *     TT     AA   AA      SS  KK  KK      SS
 *     TT     AA   AA  sSSSS   KK  KK  SSSSS
 *
 *  - TasksClass
 *    - DiscriminationClass
 *      - CardinalDirectionsClass
 *      - AllDirectionsClass
 *  ===============================================================================================
 *  ===============================================================================================*/

class DiscriminationTaskResults {

	size_t	 trialNumber	   = 0;		   // Iterating trial number
	int8_t	 promptVal		   = -1;	   // Value of new directional prompt
	String	 promptString	   = "";	   // String of new directional  prompt
	uint32_t promptDelayTimeMs = 0;		   // Randomized delay time
	int8_t	 responseVal	   = -1;	   // Value of participant response to prompt
	String	 responseString	   = "";	   // String of participant response to prompt
	uint32_t responseTimeMs	   = 0.0f;	   // Participant task completion time [ms]
	bool	 isResponseCorrect = false;	   // Flag if response correct
};

class CardinalDirectionsClass {

	public:
	uint8_t nRepetitions = 0;	 // Number of repetitions of each direction (4 directions total)
	int32_t totalTime	 = 0;	 // Total time taken to complete the tasks (ms)

	public:
	DiscriminationTaskResults Results;

	public:
	EnumsClass::DiscriminationTaskStateEnum currentState = EnumsClass::DiscriminationTaskStateEnum::IDLE;

	//
};


class OctantDirectionsClass {
	public:
	uint8_t nRepetitions = 0;	 // Number of repetitions of each direction (8 directions total)
	int32_t totalTime	 = 0;	 // Total time taken to complete the tasks (ms)

	public:
	DiscriminationTaskResults Results;

	public:
	EnumsClass::DiscriminationTaskStateEnum currentState = EnumsClass::DiscriminationTaskStateEnum::IDLE;
};


class DiscriminationTaskClass {


	public:
	CardinalDirectionsClass CardinalDirections;
	OctantDirectionsClass	OctantDirections;
};

class TasksClass {

	public:
	EnumsClass::TaskSelectionEnum activeTask = EnumsClass::TaskSelectionEnum::NONE;
	DiscriminationTaskClass		  DiscriminationTask;
};


/*  ============================================================================================
 *  ============================================================================================
 *
 *    SSSS  Y     Y   SSSS  TTTTTTT  EEEEEE  MM    MM
 *   SS      Y   Y   SS        TT    EE      MMM  MMM
 *   SS       Y Y    SS        TT    EE      MM MM MM
 *    SSSS     Y      SSSS     TT    EEEE    MM    MM
 *       SS    Y         SS    TT    EE      MM    MM
 *       SS    Y         SS    TT    EE      MM    MM
 *    SSSS     Y      SSSS     TT    EEEEEE  MM    MM
 *
 *  - SystemClass
 *    - StatesClass
 *  ============================================================================================
 *  ============================================================================================*/

class ActionsQueueClass {

	public:
	bool zeroPlatformEncoders	= false;
	bool zeroMotorEncoders		= false;
	bool setMotorTension		= false;
	bool setMotorTensionEnabled = false;
};



class SystemStateClass {

	public:
	EnumsClass::SystemStateEnum systemState = EnumsClass::SystemStateEnum::IDLE;
};


class ManagedSystemDataClass {
	public:
	ActionsQueueClass ActionQueue;
	DriveClass		  Drive;
	EnumsClass		  Enumerators;
	InterfaceClass	  Interface;
	SensorsClass	  Sensors;
	SystemStateClass  State;
	TasksClass		  Tasks;
};



// ================================================================================================
// === DRIVE ======================================================================================
// ================================================================================================

// === FLAGS ======================================================================================



// ================================================================================================
// === INTERFACE ==================================================================================
// ================================================================================================

// === GAMEPAD INPUT ==============================================================================

// === HWSERIAL INTERFACE =========================================================================


// === SWSERIAL INTERFACE =========================================================================



// ================================================================================================
// === SENSORS ====================================================================================
// ================================================================================================

// === PLATFORM ENCODERS ==========================================================================

// === MOTOR CURRENT ==============================================================================

// === MOTOR ENCODERS =============================================================================



// ================================================================================================
// === TASKS ======================================================================================
// ================================================================================================

// === DISCRIMINATION TASK ========================================================================

// === CARDINAL DIRECTIONS ========================================================================

// === ALL  DIRECTIONS ============================================================================



// ================================================================================================
// === SYSTEM =====================================================================================
// ================================================================================================

// === ENUM =====================================================================================

/**
 * @brief Map system states into a string
 * 
 * @param state State to be mapped
 * @return String State name as string
 */
inline String EnumsClass::MapSystemStateEnumToString( int8_t state ) {
	return mapOfSystemStateEnumToString[state];
}


/**
 * @brief Map task selection enum into a string
 * 
 * @param state State to be mapped
 * @return String State name as string
 */
inline String EnumsClass::MapTaskSelectionEnumToString( int8_t state ) {

	return mapOfTaskSelectionEnumToString[state];
}

/**
 * @brief Map discrimination task enum into a string
 * 
 * @param state State to be mapped
 * @return String String State name as string
 */
inline String EnumsClass::MapDiscriminationTaskStateEnumToString( int8_t state ) {

	return mapOfDiscriminationTaskStateEnumToString[state];
}



/**
 * @brief Map gamepad direction into a string
 * 
 * @param direction Direction to be mapped
 * @return String Direction name as string
 */
inline String EnumsClass::MapDiscriminationDirectionsToString( int8_t direction ) {
	return mapOfDiscriminationDirectionsToString[direction];
}
