#include "Amplifier.h"

// Global hook initialization
AmplifierClass* AmplifierClass::instance = nullptr;

AmplifierClass::AmplifierClass() {
	AmplifierClass::instance = this;
}



// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   SSSSS    EEEEEE  TTTTTTT  UU   UU  PPPPP
//  *  SS        EE         T     UU   UU  PP  PP
//  *  SS        EE         T     UU   UU  PP  PP
//  *   SSSS     EEEE       T     UU   UU  PPPPP
//  *      SS    EE         T     UU   UU  PP
//  *      SS    EE         T     UU   UU  PP
//  *   SSSSS    EEEEEE     T      UUUUU   PP
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/


/**
 * @brief Initialize amplifiers and their elements
 */
void AmplifierClass::Begin() {

	// Configure pins
	ConfigurePins();

	// Reset amplifier to clear settings
	Reset();

	// Initialize hardware serial interfaces
	InitializeAmplifierA();
	InitializeAmplifierB();
	InitializeAmplifierC();

	// Send initial zero command to enable output
	CommandZero();

	Serial.println( F( "AMPLIFIER:     All systems nominal." ) );
	// XXX          F(  X              X                                      X
}


/**
 * @brief Initializes the hardware pins
 */
void AmplifierClass::ConfigurePins() {

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

	// Set initial pin states
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_C, LOW );

	// Set analog write resolution
	analogWriteResolution( 12 );	// 12-bit = 0 to 4096

	Serial.println( F( "AMPLIFIER:     Pin configuration...                   Complete." ) );
	// XXX          F(  X              X                                      X
}



/*************************
 * INITIALIZE AMPLIFIERS *
 *************************/


/**
 * @brief Initialize amplifier B
 */
void AmplifierClass::InitializeAmplifierA() {

	// Initialize
	HWSerialA.begin( 9600 );
	delay( 250 );

	// Clear connection
	HWSerialA.clear();

	// Read amplifier name
	SendQueryA( HWSerial.Ascii.getName );
	delay( 250 );

	// Read to confirm connection update
	SendQueryA( HWSerial.Ascii.getBaud );
	delay( 250 );

	// Set amplifier into PWM current-control mode
	SendQueryA( HWSerial.Ascii.setCurrentMode );
	delay( 250 );

	// Set amplifier into higher baud rate
	SendQueryA( HWSerial.Ascii.setBaud115237 );
	delay( 250 );

	// Restart HWSerialA connection
	HWSerialA.end();
	HWSerialA.begin( 115237 );

	// Read to confirm connection update
	SendQueryA( HWSerial.Ascii.getBaud );
	delay( 250 );

	// Send initial zero command to enable output
	CommandZero();

	Serial.println( F( "AMPLIFIER:     Amplifier A initialization...          Complete." ) );
	// XXX          F(  X              X                                      X
}


/**
 * @brief Initialize amplifier B
 */
void AmplifierClass::InitializeAmplifierB() {

	// Initialize
	HWSerialB.begin( 9600 );
	delay( 250 );

	// Clear connection
	HWSerialB.clear();

	// Read amplifier name
	SendQueryB( HWSerial.Ascii.getName );
	delay( 250 );

	// Read to confirm connection update
	SendQueryB( HWSerial.Ascii.getBaud );
	delay( 250 );

	// Set amplifier into PWM current-control mode
	SendQueryB( HWSerial.Ascii.setCurrentMode );
	delay( 250 );

	// Set amplifier into higher baud rate
	SendQueryB( HWSerial.Ascii.setBaud115237 );
	delay( 250 );

	// Restart HWSerialA connection
	HWSerialB.end();
	HWSerialB.begin( 115237 );

	// Read to confirm connection update
	SendQueryB( HWSerial.Ascii.getBaud );
	delay( 250 );

	Serial.println( F( "AMPLIFIER:     Amplifier B initialization...          Complete." ) );
	// XXX          F(  X              X                                      X
}


/**
 * @brief Initialize amplifier C
 */
void AmplifierClass::InitializeAmplifierC() {

	// Initialize
	HWSerialC.begin( 9600 );
	delay( 250 );

	// Clear connection
	HWSerialC.clear();

	// Read amplifier name
	SendQueryC( HWSerial.Ascii.getName );
	delay( 250 );

	// Read to confirm connection update
	SendQueryC( HWSerial.Ascii.getBaud );
	delay( 250 );

	// Set amplifier into PWM current-control mode
	SendQueryC( HWSerial.Ascii.setCurrentMode );
	delay( 250 );

	// Set amplifier into higher baud rate
	SendQueryC( HWSerial.Ascii.setBaud115237 );
	delay( 250 );

	// Restart HWSerialA connection
	HWSerialC.end();
	HWSerialC.begin( 115237 );

	// Read to confirm connection update
	SendQueryC( HWSerial.Ascii.getBaud );
	delay( 250 );

	Serial.println( F( "AMPLIFIER:     Amplifier C initialization...          Complete." ) );
	// XXX          F(  X              X                                      X
}



// /**
//  * ===================================================================
//  * ===================================================================
//  *
//  * H   H  W     W     SSSS   EEEEEE  RRRRR    IIIIII   AAAAA   LL
//  * H   H  W     W    SS      EE      RR  RR     II    AA   AA  LL
//  * H   H  W     W    SS      EE      RR  RR     II    AA   AA  LL
//  * HHHHH  W  W  W     SSSS   EEEE    RRRRR      II    AAAAAAA  LL
//  * H   H  W W W W        SS  EE      RR  RR     II    AA   AA  LL
//  * H   H  WW WW W        SS  EE      RR  RR     II    AA   AA  LL
//  * H   H   W   W      SSSS   EEEEEE  RR   RR  IIIIII  AA   AA  LLLLL
//  *
//  * ===================================================================
//  * ===================================================================
//  *
//  * The Teensy communicates with the amplifiers using hardware serial ports,
//  * with HWSerialA = Serial5, HWSerialB = Serial5, and HWSerialC = Serial3.
//  * To properly do these communications, we use a queue of queries,
//  * where each query is a pre-defined String variable. The process is as follows:
//  *
//  * 1: Use HWSerial_EnqueueQuery to add the query to the queryQueue
//  * 2: In our Update loop, make sure we're not awaiting a response and that
//  *    the queue is empty, then process the next item in the queue using
//  *    HWSerial_ProcessQuery
//  * 3: If bytes are availalbe on the HWSerial port, read the response using
//  *    HWSerial_ReadResponse
//  * 4: Parse the response using HWSerial_ParseResponse
//  */


/*************************
 * SERIAL EVENT HANDLERS *
 *************************/

/**
 * @brief Serial event handler for HWSerial5 (ampA)
 */
void serialEvent5() {

	// Connect hook to class function
	if ( AmplifierClass::instance ) {
		AmplifierClass::instance->OnHWSerialAEvent();
	}
}


/**
 * @brief Serial event handler for HWSerial4 (ampB)
 */
void serialEvent4() {

	// Connect hook to class function
	if ( AmplifierClass::instance ) {
		AmplifierClass::instance->OnHWSerialBEvent();
	}
}


/**
 * @brief Serial event handler for HWSerial3 (ampC)
 */
void serialEvent3() {

	// Connect hook to class function
	if ( AmplifierClass::instance ) {
		AmplifierClass::instance->OnHWSerialCEvent();
	}
}


/*************************
 * SERIAL EVENT CALLBACK *
 *************************/

/**
 * @brief Callback for HWSerialA (port 5)
 */
void AmplifierClass::OnHWSerialAEvent() {

	// Read data while buffer is populated
	while ( HWSerialA.available() > 0 ) {

		// Read each byte
		char incomingChar = ( char )HWSerialA.read();

		// Look for terminating character
		if ( incomingChar == '\r' || incomingChar == '\n' ) {

			// Make sure packet isn't empty
			if ( HWSerial.Packets.respondingQueryA != "" ) {

				// Parse packet
				ParseQueryA();
			}
		} else {
			HWSerial.Packets.respondingQueryA += incomingChar;
		}
	}
}


/**
 * @brief Callback for HWSerialB (port 4)
 */
void AmplifierClass::OnHWSerialBEvent() {

	// Read data while buffer is populated
	while ( HWSerialB.available() > 0 ) {

		// Read each byte
		char incomingChar = ( char )HWSerialB.read();

		// Look for terminating character
		if ( incomingChar == '\r' || incomingChar == '\n' ) {

			// Make sure packet isn't empty
			if ( HWSerial.Packets.respondingQueryB != "" ) {

				// Parse packet
				ParseQueryB();
			}
		} else {
			HWSerial.Packets.respondingQueryB += incomingChar;
		}
	}
}


/**
 * @brief Callback for HWSerialC (port 3)
 */
void AmplifierClass::OnHWSerialCEvent() {

	// Read data while buffer is populated
	while ( HWSerialC.available() > 0 ) {

		// Read each byte
		char incomingChar = ( char )HWSerialC.read();

		// Look for terminating character
		if ( incomingChar == '\r' || incomingChar == '\n' ) {

			// Make sure packet isn't empty
			if ( HWSerial.Packets.respondingQueryC != "" ) {

				// Parse packet
				ParseQueryC();
			}
		} else {
			HWSerial.Packets.respondingQueryC += incomingChar;
		}
	}
}


/**************
 * SEND QUERY *
 **************/

/**
 * @brief Sends a query string to amp A
 */
void AmplifierClass::SendQueryA( String newQuery ) {

	// Update outgoing query
	HWSerial.Packets.outgoingQueryA = newQuery;

	// Send packet
	HWSerialA.print( newQuery );
}


/**
 * @brief Sends a query string to amp B
 */
void AmplifierClass::SendQueryB( String newQuery ) {

	// Update outgoing query
	HWSerial.Packets.outgoingQueryB = newQuery;

	// Send packet
	HWSerialB.print( newQuery );
}


/**
 * @brief Sends a query string to amp C
 */
void AmplifierClass::SendQueryC( String newQuery ) {

	// Update outgoing query
	HWSerial.Packets.outgoingQueryC = newQuery;

	// Send packet
	HWSerialC.print( newQuery );
}


/***************
 * PARSE QUERY *
 ***************/

/**
 * @brief Parse packet from amp A
 */
void AmplifierClass::ParseQueryA() {

	// Extract response
	String responseA = HWSerial.Packets.respondingQueryA;
	// Serial.print( "DEBUG! Response: " );
	// Serial.println( responseA );

	// Get baud
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.getBaud ) {
		HWSerial.Properties.baudRateA = responseA.substring( 2, responseA.length() ).toInt();
	}

	// Get encoder reading
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.getEncoderCount ) {
		// Sensors.measuredEncoderCountA = responseA.substring( 2, responseA.length() ).toInt();
		const long cnt				  = strtol( responseA.c_str(), nullptr, 10 );
		Sensors.measuredEncoderCountA = static_cast<int32_t>( cnt );
		Sensors.angleDegA			  = degrees( Sensors.measuredEncoderCountA * 2.0f * M_PI / 4096.0f );
	}

	// Get name
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.getName ) {
		HWSerial.Properties.ampNameA = responseA.substring( 2, responseA.length() );
	}

	// Set 115237 baud rate
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.setBaud115237 ) {
		// Nothing to do
	}

	// Set current-control mode
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.setCurrentMode ) {

		// Make sure amplifier acknowledged mode change
		if ( responseA == "ok" ) {
			Command.Flags.isCurrentCOmmandedA = true;
		} else {
			Command.Flags.isCurrentCOmmandedA = false;
			Serial.println( F( "Amplifier A current command change failed!" ) );
		}
	}

	// Set amp initial state (reset)
	if ( HWSerial.Packets.outgoingQueryA == HWSerial.Ascii.setReset ) {
		Serial.println( "Resetting" );
	}

	// Clear packet info
	HWSerial.Packets.outgoingQueryA	  = HWSerial.Ascii.setIdle;
	HWSerial.Packets.respondingQueryA = "";
}


/**
 * @brief Parse packet from amp B
 */
void AmplifierClass::ParseQueryB() {

	// Extract response
	String responseB = HWSerial.Packets.respondingQueryB;
	// Serial.print( "DEBUG! Response: " );
	// Serial.println( responseA );

	// Get baud
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.getBaud ) {
		HWSerial.Properties.baudRateB = responseB.substring( 2, responseB.length() ).toInt();
	}

	// Get encoder reading
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.getEncoderCount ) {
		const long cnt				  = strtol( responseB.c_str(), nullptr, 10 );
		Sensors.measuredEncoderCountB = static_cast<int32_t>( cnt );
		Sensors.angleDegB			  = degrees( Sensors.measuredEncoderCountB * 2.0f * M_PI / 4096.0f );
	}

	// Get name
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.getName ) {
		HWSerial.Properties.ampNameB = responseB.substring( 2, responseB.length() );
	}

	// Set 115237 baud rate
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.setBaud115237 ) {
		// Nothing
	}

	// Set current-control mode
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.setCurrentMode ) {

		// Make sure amplifier acknowledged mode change
		if ( responseB == "ok" ) {
			Command.Flags.isCurrentCOmmandedB = true;
		} else {
			Command.Flags.isCurrentCOmmandedB = false;
			Serial.println( F( "Amplifier B current command change failed!" ) );
		}
	}

	// Set amp initial state (reset)
	if ( HWSerial.Packets.outgoingQueryB == HWSerial.Ascii.setReset ) {
		Serial.println( "Resetting" );
	}

	// Clear packet info
	HWSerial.Packets.outgoingQueryB	  = HWSerial.Ascii.setIdle;
	HWSerial.Packets.respondingQueryB = "";
}


/**
 * @brief Parse packet from amp B
 */
void AmplifierClass::ParseQueryC() {

	// Extract response
	String responseC = HWSerial.Packets.respondingQueryC;
	// Serial.print( "DEBUG! Response: " );
	// Serial.println( responseA );

	// Get baud
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.getBaud ) {
		HWSerial.Properties.baudRateC = responseC.substring( 2, responseC.length() ).toInt();
	}

	// Get encoder reading
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.getEncoderCount ) {
		const long cnt				  = strtol( responseC.c_str(), nullptr, 10 );
		Sensors.measuredEncoderCountC = static_cast<int32_t>( cnt );
		Sensors.angleDegC			  = degrees( Sensors.measuredEncoderCountC * 2.0f * M_PI / 4096.0f );
	}

	// Get name
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.getName ) {
		HWSerial.Properties.ampNameC = responseC.substring( 2, responseC.length() );
	}

	// Set 115237 baud rate
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.setBaud115237 ) {
		// Nothing
	}

	// Set current-control mode
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.setCurrentMode ) {

		// Make sure amplifier acknowledged mode change
		if ( responseC == "ok" ) {
			Command.Flags.isCurrentCOmmandedC = true;
		} else {
			Command.Flags.isCurrentCOmmandedC = false;
			Serial.println( F( "Amplifier C current command change failed!" ) );
		}
	}

	// Set amp initial state (reset)
	if ( HWSerial.Packets.outgoingQueryC == HWSerial.Ascii.setReset ) {
		Serial.println( "Resetting" );
	}

	// Clear packet info
	HWSerial.Packets.outgoingQueryC	  = HWSerial.Ascii.setIdle;
	HWSerial.Packets.respondingQueryC = "";
}



// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *    AA     CCCCC   CCCCC  EEEEEE  SSSSS   SSSSS   OOOOO   RRRRR    SSSSS
//  *  AA  AA  CC      CC      EE     SS      SS      OO   OO  RR  RR  SS
//  *  AA  AA  CC      CC      EE     SS      SS      OO   OO  RR  RR  SS
//  *  AAAAAA  CC      CC      EEEE    SSSS    SSSS   OO   OO  RRRRR    SSSS
//  *  AA  AA  CC      CC      EE         SS      SS  OO   OO  RR  RR      SS
//  *  AA  AA  CC      CC      EE         SS      SS  OO   OO  RR  RR      SS
//  *  AA  AA   CCCCC   CCCCC  EEEEEE  SSSSS   SSSSS   OOOOO   RR  RR  SSSSS
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/



/*********************
 * GET AMPLFIER NAME *
 *********************/

/**
 * @brief Gets the name of amplifier A
 * @return * String Name of amplifier
 */
String AmplifierClass::GetAmplifierNameA() {

	return HWSerial.Properties.ampNameA;
}


/**
 * @brief Gets the name of amplifier B
 * @return * String Name of amplifier
 */
String AmplifierClass::GetAmplifierNameB() {

	return HWSerial.Properties.ampNameB;
}


/**
 * @brief Gets the name of amplifier C
 * @return * String Name of amplifier
 */
String AmplifierClass::GetAmplifierNameC() {

	return HWSerial.Properties.ampNameC;
}


/************
 * GET BAUD *
 ************/

/**
 * @brief Returns the baud rate of amp A
 * @return int32_t baud rate
 */
int32_t AmplifierClass::GetBaudA() {

	return HWSerial.Properties.baudRateA;
}


/**
 * @brief Returns the baud rate of amp B
 * @return int32_t baud rate
 */
int32_t AmplifierClass::GetBaudB() {

	return HWSerial.Properties.baudRateB;
}

/**
 * @brief Returns the baud rate of amp C
 * @return int32_t baud rate
 */
int32_t AmplifierClass::GetBaudC() {

	return HWSerial.Properties.baudRateC;
}


/************
 * SET TENSION *
 ************/

/**
 * @brief Set the tension value for the three amplifiers
 * @param tenA, tenB, tenC Tension to apply in percentage of max
 */
void AmplifierClass::SetTension( uint8_t tenA, uint8_t tenB, uint8_t tenC ) {

	// Calculate appropriate tension
	Command.Pwm.tensionA = int( float( tenA / 100.0f ) * 2048.0f );
	Command.Pwm.tensionB = int( float( tenB / 100.0f ) * 2048.0f );
	Command.Pwm.tensionC = int( float( tenC / 100.0f ) * 2048.0f );

	// Update results
	Serial.print( F( "   Amplifier: Tension updated to " ) );
	Serial.print( Command.Pwm.tensionC );
}


/************************
 * GET ANGLE IN DEGREES *
 ************************/

/**
 * @brief Returns the angle of amp encoder A
 * @return float angle in degrees
 */
float AmplifierClass::GetAngleDegA() {

	return Sensors.angleDegA ;
}


/**
 * @brief Returns the angle of amp encoder B
 * @return float angle in degrees
 */
float AmplifierClass::GetAngleDegB() {

	return Sensors.angleDegB ;
}


/**
 * @brief Returns the angle of amp encoder B
 * @return float angle in degrees
 */
float AmplifierClass::GetAngleDegC() {

	return Sensors.angleDegC ;
}


// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   CCCCC   OOOOO   MM     MM  MM     MM    AA    NN   NN  DDDDD
//  *  CC      OO   OO  MMM   MMM  MMM   MMM  AA  AA  NNN  NN  DD  DD
//  *  CC      OO   OO  MM M M MM  MM M M MM  AA  AA  NNNN NN  DD  DD
//  *  CC      OO   OO  MM  M  MM  MM  M  MM  AAAAAA  NN NNNN  DD  DD
//  *  CC      OO   OO  MM     MM  MM     MM  AA  AA  NN  NNN  DD  DD
//  *  CC      OO   OO  MM     MM  MM     MM  AA  AA  NN   NN  DD  DD
//  *   CCCCC   OOOOO   MM     MM  MM     MM  AA  AA  NN   NN  DDDDD
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/


/**
 * @brief Drive the amplifiers
 */
void AmplifierClass::CommandPWM() {

	// Calculate sums for each amplifier
	Command.Pwm.totalOutgoingA = Command.Pwm.outgoingA - Command.Pwm.tensionA;
	Command.Pwm.totalOutgoingB = Command.Pwm.outgoingB - Command.Pwm.tensionB;
	Command.Pwm.totalOutgoingC = Command.Pwm.outgoingC - Command.Pwm.tensionC;

	// Constrain values to 16-bit range
	Command.Pwm.totalOutgoingA = constrain( Command.Pwm.totalOutgoingA, Command.Pwm.CONST_PWM_MAX, Command.Pwm.CONST_PWM_ZERO );
	Command.Pwm.totalOutgoingB = constrain( Command.Pwm.totalOutgoingB, Command.Pwm.CONST_PWM_MAX, Command.Pwm.CONST_PWM_ZERO );
	Command.Pwm.totalOutgoingC = constrain( Command.Pwm.totalOutgoingC, Command.Pwm.CONST_PWM_MAX, Command.Pwm.CONST_PWM_ZERO );


	// Make sure safety switch is engaged and output enabled
	if ( Command.Flags.isOutputEnabled && Command.Flags.isSafetySwitchEnabled ) {

		// Write analog values
		analogWrite( PIN_AMPLIFIER_PWM_A, Command.Pwm.totalOutgoingA );
		analogWrite( PIN_AMPLIFIER_PWM_B, Command.Pwm.totalOutgoingB );
		analogWrite( PIN_AMPLIFIER_PWM_C, Command.Pwm.totalOutgoingC );
	} else {

		// Write zeros
		CommandZero();
	}
}


/**
 * @brief Send zeros to the PWM output
 */
void AmplifierClass::CommandZero() {

	// Set total to zero
	Command.Pwm.totalOutgoingA = Command.Pwm.CONST_PWM_ZERO;
	Command.Pwm.totalOutgoingB = Command.Pwm.CONST_PWM_ZERO;
	Command.Pwm.totalOutgoingC = Command.Pwm.CONST_PWM_ZERO;

	// Send zero
	analogWrite( PIN_AMPLIFIER_PWM_A, Command.Pwm.totalOutgoingA );
	analogWrite( PIN_AMPLIFIER_PWM_B, Command.Pwm.totalOutgoingB );
	analogWrite( PIN_AMPLIFIER_PWM_C, Command.Pwm.totalOutgoingC );
}



/***
 * *****
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */



/**
 * @brief Reset amplifier to clear previous configurations
 * (note: reset sequence is ENABLE HIGH to LOW)
 */
void AmplifierClass::Reset() {

	// Send reset sequence to amp A
	digitalWrite( PIN_AMPLIFIER_ENABLE_A, HIGH );
	digitalWrite( PIN_AMPLIFIER_ENABLE_A, LOW );
	delay( 500 );
	digitalWrite( PIN_AMPLIFIER_ENABLE_A, HIGH );

	// Send reset sequence to amp B
	digitalWrite( PIN_AMPLIFIER_ENABLE_B, HIGH );
	digitalWrite( PIN_AMPLIFIER_ENABLE_B, LOW );
	delay( 500 );
	digitalWrite( PIN_AMPLIFIER_ENABLE_B, HIGH );

	// Send reset sequence to amp C
	digitalWrite( PIN_AMPLIFIER_ENABLE_C, HIGH );
	digitalWrite( PIN_AMPLIFIER_ENABLE_C, LOW );
	delay( 500 );
	digitalWrite( PIN_AMPLIFIER_ENABLE_C, HIGH );

	Serial.println( F( "AMPLIFIER:     Reset command...                       Complete." ) );
	// XXX          F(  X              X                                      X
}



/**
 * @brief Returns the state of the safety switch
 *
 * @return true Safety switch engaged, system enabled
 * @return false Safety switch disengaged, system disabled
 */
bool AmplifierClass::GetSafetySwitchState() {

	// Read state
	Command.Flags.isSafetySwitchEnabled = digitalReadFast( PIN_AMPLIFIER_SAFETY );
	return Command.Flags.isSafetySwitchEnabled;
}

/**
 * @brief Disable amplifier A
 */
void AmplifierClass::DisableA() {

	// Write pin low
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
}


/**
 * @brief Enable amplifier A
 */
void AmplifierClass::EnableA() {

	// Write pin high
	digitalWrite( PIN_AMPLIFIER_ENABLE_A, HIGH );
}



// /**
//  * @brief Initialize the amplifier
//  *
//  * @param newFreqPWMDriverHz Frequency to drive PWM signal at
//  * @param newFreqHWSerialHz Frequency to read HW serial at
//  */
// void AmplifierClass::Begin( uint16_t newFreqPWMDriverHz, uint16_t newFreqHWSerialHz ) {

// 	// Update frequencies
// 	freqPWMDriverHz = newFreqPWMDriverHz;
// 	freqHWSerialHz	= newFreqHWSerialHz;

// 	// Configure hardware IO (pinMode, initial pin state, set analog resolution)
// 	CONFIGURE_ConfigurePins();


// 	// Zero analog output
// 	COMMAND_SetZeroOutput();

// 	// // Reset to clear settings
// 	COMMAND_Reset();
// 	delay( 1500 );


// 	// Initialize hardware serial to amplifiers
// 	HWSERIAL_InitializeHardwareSerial();

// 	// Configure amplifier for PWM current-control (resets, sends current mode command)
// 	// HWSERIAL_SetPwmCurrentMode();

// 	// Increase hardware serial speed
// 	HWSERIAL_IncreaseHardwareSerialSpeed();

// 	// Get amplifier names
// 	// HWSERIAL_GetAmplifierNames();

// 	// Configure IO
// 	// Disable
// 	// Zero output
// 	// HWSerial
// 	// Baud update
// 	// Current Mode



// 	// Enable amplifier
// 	// COMMAND_Enable();

// 	delay( 500 );	 // Allow boot sequence to settle

// 	Serial.println( F( "Amplifier initialization...                   Success!" ) );
// }



// /**
//  * @brief Function that gets called every loop to update serial interface
//  *
//  */
// void AmplifierClass::UpdatePWM() {

// 	// Check safety switch
// 	FLAGS_GetSafetySwitchState();

// 	// Send signals
// 	COMMAND_SendCommandedPWM();
// }


// void AmplifierClass::UpdateEncoders() {

// 	// Read Encoders
// 	// READ_Encoders();
// 	// phiA = Read.angleDegA;
// 	// phiB = Read.angleDegB;
// 	// phiC = Read.angleDegC;



// 	// Callback to HWSerialA
// 	HWSERIAL_PushQueryA();
// 	if ( HWSerialA.available() ) {
// 		HWSERIAL_ReadQueryResponseA();
// 	}

// 	// Callback to HWSerialB
// 	HWSERIAL_PushQueryB();
// 	if ( HWSerialB.available() ) {
// 		HWSERIAL_ReadQueryResponseB();
// 	}

// 	// Callback to HWSerialC
// 	HWSERIAL_PushQueryC();
// 	if ( HWSerialC.available() ) {
// 		HWSERIAL_ReadQueryResponseC();
// 	}
// }



// /*  ===================================================================================
//  *  ===================================================================================
//  *
//  *   CCCCC     OOOOO    NN   NN   FFFFFF  IIIIII    GGGGG    UU   UU   RRRRR     EEEEEE
//  *  CC        OO   OO   NNN  NN   FF        II     GG   GG   UU   UU   RR  RR    EE
//  *  CC        OO   OO   NN N NN   FF        II     GG        UU   UU   RR  RR    EE
//  *  CC        OO   OO   NN  NNN   FFFF      II     GG  GGG   UU   UU   RRRRR     EEEE
//  *  CC        OO   OO   NN   NN   FF        II     GG   GG   UU   UU   RR  RR    EE
//  *  CC        OO   OO   NN   NN   FF        II     GG   GG   UU   UU   RR  RR    EE
//  *   CCCCC     OOOOO    NN   NN   FF      IIIIII    GGGGG     UUUUU    RR   RR   EEEEEE
//  *
//  *  ===================================================================================
//  *  ===================================================================================*/



// /**
//  * @brief Configures the hardware serial interface with the amplifiers
//  */
// void AmplifierClass::HWSERIAL_InitializeHardwareSerial() {

// 	Serial.println( "Entering HWSERIAL_InitializeHardwareSerial" );
// 	// Clear states
// 	HWSerialA.clear();
// 	HWSerialB.clear();
// 	HWSerialC.clear();
// 	// HWSerialA.flush();
// 	// HWSerialB.flush();
// 	// HWSerialC.flush();

// 	// Establish hardware serial connection amp A
// 	HWSerialA.begin( 9600 );
// 	delay( 100 );
// 	HWSerialA.clear();
// 	// HWSerialA.flush();
// 	digitalWriteFast( PIN_AMPLIFIER_LED_A, HIGH );

// 	// Establish hardware serial connection amp B
// 	HWSerialB.begin( 9600 );
// 	delay( 100 );
// 	HWSerialB.clear();
// 	// HWSerialB.flush();
// 	digitalWriteFast( PIN_AMPLIFIER_LED_B, HIGH );

// 	// Establish hardware serial connection amp C
// 	HWSerialC.begin( 9600 );
// 	delay( 100 );
// 	HWSerialC.clear();
// 	// HWSerialC.flush();
// 	digitalWriteFast( PIN_AMPLIFIER_LED_C, HIGH );

// 	// Get baud rates to confirm
// 	HWSERIAL_GetAmplifierBaudRates();

// 	Serial.println( "HWSERIAL_InitializeHardwareSerial complete" );
// }



// bool AmplifierClass::HWSERIAL_WaitForPortA(uint32_t ms) {
//     const uint32_t t0 = millis();
//     while (HWSerial.isAwaitingResponseA) {
//         if (HWSerialA.available()) {
//             HWSERIAL_ReadQueryResponseA();   // make sure this parses on '\r' OR '\n'
//         } else if (millis() - t0 > ms) {
//             HWSerial.isAwaitingResponseA = false;   // timeout: bail cleanly
// 			// HWSerialA.clear();
//             return false;
//         }
//         yield();
//     }
//     return true;
// }

// bool AmplifierClass::HWSERIAL_WaitForPortB(uint32_t ms) {
//     const uint32_t t0 = millis();
//     while (HWSerial.isAwaitingResponseB) {
//         if (HWSerialB.available()) {
//             HWSERIAL_ReadQueryResponseB();
//         } else if (millis() - t0 > ms) {
//             HWSerial.isAwaitingResponseB = false;
// 			// HWSerialB.clear();
//             return false;
//         }
//         yield();
//     }
//     return true;
// }

// bool AmplifierClass::HWSERIAL_WaitForPortC(uint32_t ms) {
//     const uint32_t t0 = millis();
//     while (HWSerial.isAwaitingResponseC) {
//         if (HWSerialC.available()) {
//             HWSERIAL_ReadQueryResponseC();
//         } else if (millis() - t0 > ms) {
//             HWSerial.isAwaitingResponseC = false;
// 			// HWSerialC.clear();
//             return false;
//         }
//         yield();
//     }
//     return true;
// }

// /**
//  * @brief Update hardware serial speed to 115237
//  */
// void AmplifierClass::HWSERIAL_IncreaseHardwareSerialSpeed() {

// 	Serial.println( "Entering HWSERIAL_IncreaseHWSerialSPeed " );

// 	HWSerialA.clear();
// 	HWSerialB.clear();
// 	HWSerialC.clear();
// 	HWSerialA.flush();
// 	HWSerialB.flush();
// 	HWSerialC.flush();

// 	// Send speed update cues
// 	HWSERIAL_EnqueueQueryA( HWSerial.Ascii.setBaud115237 );
// 	delay( 100 );

// 	// Push queries out
// 	HWSERIAL_PushQueryA();
// 	HWSerial.isAwaitingResponseA = false;
// 	delay( 300 );

// 	// // Wait for responses before proceeding
// 	// while ( HWSerial.isAwaitingResponseA ) {
// 	// 	Serial.println( "WhileA " );
// 	// 	if ( HWSerialA.available() ) {
// 	// 		Serial.println( "AvailA" );
// 	// 		HWSERIAL_ReadQueryResponseA();
// 	// 	} else if ( !HWSerialA.available() ) {
// 	// 		Serial.println( "!AvailA" );
// 	// 	}
// 	// }

// 	HWSERIAL_EnqueueQueryB( HWSerial.Ascii.setBaud115237 );
// 	delay( 100 );

// 	HWSERIAL_PushQueryB();
// 	delay( 300 );
// 	HWSerial.isAwaitingResponseB = false;

// 	// while ( HWSerial.isAwaitingResponseB ) {
// 	// 	Serial.println( "WhileB " );
// 	// 	if ( HWSerialB.available() ) {
// 	// 		Serial.println( "AvailB" );
// 	// 		HWSERIAL_ReadQueryResponseB();
// 	// 	} else if ( !HWSerialB.available() ) {
// 	// 		Serial.println( "!AvailB" );
// 	// 	}
// 	// }
// 	// delay( 500 );


// 	HWSERIAL_EnqueueQueryC( HWSerial.Ascii.setBaud115237 );
// 	delay( 100 );

// 	HWSERIAL_PushQueryC();
// 	delay( 300 );
// 	HWSerial.isAwaitingResponseC = false;

// 	// while ( HWSerial.isAwaitingResponseC ) {
// 	// 	Serial.println( "WhileC " );
// 	// 	if ( HWSerialC.available() ) {
// 	// 		Serial.println( "AvailC" );
// 	// 		HWSERIAL_ReadQueryResponseC();
// 	// 	} else if ( !HWSerialC.available() ) {
// 	// 		Serial.println( "!AvailC" );
// 	// 	}
// 	// }
// 	// delay( 500 );

// 	// Push queries and wait for response
// 	// HWSERIAL_PushAllAndWaitForResponses();


// 	// Close serial connections
// 	HWSerialA.end();
// 	HWSerialB.end();
// 	HWSerialC.end();
// 	Serial.println( "HWSerial ended" );
// 	delay( 300 );


// 	// HWSerialA.flush();
// 	// HWSerialB.flush();
// 	// HWSerialC.flush();


// 	// Re-connect with faster  serial connection
// 	HWSerialA.begin( 115237 );
// 	delay( 100 );
// 	HWSerialA.clear();
// 	HWSerialA.flush();
// 	HWSerialB.begin( 115237 );
// 	delay( 100 );
// 	HWSerialB.clear();
// 	HWSerialB.flush();
// 	HWSerialC.begin( 115237 );
// 	delay( 100 );
// 	HWSerialC.clear();
// 	HWSerialC.flush();
// 	Serial.println( "HWSerial began" );

// 	// Push queries and wait for response
// 	// HWSERIAL_PushAllAndWaitForResponses();
// 	//

// 	// Get baud rates
// 	HWSERIAL_GetAmplifierBaudRates();

// 	// Push queries and wait for response
// 	// HWSERIAL_PushAllAndWaitForResponses();

// 	Serial.print( "DEBUG: IncreaseHWSerialSPeed Start  -->  " );

// 	Serial.println( F( "Amplifier HWSerial speed increase...          Success!" ) );
// }



// /**
//  * @brief Initializes the hardware pins
//  *
//  */
// void AmplifierClass::CONFIGURE_ConfigurePins() {

// 	// Initialize pins
// 	pinMode( PIN_AMPLIFIER_ENABLE_A, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_ENABLE_B, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_ENABLE_C, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_PWM_A, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_PWM_B, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_PWM_C, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_LED_A, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_LED_B, OUTPUT );
// 	pinMode( PIN_AMPLIFIER_LED_C, OUTPUT );

// 	// Set initial pin states
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_LED_A, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_LED_B, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_LED_C, LOW );

// 	// Set analog write resolution
// 	analogWriteResolution( 12 );	// 12-bit = 0 to 4096

// 	Serial.println( "CONFIGURE_ConfigurePins complete" );
// }



// /*  ==========================================================================
//  *  ==========================================================================
//  *
//  *   CCCCC     OOOOO     MM      MM    MM      MM    AA     NN   NN    DDDDD
//  *  CC        OO   OO    MMMM  MMMM    MMMM  MMMM   AA AA   NNN  NN    DD  DD
//  *  CC        OO   OO    MM MMMM MM    MM MMMM MM  AA   AA  NN N NN    DD   DD
//  *  CC        OO   OO    MM  MM  MM    MM  MM  MM  AAAAAAA  NN  NNN    DD   DD
//  *  CC        OO   OO    MM      MM    MM      MM  AA   AA  NN   NN    DD   DD
//  *  CC        OO   OO    MM      MM    MM      MM  AA   AA  NN   NN    DD  DD
//  *   CCCCC     OOOOO     MM      MM    MM      MM  AA   AA  NN   NN    DDDDD
//  *
//  *  ==========================================================================
//  *  ==========================================================================
//  */



// /**
//  * @brief Disable amplifier output
//  *
//  */
// void AmplifierClass::COMMAND_Disable() {

// 	// Disable ENABLE pins
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );

// 	// Update state flags
// 	Flags.isEnabled = false;
// 	Serial.println( "Disable complete" );

// 	// // Zero outputs
// 	// COMMAND_SetZeroOutput();
// }



// /**
//  * @brief Enable amplifier output
//  *
//  */
// void AmplifierClass::COMMAND_Enable() {

// 	// Enable ENABLE pins
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, HIGH );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, HIGH );
// 	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, HIGH );

// 	// Update state flag
// 	Flags.isEnabled = true;
// }



// /**
//  * @brief Reset amplifier to clear previous configurations
//  * (note: reset sequence is ENABLE HIGH to LOW)
//  */
// void AmplifierClass::COMMAND_Reset() {

// 	// Send reset sequence to amp A
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_A, HIGH );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_A, LOW );
// 	delay( 500 );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_A, HIGH );

// 	// Send reset sequence to amp B
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_B, HIGH );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_B, LOW );
// 	delay( 500 );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_B, HIGH );

// 	// Send reset sequence to amp C
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_C, HIGH );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_C, LOW );
// 	delay( 500 );
// 	digitalWrite( PIN_AMPLIFIER_ENABLE_C, HIGH );

// 	Serial.println( "COMMAND_Reset complete" );
// }



// /**
//  * @brief Sends commands to reset the amplifiers into PWM mode to apply
//  * cogging compensation table (set in the Copley Tools app)
//  */
// void AmplifierClass::HWSERIAL_SetPwmCurrentMode() {

// 	Serial.print( "DEBUG: InitializeHWSerial Start  -->  " );


// 	// Reset amplifier to clear previous configurations
// 	COMMAND_Reset();
// 	delay( 500 );

// 	// Send serial command to set amps into current mode
// 	HWSERIAL_EnqueueQueryA( HWSerial.Ascii.setCurrentMode );
// 	delay( 100 );
// 	// HWSERIAL_ProcessQueryA();

// 	HWSERIAL_EnqueueQueryB( HWSerial.Ascii.setCurrentMode );
// 	delay( 100 );
// 	// HWSERIAL_ProcessQueryB();

// 	HWSERIAL_EnqueueQueryC( HWSerial.Ascii.setCurrentMode );
// 	delay( 100 );

// 	HWSERIAL_PushAllAndWaitForResponses();



// 	// // Callback to HWSerialA
// 	// HWSERIAL_PushQueryA();
// 	// delay( 100 );
// 	// if ( HWSerialA.available() ) {
// 	// 	HWSERIAL_ReadQueryResponseA();
// 	// }

// 	// // Callback to HWSerialB
// 	// HWSERIAL_PushQueryB();
// 	// delay( 100 );
// 	// if ( HWSerialB.available() ) {
// 	// 	HWSERIAL_ReadQueryResponseB();
// 	// }

// 	// // Callback to HWSerialC
// 	// HWSERIAL_PushQueryC();
// 	// delay( 100 );
// 	// if ( HWSerialC.available() ) {
// 	// 	HWSERIAL_ReadQueryResponseC();
// 	// }


// 	// HWSERIAL_PushAllAndWaitForResponses();
// 	Serial.println( "DEBUG: InitializeHWSerial Complete" );

// 	Serial.println( F( "Amplifier current control initialization...   Success!" ) );
// }



// /**
//  * @brief Sends commands to reset the amplifiers into PWM mode to apply
//  * cogging compensation table (set in the Copley Tools app)
//  */
// void AmplifierClass::HWSERIAL_SetEncodersZero() {

// 	// // Reset responses
// 	// HWSerial.isAwaitingResponseA = false;
// 	// HWSerial.isAwaitingResponseB = false;
// 	// HWSerial.isAwaitingResponseC = false;
// 	// HWSerial.Query.responseA	 = "";
// 	// HWSerial.Query.responseB	 = "";
// 	// HWSerial.Query.responseC	 = "";
// 	// HWSerial.Query.outgoingNextA = "";
// 	// HWSerial.Query.outgoingNextB = "";
// 	// HWSerial.Query.outgoingNextC = "";

// 	// Reset amplifier to clear previous configurations
// 	HWSERIAL_EnqueueQueryA( HWSerial.Ascii.setEncoderZero );
// 	delay( 200 );
// 	HWSERIAL_PushQueryA();
// 	HWSERIAL_EnqueueQueryB( HWSerial.Ascii.setEncoderZero );
// 	delay( 200 );
// 	HWSERIAL_PushQueryB();
// 	HWSERIAL_EnqueueQueryC( HWSerial.Ascii.setEncoderZero );
// 	delay( 200 );
// 	HWSERIAL_PushQueryC();

// 	delay( 400 );

// 	// Process queries

// 	if ( HWSerialA.available() ) {
// 		HWSERIAL_ReadQueryResponseA();
// 	}
// 	if ( HWSerialB.available() ) {
// 		HWSERIAL_ReadQueryResponseB();
// 	}
// 	if ( HWSerialC.available() ) {
// 		HWSERIAL_ReadQueryResponseC();
// 	}

// 	delay( 100 );
// 	// Serial.println( F( "Amplifier encoder initialization...           Success!" ) );
// }



// /**
//  * @brief Sets the tensioning value, which is added to all PWM commands
//  * @param tensionPercentage Tensioning value, between 0 and 25%
//  */
// void AmplifierClass::COMMAND_SetTension( uint8_t tensionPercentage ) {

 // 	Serial.println( " [pwm]." );
 // }


 // /**
 //  * @brief Sets amplifier values to zero output (2048 in 12-bit scale)
 //  *
 //  */
 // void AmplifierClass::COMMAND_SetZeroOutput() {

 // 	// Set zero values to PWM pins
 // 	Command.commandedPwmA = Command.pwmZero;
 // 	Command.commandedPwmB = Command.pwmZero;
 // 	Command.commandedPwmC = Command.pwmZero;

 // 	analogWrite( PIN_AMPLIFIER_PWM_A, Command.commandedPwmA );
 // 	analogWrite( PIN_AMPLIFIER_PWM_B, Command.commandedPwmB );
 // 	analogWrite( PIN_AMPLIFIER_PWM_C, Command.commandedPwmC );

 // 	Serial.println( "COMMAND_SetZeroOutput complete" );
 // }

 // /**
 //  * @brief Interval timer callback for hardware serial interface
 //  */
 // void AmplifierClass::IT_HWSerial_Callback() {

 // 	// Process outgoing query packets
 // 	HWSERIAL_PushQueryA();
 // 	HWSERIAL_PushQueryB();
 // 	HWSERIAL_PushQueryC();

 // 	// Process incoming responses
 // 	if ( HWSerialA.available() ) {
 // 		HWSERIAL_ReadQueryResponseA();
 // 	}
 // 	if ( HWSerialB.available() ) {
 // 		HWSERIAL_ReadQueryResponseB();
 // 	}
 // 	if ( HWSerialC.available() ) {
 // 		HWSERIAL_ReadQueryResponseC();
 // 	}
 // }


 // void AmplifierClass::HWSERIAL_PushAllAndWaitForResponses() {

 // 	// Push queries out
 // 	HWSERIAL_PushQueryA();
 // 	delay( 300 );


 // 	// Wait for responses before proceeding
 // 	while ( HWSerial.isAwaitingResponseA ) {
 // 		Serial.println( "WhileA " );
 // 		if ( HWSerialA.available() ) {
 // 			Serial.println( "AvailA" );
 // 			HWSERIAL_ReadQueryResponseA();
 // 		} else if ( !HWSerialA.available() ) {
 // 			Serial.println( "!AvailA" );
 // 		}
 // 	}


 // 	HWSERIAL_PushQueryB();
 // 	delay( 300 );

 // 	while ( HWSerial.isAwaitingResponseB ) {
 // 		Serial.println( "WhileB " );
 // 		if ( HWSerialB.available() ) {
 // 			Serial.println( "AvailB" );
 // 			HWSERIAL_ReadQueryResponseB();
 // 		} else if ( !HWSerialB.available() ) {
 // 			Serial.println( "!AvailB" );
 // 		}
 // 	}
 // 	delay( 500 );

 // 	HWSERIAL_PushQueryC();
 // 	delay( 300 );


 // 	while ( HWSerial.isAwaitingResponseC ) {
 // 		Serial.println( "WhileC " );
 // 		if ( HWSerialC.available() ) {
 // 			Serial.println( "AvailC" );
 // 			HWSERIAL_ReadQueryResponseC();
 // 		} else if ( !HWSerialC.available() ) {
 // 			Serial.println( "!AvailC" );
 // 		}
 // 	}
 // 	delay( 500 );
 // 	// }
 // 	Serial.println( "HWSERIAL_PushAllAndWaitForResponse complete" );
 // }


 // /*******************************************
 //  *  HWSerial_EnqueueQuery  - Add to queue  *
 //  *******************************************/

 // /**
 //  * @brief Add a new HWSerial query the queue for amplifier A
 //  * @param cmd ASCII command string to send to A
 //  */
 // void AmplifierClass::HWSERIAL_EnqueueQueryA( const String& cmd ) {
 // 	HWSerial.Query.queueA.push( cmd );
 // 	Serial.println( "EnqueueA" );
 // }

 // /**
 //  * @brief Add a new HWSerial query the queue for amplifier B
 //  * @param cmd ASCII command string to send to B
 //  */
 // void AmplifierClass::HWSERIAL_EnqueueQueryB( const String& cmd ) {
 // 	HWSerial.Query.queueB.push( cmd );
 // 	Serial.println( "EnqueueB" );
 // }

 // /**
 //  * @brief Add a new HWSerial query the queue for amplifier C
 //  * @param cmd ASCII command string to send to C
 //  */
 // void AmplifierClass::HWSERIAL_EnqueueQueryC( const String& cmd ) {
 // 	HWSerial.Query.queueC.push( cmd );
 // 	Serial.println( "EnqueueC" );
 // }


 // /************************************************
 //  *  HWSerial_ProcessQuery - Process next query  *
 //  ***********************************************/

 // /**
 //   * @brief Process the next query in queue A
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_PushQueryA() {

 // 	// Check if there isn't already a request in waiting, and the queue isn't empty
 // 	if ( !HWSerial.isAwaitingResponseA && !HWSerial.Query.queueA.empty() ) {
 // 		Serial.println( "PushA" );
 // 		// Extract first query
 // 		HWSerial.Query.outgoingNextA = HWSerial.Query.queueA.front();
 // 		HWSerial.Query.queueA.pop();

 // 		// Update
 // 		HWSerial.Query.responseA	 = "";
 // 		HWSerial.isAwaitingResponseA = true;

 // 		// Send query
 // 		HWSerialA.clear();
 // 		HWSerialA.print( HWSerial.Query.outgoingNextA );
 // 		// HWSerialA.flush();
 // 	}
 // }

 // /**
 //   * @brief Process the next query in queue B
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_PushQueryB() {

 // 	// Check if there isn't already a request in waiting, and the queue isn't empty
 // 	if ( !HWSerial.isAwaitingResponseB && !HWSerial.Query.queueB.empty() ) {
 // 		Serial.println( "PushB" );
 // 		// Extract first query
 // 		HWSerial.Query.outgoingNextB = HWSerial.Query.queueB.front();
 // 		HWSerial.Query.queueB.pop();

 // 		// Update
 // 		HWSerial.Query.responseB	 = "";
 // 		HWSerial.isAwaitingResponseB = true;

 // 		// Send query
 // 		// HWSerialB.clear();
 // 		HWSerialB.print( HWSerial.Query.outgoingNextB );
 // 		// HWSerialB.flush();
 // 	}
 // }

 // /**
 //   * @brief Process the next query in queue C
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_PushQueryC() {


 // 	// Check if there isn't already a request in waiting, and the queue isn't empty
 // 	if ( !HWSerial.isAwaitingResponseC && !HWSerial.Query.queueC.empty() ) {
 // 		Serial.println( "PushC" );

 // 		// Extract first query
 // 		HWSerial.Query.outgoingNextC = HWSerial.Query.queueC.front();
 // 		HWSerial.Query.queueC.pop();

 // 		// Update
 // 		HWSerial.Query.responseC	 = "";
 // 		HWSerial.isAwaitingResponseC = true;

 // 		// Send query
 // 		HWSerialC.clear();
 // 		HWSerialC.print( HWSerial.Query.outgoingNextC );
 // 		// HWSerialC.flush();
 // 	}
 // }



 // /*****************************************************************
 //  *  HWSerial_ReadQueryResponse - checks for and reads responses  *
 //  *****************************************************************/

 // /**
 //   * @brief Read the response for amplifier A
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ReadQueryResponseA() {


 // 	while ( HWSerialA.available() > 0 ) {

 // 		Serial.print( "ReadA " );

 // 		// Read character
 // 		char incomingChar = ( char )HWSerialA.read();

 // 		// Look for line return
 // 		if ( incomingChar == '\r' || incomingChar == '\n' ) {

 // 			// Parse string
 // 			HWSERIAL_ParseResponseA();
 // 			HWSerialA.clear();
 // 		} else {

 // 			// Build string
 // 			HWSerial.Query.responseA += incomingChar;
 // 		}
 // 	}
 // }

 // /**
 //   * @brief Read the response for amplifier B
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ReadQueryResponseB() {


 // 	while ( HWSerialB.available() > 0 ) {

 // 		Serial.print( "ReadB " );
 // 		// Read character
 // 		char incomingChar = ( char )HWSerialB.read();

 // 		// Look for line return
 // 		if ( incomingChar == '\r' || incomingChar == '\n' ) {

 // 			// Parse string
 // 			HWSERIAL_ParseResponseB();
 // 			HWSerialB.clear();

 // 		} else {

 // 			// Build string
 // 			HWSerial.Query.responseB += incomingChar;
 // 		}
 // 	}
 // }

 // /**
 //   * @brief Read the response for amplifier C
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ReadQueryResponseC() {

 // 	while ( HWSerialC.available() > 0 ) {
 // 		Serial.print( "ReadC " );
 // 		// Read character
 // 		char incomingChar = ( char )HWSerialC.read();

 // 		// Look for line return
 // 		if ( incomingChar == '\r' || incomingChar == '\n' ) {

 // 			// Parse string
 // 			HWSERIAL_ParseResponseC();
 // 			HWSerialC.clear();
 // 		} else {

 // 			// Build string
 // 			HWSerial.Query.responseC += incomingChar;
 // 		}
 // 	}
 // }



 // /****************************
 //  *  HWSerial_ParseResponse  *
 //  ****************************/

 // /**
 //   * @brief Parse serialA response and save values
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ParseResponseA() {

 // 	Serial.println( "ParseA " );
 // 	// Remove "> " if it exists
 // 	if ( HWSerial.Query.responseA.startsWith( "v " ) ) {
 // 		HWSerial.Query.responseA.remove( 0, 2 );
 // 	}

 // 	// Extract response
 // 	String response = HWSerial.Query.responseA;
 // 	Serial.println( "RespA: " );
 // 	Serial.print( HWSerial.Query.outgoingNextA );
 // 	Serial.print( " = " );
 // 	Serial.print( response );
 // 	Serial.println( " " );

 // 	// GetBaud
 // 	if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getBaud ) {
 // 		Serial.print( "GetBaudA" );
 // 		HWSerial.Connection.baudRateA = response.toInt();
 // 		Serial.print( "  value = " );
 // 		Serial.println( response.toInt() );
 // 	}
 // 	// SetBaud
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.setBaud115237 ) {
 // 		Serial.print( "SetBaud115237A" );
 // 		HWSerial.Connection.baudRateA = response.toInt();
 // 	}
 // 	// SetCurrentMode
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.setCurrentMode ) {
 // 		Serial.print( "SetCurrentA" );
 // 		Flags.isCurrentCommandedA = true;
 // 		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
 // 	}
 // 	// GetName
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getName ) {
 // 		HWSerial.Connection.nameStringA = response.substring( 2, HWSerial.Query.responseA.length() );
 // 		Serial.print( "GetNameA" );
 // 		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
 // 	}
 // 	// GetEncoderCount
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getEncoderCount ) {
 // 		const long cnt = strtol( HWSerial.Query.responseA.c_str(), nullptr, 10 );
 // 		Read.countA	   = static_cast<int32_t>( cnt );
 // 		Read.angleDegA = degrees( Read.countA * 2.0f * M_PI / 4096 );
 // 		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
 // 	}
 // 	// GetCurrent
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.getCurrentReading ) {
 // 		Read.currentA = response.toInt();
 // 		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
 // 	}
 // 	// Set encoder zero
 // 	else if ( HWSerial.Query.outgoingNextA == HWSerial.Ascii.setEncoderZero ) {
 // 		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
 // 	}

 // 	// Update
 // 	HWSerial.Query.outgoingNextA = "";
 // 	HWSerial.Query.responseA	 = "";
 // 	HWSerial.isAwaitingResponseA = false;
 // 	// HWSerialA.flush();
 // 	HWSerialA.clear();
 // 	Serial.println( "Completed A" );
 // }

 // /**
 //   * @brief Parse serialB response and save values
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ParseResponseB() {

 // 	// Remove "> " if it exists
 // 	if ( HWSerial.Query.responseB.startsWith( "v " ) ) {
 // 		HWSerial.Query.responseB.remove( 0, 2 );
 // 	}

 // 	// Extract response
 // 	String response = HWSerial.Query.responseB;
 // 	Serial.println( "RespB: " );
 // 	Serial.print( HWSerial.Query.outgoingNextB );
 // 	Serial.print( " = " );
 // 	Serial.print( response );
 // 	Serial.println( " " );

 // 	// GetBaud
 // 	if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getBaud ) {
 // 		Serial.print( "GetBaudB" );
 // 		HWSerial.Connection.baudRateB = response.toInt();
 // 		Serial.print( "  value = " );
 // 		Serial.println( response.toInt() );
 // 	}
 // 	// SetBaud
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.setBaud115237 ) {
 // 		Serial.print( "SetBaud115237B" );
 // 		HWSerial.Connection.baudRateB = response.toInt();
 // 	}
 // 	// SetCurrentMode
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.setCurrentMode ) {
 // 		Serial.print( "SetCurrentB" );
 // 		Flags.isCurrentCommandedB = true;
 // 		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
 // 	}
 // 	// GetName
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getName ) {
 // 		HWSerial.Connection.nameStringB = response.substring( 2, HWSerial.Query.responseB.length() );
 // 		Serial.print( "GetNameB" );
 // 		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
 // 	}
 // 	// GetEncoderCount
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getEncoderCount ) {
 // 		const long cnt = strtol( HWSerial.Query.responseB.c_str(), nullptr, 10 );
 // 		Read.countB	   = static_cast<int32_t>( cnt );
 // 		Read.angleDegB = degrees( Read.countB * 2.0f * M_PI / 4096 );
 // 		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
 // 	}
 // 	// GetCurrent
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.getCurrentReading ) {
 // 		Read.currentB = response.toInt();
 // 		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
 // 	}
 // 	// Set encoder zero
 // 	else if ( HWSerial.Query.outgoingNextB == HWSerial.Ascii.setEncoderZero ) {
 // 		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
 // 	}

 // 	// Update
 // 	HWSerial.Query.outgoingNextB = "";
 // 	HWSerial.Query.responseB	 = "";
 // 	HWSerial.isAwaitingResponseB = false;
 // 	// HWSerialB.flush();
 // 	HWSerialB.clear();
 // 	Serial.println( "Completed B" );
 // }

 // /**
 //   * @brief Parse serialC response and save values
 //   *
 //   */
 // void AmplifierClass::HWSERIAL_ParseResponseC() {

 // 	Serial.println( "Parse C" );

 // 	// Remove "> " if it exists
 // 	if ( HWSerial.Query.responseC.startsWith( "v " ) ) {
 // 		HWSerial.Query.responseC.remove( 0, 2 );
 // 	}

 // 	// Extract response
 // 	String response = HWSerial.Query.responseC;
 // 	Serial.println( "RespC: " );
 // 	Serial.print( HWSerial.Query.outgoingNextC );
 // 	Serial.print( " = " );
 // 	Serial.print( response );
 // 	Serial.println( " " );

 // 	// GetBaud
 // 	if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getBaud ) {
 // 		Serial.print( "GetBaudC" );
 // 		HWSerial.Connection.baudRateC = response.toInt();
 // 		Serial.print( "  value = " );
 // 		Serial.println( response.toInt() );
 // 	}
 // 	// SetBaud
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.setBaud115237 ) {
 // 		Serial.print( "SetBaud115237C" );
 // 		HWSerial.Connection.baudRateC = response.toInt();
 // 	}
 // 	// SetCurrentMode
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.setCurrentMode ) {
 // 		Serial.print( "SetCurrentC" );
 // 		Flags.isCurrentCommandedC = true;
 // 		// dataHandle.getData()->PrintDebug( "SetCurrentMode[A]: " + response );
 // 	}
 // 	// GetName
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getName ) {
 // 		HWSerial.Connection.nameStringC = response.substring( 2, HWSerial.Query.responseC.length() );
 // 		Serial.print( "GetNameC" );
 // 		// dataHandle.getData()->PrintDebug( "GetName[A]: " + shared->Amplifier.nameA );
 // 	}
 // 	// GetEncoderCount
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getEncoderCount ) {
 // 		const long cnt = strtol( HWSerial.Query.responseC.c_str(), nullptr, 10 );
 // 		Read.countC	   = static_cast<int32_t>( cnt );
 // 		Read.angleDegC = degrees( Read.countC * 2.0f * M_PI / 4096 );
 // 		// dataHandle.getData()->PrintDebug( "GetCount[A]: " + String( shared->Amplifier.encoderMeasuredCountA ) );
 // 	}
 // 	// GetCurrent
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.getCurrentReading ) {
 // 		Read.currentC = response.toInt();
 // 		// dataHandle.getData()->PrintDebug( "GetCurrent[A]: " + String( shared->Amplifier.currentMeasuredRawA ) );
 // 	}
 // 	// Set encoder zero
 // 	else if ( HWSerial.Query.outgoingNextC == HWSerial.Ascii.setEncoderZero ) {
 // 		// dataHandle.getData()->PrintDebug( "SetEncoderZero[A]: " + String( HWSerial.Query.responseA ) );
 // 	}

 // 	// Update
 // 	HWSerial.Query.outgoingNextC = "";
 // 	HWSerial.Query.responseC	 = "";
 // 	HWSerial.isAwaitingResponseC = false;
 // 	// HWSerialC.flush();
 // 	HWSerialC.clear();
 // 	Serial.println( "Completed C" );
 // }



 // void AmplifierClass::HWSERIAL_GetAmplifierNames() {

 // 	// Queue up commands
 // 	HWSERIAL_EnqueueQueryA( HWSerial.Ascii.getName );
 // 	HWSERIAL_EnqueueQueryB( HWSerial.Ascii.getName );

 // 	// Push queries
 // 	HWSERIAL_EnqueueQueryC( HWSerial.Ascii.getName );
 // 	HWSERIAL_PushQueryA();
 // 	HWSERIAL_PushQueryB();
 // 	HWSERIAL_PushQueryC();

 // 	delay( 100 );

 // 	// Check responses
 // 	if ( HWSerialA.available() ) {
 // 		HWSERIAL_ReadQueryResponseA();
 // 	}
 // 	if ( HWSerialB.available() ) {
 // 		HWSERIAL_ReadQueryResponseB();
 // 	}
 // 	if ( HWSerialC.available() ) {
 // 		HWSERIAL_ReadQueryResponseC();
 // 	}

 // 	delay( 100 );
 // }


 // /*  =========================================
 //  *  =========================================
 //  *
 //  *   FFFFFF  LL      AAAAA    GGGGG    SSSS
 //  *   FF      LL     AA   AA  GG   GG  SS
 //  *   FF      LL     AA   AA  GG       SS
 //  *   FFFF    LL     AAAAAAA  GG  GGG   SSSS
 //  *   FF      LL     AA   AA  GG   GG      SS
 //  *   FF      LL     AA   AA  GG   GG      SS
 //  *   FF      LLLLL  AA   AA   GGGGG    SSSS
 //  *
 //  *  =========================================
 //  *  ========================================= */

 // /**
 //  * @brief Returns the state of the safety switch
 //  *
 //  * @return true Safety switch engaged, system enabled
 //  * @return false Safety switch disengaged, system disabled
 //  */
 // bool AmplifierClass::FLAGS_GetSafetySwitchState() {

 // 	// Read state
 // 	Flags.isSafetySwitchEnabled = digitalReadFast( PIN_AMPLIFIER_SAFETY );
 // 	return Flags.isSafetySwitchEnabled;
 // }

 // /**
 //  * @brief Get the string representing the class state
 //  * @return String text of any class updates / states
 //  */
 // String AmplifierClass::FLAGS_GetStateStringA() {
 // 	return "INCOMPLETE";
 // }

 // /**
 //  * @brief Get the string representing the class state
 //  * @return String text of any class updates / states
 //  */
 // String AmplifierClass::FLAGS_GetStateStringB() {
 // 	return "INCOMPLETE";
 // }

 // /**
 //  * @brief Get the string representing the class state
 //  * @return String text of any class updates / states
 //  */
 // String AmplifierClass::FLAGS_GetStateStringC() {
 // 	return "INCOMPLETE";
 // }

 // /**
 //  * @brief Check if amplifiers are active
 //  * @return true Amplfiers running
 //  * @return false Amplifiers disabled
 //  */
 // bool AmplifierClass::FLAGS_GetAmplifierState() {

 // 	return ( Flags.isEnabled );
 // }



 // /*  ======================================
 //  *  ======================================
 //  *
 //  *   RRRRR     EEEEEE    AAAAAAA    DDDDD
 //  *   RR  RR    EE        AA   AA    DD  DD
 //  *   RR  RR    EE        AA   AA    DD  DD
 //  *   RRRRR     EEEE      AAAAAAA    DD  DD
 //  *   RR  RR    EE        AA   AA    DD  DD
 //  *   RR  RR    EE        AA   AA    DD  DD
 //  *   RR   RR   EEEEEE    AA   AA    DDDDD
 //  *
 //  *  ======================================
 //  *  ======================================*/

 // /**
 //  * @brief Read encoders using HWserial
 //  */
 // void AmplifierClass::HWSERIAL_GetEncoderValues() {

 // 	// Send serial command to read encoder
 // 	HWSERIAL_EnqueueQueryA( HWSerial.Ascii.getEncoderCount );
 // 	HWSERIAL_EnqueueQueryB( HWSerial.Ascii.getEncoderCount );
 // 	HWSERIAL_EnqueueQueryC( HWSerial.Ascii.getEncoderCount );
 // }

 // /**
 //  * @brief Read amplifier baud rate
 //  */
 // void AmplifierClass::HWSERIAL_GetAmplifierBaudRates() {

 // 	Serial.println( "Entering HWSERIAL_GetAmplifierBaudRates" );


 // 	// A
 // 	HWSERIAL_EnqueueQueryA(HWSerial.Ascii.getBaud);
 // 	delay(50);
 // 	HWSERIAL_PushQueryA();
 // 	HWSERIAL_WaitForPortA(500);           // no Serial spam, has timeout
 // 	// no HWSerialA.clear() here

 // 	// B
 // 	HWSERIAL_EnqueueQueryB(HWSerial.Ascii.getBaud);
 // 	delay(50);
 // 	HWSERIAL_PushQueryB();
 // 	HWSERIAL_WaitForPortB(500);
 // 	// no HWSerialB.clear()

 // 	// C
 // 	HWSERIAL_EnqueueQueryC(HWSerial.Ascii.getBaud);
 // 	delay(50);
 // 	HWSERIAL_PushQueryC();
 // 	HWSERIAL_WaitForPortC(500);
 // 	// no HWSerialC.clear()

 // 	Serial.print(F("Baud rates: A = "));
 // 	Serial.print(HWSerial.Connection.baudRateA);
 // 	Serial.print(F(", B = "));
 // 	Serial.print(HWSerial.Connection.baudRateB);
 // 	Serial.print(F(", C = "));
 // 	Serial.println(HWSerial.Connection.baudRateC);

 // 	Serial.println("HWSERIAL_GetAmplifierBaudRates complete");


 // 	// // Enqueue command
 // 	// HWSERIAL_EnqueueQueryA( HWSerial.Ascii.getBaud );
 // 	// delay( 100 );

 // 	// // Push queries out
 // 	// HWSERIAL_PushQueryA();
 // 	// delay( 300 );

 // 	// HWSerialA.flush();

 // 	// // Wait for responses before proceeding
 // 	// while ( HWSerial.isAwaitingResponseA ) {
 // 	// 	Serial.println( "WhileA " );
 // 	// 	if ( HWSerialA.available() ) {
 // 	// 		Serial.println( "AvailA" );
 // 	// 		HWSERIAL_ReadQueryResponseA();
 // 	// 	} else if ( !HWSerialA.available() ) {
 // 	// 		Serial.println( "!AvailA" );
 // 	// 	}
 // 	// }
 // 	// delay(300) ;



 // 	// HWSERIAL_EnqueueQueryB( HWSerial.Ascii.getBaud );
 // 	// delay( 100 );

 // 	// HWSERIAL_PushQueryB();
 // 	// delay( 300 );

 // 	// HWSerialB.flush();
 // 	// while ( HWSerial.isAwaitingResponseB ) {
 // 	// 	Serial.println( "WhileB " );
 // 	// 	if ( HWSerialB.available() ) {
 // 	// 		Serial.println( "AvailB" );
 // 	// 		HWSERIAL_ReadQueryResponseB();
 // 	// 	} else if ( !HWSerialB.available() ) {
 // 	// 		Serial.println( "!AvailB" );
 // 	// 	}
 // 	// }
 // 	// delay(300) ;



 // 	// HWSERIAL_EnqueueQueryC( HWSerial.Ascii.getBaud );
 // 	// delay( 100 );
 // 	// HWSERIAL_PushQueryC();
 // 	// delay( 300 );
 // 	// HWSerialC.flush();

 // 	// while ( HWSerial.isAwaitingResponseC ) {
 // 	// 	Serial.println( "WhileC " );
 // 	// 	if ( HWSerialC.available() ) {
 // 	// 		Serial.println( "AvailC" );
 // 	// 		HWSERIAL_ReadQueryResponseC();
 // 	// 	} else if ( !HWSerialC.available() ) {
 // 	// 		Serial.println( "!AvailC" );
 // 	// 	}
 // 	// }
 // 	// delay(300) ;
 // 	// // HWSerialC.clear();


 // 	// // Wait for responses
 // 	// // HWSERIAL_PushAllAndWaitForResponses();

 // 	// // Print baud rates
 // 	// Serial.print( F( "Baud rates: A = " ) );
 // 	// Serial.print( HWSerial.Connection.baudRateA );
 // 	// Serial.print( F( ", B = " ) );
 // 	// Serial.print( HWSerial.Connection.baudRateB );
 // 	// Serial.print( F( ", C = " ) );
 // 	// Serial.print( HWSerial.Connection.baudRateC );
 // 	// Serial.println();

 // 	Serial.println( "HWSERIAL_GetAmplifierBaudRates complete" );
 // }


 // /**
 //  * @brief Gets the encoder count from motor A
 //  * @return int32_t encoder count in ticks
 //  */
 // int32_t AmplifierClass::READ_GetCountA() {

 // 	return Read.countA;
 // }

 // /**
 //  * @brief Gets the encoder count from motor B
 //  * @return int32_t encoder count in ticks
 //  */
 // int32_t AmplifierClass::READ_GetCountB() {

 // 	return Read.countB;
 // }

 // /**
 //  * @brief Gets the encoder count from motor C
 //  * @return int32_t encoder count in ticks
 //  */
 // int32_t AmplifierClass::READ_GetCountC() {

 // 	return Read.countC;
 // }



 // /**
 //  * @brief Gets the encoder count from motor A
 //  * @return float encoder count in degrees
 //  */
 // float AmplifierClass::READ_GetAngleDegA() {

 // 	return Read.angleDegA;
 // }

 // /**
 //  * @brief Gets the encoder count from motor B
 //  * @return float encoder count in degrees
 //  */
 // float AmplifierClass::READ_GetAngleDegB() {

 // 	return Read.angleDegB;
 // }

 // /**
 //  * @brief Gets the encoder count from motor C
 //  * @return float encoder count in degrees
 //  */
 // float AmplifierClass::READ_GetAngleDegC() {

 // 	return Read.angleDegC;
 // }
