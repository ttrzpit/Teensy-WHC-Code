#include "AmplifierClass.h"

AmplifierClass::AmplifierClass() {

	// Do not modify the order of the items below
	CONFIGURE_ConfigurePins();
	CONFIGURE_ConfigureHWSerialInterface();
	COMMAND_ResetIntoPwmCurrentMode();
}



/*  ===================================================================================
 *  ===================================================================================
 * 
 *   CCCCC     OOOOO    NN   NN   FFFFFF  IIIIII    GGGGG    UU   UU   RRRRR     EEEEEE
 *  CC        OO   OO   NNN  NN   FF        II     GG   GG   UU   UU   RR  RR    EE
 *  CC        OO   OO   NN N NN   FF        II     GG        UU   UU   RR  RR    EE
 *  CC        OO   OO   NN  NNN   FFFF      II     GG  GGG   UU   UU   RRRRR     EEEE
 *  CC        OO   OO   NN   NN   FF        II     GG   GG   UU   UU   RR  RR    EE
 *  CC        OO   OO   NN   NN   FF        II     GG   GG   UU   UU   RR  RR    EE
 *   CCCCC     OOOOO    NN   NN   FF      IIIIII    GGGGG     UUUUU    RR   RR   EEEEEE
 * 
 *  =================================================================================== 
 *  ===================================================================================*/



/**
 * @brief Configures the hardware serial interface with the amplifiers
 * 
 */
void AmplifierClass::CONFIGURE_ConfigureHWSerialInterface() {

	// Establish hardware serial connection to amplifiers
	HWSerialA.begin( 9600 );
	delay( 100 );
	HWSerialB.begin( 9600 );
	delay( 100 );
	HWSerialC.begin( 9600 );
	delay( 100 );
}



/**
 * @brief Initializes the hardware pins
 * 
 */
void AmplifierClass::CONFIGURE_ConfigurePins() {

	// Initialize pins
	pinMode( PIN_AMPLIFIER_ENABLE_A, OUTPUT );
	pinMode( PIN_AMPLIFIER_ENABLE_B, OUTPUT );
	pinMode( PIN_AMPLIFIER_ENABLE_C, OUTPUT );
	pinMode( PIN_AMPLIFIER_PWM_A, OUTPUT );
	pinMode( PIN_AMPLIFIER_PWM_B, OUTPUT );
	pinMode( PIN_AMPLIFIER_PWM_C, OUTPUT );
	pinMode( PIN_AMPLIFIER_LED_A, OUTPUT );
	pinMode( PIN_AMPLIFIER_LED_B, OUTPUT );
	pinMode( PIN_AMPLIFIER_LED_C, OUTPUT );
	pinMode( PIN_AMPLIFIER_SAFETY, INPUT_PULLDOWN );

	// Disable amplifier output (as a safety precaution)
	COMMAND_Disable();

	// Set LED initial values
	digitalWriteFast( PIN_AMPLIFIER_LED_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_C, LOW );

	// Set analog resolution
	analogWriteResolution( 12 );	// 12-bit (0 to 4096)

	// Default to zero output
	COMMAND_SetZeroOutput();
}



/*  ==========================================================================
 *  ==========================================================================
 * 
 *   CCCCC     OOOOO     MM      MM    MM      MM    AA     NN   NN    DDDDD
 *  CC        OO   OO    MMMM  MMMM    MMMM  MMMM   AA AA   NNN  NN    DD  DD
 *  CC        OO   OO    MM MMMM MM    MM MMMM MM  AA   AA  NN N NN    DD   DD
 *  CC        OO   OO    MM  MM  MM    MM  MM  MM  AAAAAAA  NN  NNN    DD   DD
 *  CC        OO   OO    MM      MM    MM      MM  AA   AA  NN   NN    DD   DD
 *  CC        OO   OO    MM      MM    MM      MM  AA   AA  NN   NN    DD  DD
 *   CCCCC     OOOOO     MM      MM    MM      MM  AA   AA  NN   NN    DDDDD
 * 
 *  ==========================================================================
 *  ==========================================================================
 */



/**
 * @brief Disable amplifier output
 * 
 */
void AmplifierClass::COMMAND_Disable() {

	// Set amplifier values to zero
	COMMAND_SetZeroOutput();

	// Disable ENABLE pins
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );

	// Update state flags
	Flags.isEnabledA = false;
	Flags.isEnabledB = false;
	Flags.isEnabledC = false;
}



/**
 * @brief Enable amplifier output
 * 
 */
void AmplifierClass::COMMAND_Enable() {

	// Enable ENABLE pins
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, HIGH );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, HIGH );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, HIGH );

	// Update state flag
	Flags.isEnabledA = true;
	Flags.isEnabledB = true;
	Flags.isEnabledC = true;
}



/**
 * @brief Reset amplifier to clear previous configurations
 * (note: reset sequence is ENABLE HIGH to LOW)
 */
void AmplifierClass::COMMAND_Reset() {

	// Send reset sequence to amp A
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, HIGH );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
	delay( 500 );
	// Re-enable amp A
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, HIGH );

	// Send reset sequence to amp B
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, HIGH );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
	delay( 500 );
	// Re-enable amp B
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, HIGH );

	// Send reset sequence to amp C
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, HIGH );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );
	delay( 500 );
	// Re-enable amp C
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, HIGH );
}



/**
 * @brief Sends commands to reset the amplifiers into PWM mode to apply
 * cogging compensation table (set in the Copley Tools app)
 */
void AmplifierClass::COMMAND_ResetIntoPwmCurrentMode() {

	// Reset amplifier to clear previous configurations
	COMMAND_Reset();

	// Send serial command to set amps into current mode
	HWSERIAL_EnqueueQueryA ( HWSerial.Ascii.setCurrentMode ) ; 
	delay ( 600 ) ; 
	digitalWriteFast( PIN_AMPLIFIER_LED_A, HIGH ) ; 
	HWSERIAL_EnqueueQueryB ( HWSerial.Ascii.setCurrentMode ) ; 
	delay ( 600 ) ; 
	digitalWriteFast( PIN_AMPLIFIER_LED_B, HIGH ) ; 
	HWSERIAL_EnqueueQueryC ( HWSerial.Ascii.setCurrentMode ) ; 
	delay ( 600 ) ; 
	digitalWriteFast( PIN_AMPLIFIER_LED_C, HIGH ) ; 

	
}



/**
 * @brief Drive the amplifiers 
 */
void AmplifierClass::COMMAND_SendCommandedPWM( uint16_t pwmA, uint16_t pwmB, uint16_t pwmC ) {

	// Clamp values to 16-bit range
	pwmA = std::clamp( pwmA, uint16_t( 0 ), uint16_t( 4096 ) );
	pwmB = std::clamp( pwmB, uint16_t( 0 ), uint16_t( 4096 ) );
	pwmC = std::clamp( pwmC, uint16_t( 0 ), uint16_t( 4096 ) );

	// Send command to amplifiers
	analogWrite( PIN_AMPLIFIER_PWM_A, pwmA );
	analogWrite( PIN_AMPLIFIER_PWM_B, pwmB );
	analogWrite( PIN_AMPLIFIER_PWM_C, pwmC );
}



/**
 * @brief Sets amplifier values to zero output (2048 in 12-bit scale)
 * 
 */
void AmplifierClass::COMMAND_SetZeroOutput() {

	// Update commanded values
	Command.pwmOldA = Command.pwmNewA;
	Command.pwmOldB = Command.pwmNewB;
	Command.pwmOldC = Command.pwmNewC;

	// Set zero values to PWM pins
	analogWrite( PIN_AMPLIFIER_PWM_A, Command.pwmZero );
	analogWrite( PIN_AMPLIFIER_PWM_B, Command.pwmZero );
	analogWrite( PIN_AMPLIFIER_PWM_C, Command.pwmZero );
}



/**
 * ===================================================================
 * ===================================================================
 * 
 * H   H  W     W     SSSS   EEEEEE  RRRRR    IIIIII   AAAAA   LL
 * H   H  W     W    SS      EE      RR  RR     II    AA   AA  LL
 * H   H  W     W    SS      EE      RR  RR     II    AA   AA  LL
 * HHHHH  W  W  W     SSSS   EEEE    RRRRR      II    AAAAAAA  LL
 * H   H  W W W W        SS  EE      RR  RR     II    AA   AA  LL
 * H   H  WW WW W        SS  EE      RR  RR     II    AA   AA  LL
 * H   H   W   W      SSSS   EEEEEE  RR   RR  IIIIII  AA   AA  LLLLL
 * 
 * =================================================================== 
 * =================================================================== 
 * 
 * The Teensy communicates with the amplifiers using hardware serial ports,
 * with HWSerialA = Serial5, HWSerialB = Serial5, and HWSerialC = Serial3.
 * To properly do these communications, we use a queue of queries,
 * where each query is a pre-defined String variable. The process is as follows:
 * 
 * 1: Use HWSerial_EnqueueQuery to add the query to the queryQueue
 * 2: In our Update loop, make sure we're not awaiting a response and that
 *    the queue is empty, then process the next item in the queue using 
 *    HWSerial_ProcessQuery
 * 3: If bytes are availalbe on the HWSerial port, read the response using
 *    HWSerial_ReadResponse
 * 4: Parse the response using HWSerial_ParseResponse
 */


/*******************************************
 *  HWSerial_EnqueueQuery  - Add to queue  * 
 *******************************************/

/**
 * @brief Add a new HWSerial query the queue for amplifier A
 * @param cmd ASCII command string to send to A
 */
void AmplifierClass::HWSERIAL_EnqueueQueryA( const String& cmd ) {
	HWSerial.Query.queueA.push( cmd );
}

/**
 * @brief Add a new HWSerial query the queue for amplifier B
 * @param cmd ASCII command string to send to B
 */
void AmplifierClass::HWSERIAL_EnqueueQueryB( const String& cmd ) {
	HWSerial.Query.queueB.push( cmd );
}

/**
 * @brief Add a new HWSerial query the queue for amplifier C
 * @param cmd ASCII command string to send to C
 */
void AmplifierClass::HWSERIAL_EnqueueQueryC( const String& cmd ) {
	HWSerial.Query.queueC.push( cmd );
}



/************************************************
 *  HWSerial_ProcessQuery - Process next query  *
 ***********************************************/

/**
  * @brief Process the next query in queue A
  * 
  */
void AmplifierClass::HWSERIAL_ProcessQueryA() {

	// Check if there isn't already a request in waiting, and the queue isn't empty
	if ( !HWSerial.isAwaitingResponseA && !HWSerial.Query.queueA.empty() ) {

		// Extract first query
		HWSerial.Query.outgoingNextA = HWSerial.Query.queueA.front();
		HWSerial.Query.queueA.pop();

		// Update
		HWSerial.Query.responseA	 = "";
		HWSerial.isAwaitingResponseA = true;

		// Send query
		HWSerialA.print( HWSerial.Query.outgoingNextA );
	}
}

/**
  * @brief Process the next query in queue B
  * 
  */
void AmplifierClass::HWSERIAL_ProcessQueryB() {

	// Check if there isn't already a request in waiting, and the queue isn't empty
	if ( !HWSerial.isAwaitingResponseB && !HWSerial.Query.queueB.empty() ) {

		// Extract first query
		HWSerial.Query.outgoingNextB = HWSerial.Query.queueB.front();
		HWSerial.Query.queueA.pop();

		// Update
		HWSerial.Query.responseB	 = "";
		HWSerial.isAwaitingResponseB = true;

		// Send query
		HWSerialB.print( HWSerial.Query.outgoingNextB );
	}
}

/**
  * @brief Process the next query in queue C
  * 
  */
void AmplifierClass::HWSERIAL_ProcessQueryC() {

	// Check if there isn't already a request in waiting, and the queue isn't empty
	if ( !HWSerial.isAwaitingResponseC && !HWSerial.Query.queueC.empty() ) {

		// Extract first query
		HWSerial.Query.outgoingNextC = HWSerial.Query.queueC.front();
		HWSerial.Query.queueA.pop();

		// Update
		HWSerial.Query.responseC	 = "";
		HWSerial.isAwaitingResponseC = true;

		// Send query
		HWSerialC.print( HWSerial.Query.outgoingNextC );
	}
}



/*****************************************************************
 *  HWSerial_ReadQueryResponse - checks for and reads responses  *
 *****************************************************************/

/**
  * @brief Read the response for amplifier A 
  * 
  */
void AmplifierClass::HWSERIAL_ReadQueryResponseA() {


	while ( HWSerialA.available() > 0 ) {

		// Read character
		char incomingChar = ( char )HWSerialA.read();

		// Look for line return
		if ( incomingChar == '\r' ) {

			// Parse string
			HWSERIAL_ParseResponseA();

		} else {

			// Build string
			HWSerial.Query.responseA += incomingChar;
		}
	}
}

/**
  * @brief Read the response for amplifier B
  * 
  */
void AmplifierClass::HWSERIAL_ReadQueryResponseB() {


	while ( HWSerialB.available() > 0 ) {

		// Read character
		char incomingChar = ( char )HWSerialB.read();

		// Look for line return
		if ( incomingChar == '\r' ) {

			// Parse string
			HWSERIAL_ParseResponseB();

		} else {

			// Build string
			HWSerial.Query.responseB += incomingChar;
		}
	}
}

/**
  * @brief Read the response for amplifier C
  * 
  */
void AmplifierClass::HWSERIAL_ReadQueryResponseC() {


	while ( HWSerialC.available() > 0 ) {

		// Read character
		char incomingChar = ( char )HWSerialC.read();

		// Look for line return
		if ( incomingChar == '\r' ) {

			// Parse string
			HWSERIAL_ParseResponseC();

		} else {

			// Build string
			HWSerial.Query.responseC += incomingChar;
		}
	}
}



/****************************
 *  HWSerial_ParseResponse  *
 ****************************/

/**
  * @brief Parse serialA response and save values
  * 
  */
void AmplifierClass::HWSERIAL_ParseResponseA() {

	// Remove "> " if it exists
	if ( HWSerial.Query.responseA.startsWith( "v " ) ) {
		HWSerial.Query.responseA.remove( 0, 2 );
	}

	// Extract response
	String response = HWSerial.Query.responseA;

	// GetBaud
	if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getBaud ) {
		HWSerial.AmpProperty.baudRateA = response.toInt();
	}
	// SetCurrentMode
	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.setCurrentMode ) {
		Flags.isCurrentCommandedA = true;
		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
	}
	// GetName
	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getName ) {
		HWSerial.AmpProperty.nameStringA = response.substring( 2, HWSerial.Query.responseA.length() );
		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
	}
	// GetEncoderCount
	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getEncoderCount ) {
		Measured.countA = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
	}
	// GetCurrent
	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getCurrentReading ) {
		Measured.currentA = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
	}
	// Set encoder zero
	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.setEncoderZero ) {
		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
	}

	// Update
	HWSerial.Query.outgoingNextA = "";
	HWSerial.Query.responseA	 = "";
	HWSerial.isAwaitingResponseA = false;
}

/**
  * @brief Parse serialB response and save values
  * 
  */
void AmplifierClass::HWSERIAL_ParseResponseB() {

	// Remove "> " if it exists
	if ( HWSerial.Query.responseB.startsWith( "v " ) ) {
		HWSerial.Query.responseB.remove( 0, 2 );
	}

	// Extract response
	String response = HWSerial.Query.responseB;

	// GetBaud
	if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getBaud ) {
		HWSerial.AmpProperty.baudRateB = response.toInt();
	}
	// SetCurrentMode
	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.setCurrentMode ) {
		Flags.isCurrentCommandedB = true;
		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
	}
	// GetName
	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getName ) {
		HWSerial.AmpProperty.nameStringB = response.substring( 2, HWSerial.Query.responseB.length() );
		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
	}
	// GetEncoderCount
	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getEncoderCount ) {
		Measured.countB = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
	}
	// GetCurrent
	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getCurrentReading ) {
		Measured.currentB = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
	}
	// Set encoder zero
	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.setEncoderZero ) {
		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
	}

	// Update
	HWSerial.Query.outgoingNextB = "";
	HWSerial.Query.responseB	 = "";
	HWSerial.isAwaitingResponseB = false;
}

/**
  * @brief Parse serialC response and save values
  * 
  */
void AmplifierClass::HWSERIAL_ParseResponseC() {

	// Remove "> " if it exists
	if ( HWSerial.Query.responseC.startsWith( "v " ) ) {
		HWSerial.Query.responseC.remove( 0, 2 );
	}

	// Extract response
	String response = HWSerial.Query.responseC;

	// GetBaud
	if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getBaud ) {
		HWSerial.AmpProperty.baudRateC = response.toInt();
	}
	// SetCurrentMode
	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.setCurrentMode ) {
		Flags.isCurrentCommandedC = true;
		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
	}
	// GetName
	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getName ) {
		HWSerial.AmpProperty.nameStringC = response.substring( 2, HWSerial.Query.responseC.length() );
		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
	}
	// GetEncoderCount
	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getEncoderCount ) {
		Measured.countC = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
	}
	// GetCurrent
	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getCurrentReading ) {
		Measured.currentC = response.toInt();
		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
	}
	// Set encoder zero
	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.setEncoderZero ) {
		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
	}

	// Update
	HWSerial.Query.outgoingNextC = "";
	HWSerial.Query.responseC	 = "";
	HWSerial.isAwaitingResponseC = false;
}



/*  =========================================
 *  =========================================
 * 
 *   FFFFFF  LL      AAAAA    GGGGG    SSSS
 *   FF      LL     AA   AA  GG   GG  SS
 *   FF      LL     AA   AA  GG       SS
 *   FFFF    LL     AAAAAAA  GG  GGG   SSSS
 *   FF      LL     AA   AA  GG   GG      SS
 *   FF      LL     AA   AA  GG   GG      SS
 *   FF      LLLLL  AA   AA   GGGGG    SSSS
 * 
 *  =========================================
 *  ========================================= */

/**
 * @brief Returns the state of the safety switch
 * 
 * @return true Safety switch engaged, system enabled
 * @return false Safety switch disengaged, system disabled
 */
bool AmplifierClass::FLAGS_GetSafetySwitchState() {

	return true;
}

/**
 * @brief Get the string representing the class state
 * @return String text of any class updates / states
 */
String AmplifierClass::FLAGS_GetStateStringA() {
	return "INCOMPLETE";
}

/**
 * @brief Get the string representing the class state
 * @return String text of any class updates / states
 */
String AmplifierClass::FLAGS_GetStateStringB() {
	return "INCOMPLETE";
}

/**
 * @brief Get the string representing the class state
 * @return String text of any class updates / states
 */
String AmplifierClass::FLAGS_GetStateStringC() {
	return "INCOMPLETE";
}
