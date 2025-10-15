#pragma once

// === STANDARD LIBRARY HEADERS ===================================================================
#include <Arduino.h>
#include <memory>
#include <unordered_map>

// === PROJECT HEADERS ============================================================================
#include "SharedMemoryDataTypes.h"



// === CLASS DEFINITION ===========================================================================
class SharedMemoryManager {

	// Constructor
	public:
	SharedMemoryManager();

	// Functions
	public:
	std::shared_ptr<ManagedSystemDataClass> GetData();								 // Return pointer to data
	void									ResetActionFlag( bool& flagToReset );	 // Reset an action flag

	private:
	std::shared_ptr<ManagedSystemDataClass> data;
};

// Define global instance
extern SharedMemoryManager SYSTEM_GLOBAL;	 // Declaration only