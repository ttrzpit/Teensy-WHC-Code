#pragma once

#include <Arduino.h>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

struct DiscriminationTaskResultRuntimeStruct {

	size_t	 trialNumber	   = 0;		   // Iterating trial number
	uint32_t promptDelayTimeMs = 0;		   // Randomized delay time
	int8_t	 promptVal		   = -1;	   // Value of new directional prompt
	String	 promptString	   = "";	   // String of new directional  prompt
	int8_t	 responseVal	   = -1;	   // Value of participant response to prompt
	String	 responseString	   = "";	   // String of participant response to prompt
	uint32_t responseTimeMs	   = 0.0f;	   // Participant task completion time [ms]
	bool	 isResponseCorrect = false;	   // Flag if response correct
};

class CardinalDirectionsRuntimeClass {

	// Functions
	public:
	void Loop();						   // Called every loop
	void PrintCardinalDirectionTable();	   // Print table of cardinal directions and user responses

	private:
	void InitializeRandomCardinalPool( uint16_t nRepetitions );	   // Initialize random pool of cardinal directions
	bool isRandomPoolInitialized = false;						   // Flag if random pool initialized

	// Local variables
	private:
	size_t											   currentTrialNumber = 0;	  // Current trial being run
	std::vector<int8_t>								   randomPool;				  // Pool of random cardinal directions
	std::vector<DiscriminationTaskResultRuntimeStruct> userResponses;			  // Vector of user responses

	// Timing
	private:
	uint32_t			  timeTrialStartMs	 = 0;																																								// Time that the trial started (in Teensy milliseconds)
	uint32_t			  timeDelayStartMs	 = 0;																																								// Time that the delay started (in Teensy milliseconds)
	uint32_t			  timePromptStartMs	 = 0;																																								// Time that the prompt was presented (in Teensy milliseconds)
	uint32_t			  timeTotalTask		 = 0;																																								// Total time for the task
	const uint32_t		  MINIMUM_TIME_DELAY = 3000;																																							// Ms to wait between trials
	std::vector<uint32_t> timeDelayPoolMs	 = { MINIMUM_TIME_DELAY, MINIMUM_TIME_DELAY + 500, MINIMUM_TIME_DELAY + 1000, MINIMUM_TIME_DELAY + 1500, MINIMUM_TIME_DELAY + 2000, MINIMUM_TIME_DELAY + 2500 };	// Pool of delay times
};


class OctantDirectionsRuntimeClass {

	// Functions
	public:
	void Loop();						 // Called every loop
	void PrintOctantDirectionTable();	 // Print table of cardinal directions and user responses

	private:
	void InitializeRandomOctantPool( uint16_t nRepetitions );	 // Initialize random pool of cardinal directions
	bool isRandomPoolInitialized = false;						 // Flag if random pool initialized

	// Local variables
	private:
	size_t											   currentTrialNumber = 0;	  // Current trial being run
	std::vector<int8_t>								   randomPool;				  // Pool of random cardinal directions
	std::vector<DiscriminationTaskResultRuntimeStruct> userResponses;			  // Vector of user responses

	// Timing
	private:
	uint32_t			  timeTrialStartMs	 = 0;																																								// Time that the trial started (in Teensy milliseconds)
	uint32_t			  timeDelayStartMs	 = 0;																																								// Time that the delay started (in Teensy milliseconds)
	uint32_t			  timePromptStartMs	 = 0;																																								// Time that the prompt was presented (in Teensy milliseconds)
	uint32_t			  timeTotalTask		 = 0;																																								// Total time for the task
	const uint32_t		  MINIMUM_TIME_DELAY = 3000;																																							// Ms to wait between trials
	std::vector<uint32_t> timeDelayPoolMs	 = { MINIMUM_TIME_DELAY, MINIMUM_TIME_DELAY + 500, MINIMUM_TIME_DELAY + 1000, MINIMUM_TIME_DELAY + 1500, MINIMUM_TIME_DELAY + 2000, MINIMUM_TIME_DELAY + 2500 };	// Pool of delay times
};


class DiscriminationTaskRuntimeClass {

	public:
	CardinalDirectionsRuntimeClass CardinalDirectionsRuntime;
	OctantDirectionsRuntimeClass   OctantDirectionsRuntime;
};



class TaskManagerRuntimeClass {

	public:
	DiscriminationTaskRuntimeClass DiscriminationTaskRuntime;


	private:
};
