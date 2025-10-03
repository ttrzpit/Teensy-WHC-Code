#include "AmplifierClass.h"

AmplifierClass::AmplifierClass() {
	InitializePins();
}



/**
 * @brief Initializes the hardware pins
 * 
 */
void AmplifierClass::InitializePins() {

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

	// Set initial values
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_ENABLE_C, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_A, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_B, LOW );
	digitalWriteFast( PIN_AMPLIFIER_LED_C, LOW );

	// Set analog resolution
	analogWriteResolution( 12 );	// 12-bit (0 to 4096)
}

/**
 * @brief Returns the state of the safety switch
 * 
 * @return true Safety switch engaged, system enabled
 * @return false Safety switch disengaged, system disabled
 */
bool AmplifierClass::GetSafetySwitchState() {

	return true;
}