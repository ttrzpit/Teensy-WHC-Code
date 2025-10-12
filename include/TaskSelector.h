#pragma once

// Pre-built libraries
#include <Arduino.h>

// Forward declarations
class AmplifierClass;
class ArmEncoderClass;
class GamepadClass;


class TaskSelectorClass {

	/*****************
	*  Constructors  *
	******************/
	public:
	explicit TaskSelectorClass( AmplifierClass& amp, ArmEncoderClass& enc, GamepadClass& gpad );	 // Constructor
	private:
	AmplifierClass& Amplifier;	  // Stored reference to amplifier
	ArmEncoderClass&	Encoders;	  // Stored reference to arm encoders
	GamepadClass&	Gamepad;	  // Stored reference to gamepad

	/**************
	*  Accessors  *
	***************/
	public:
	void Update();	  // Run every loop
	void Begin();	  // Initialize class

	/**********
	*  Tasks  *
	***********/
	public:
	enum activeTaskEnum { NONE, DISCRIMINATION, GUIDANCE };
	activeTaskEnum activeTask = NONE;

	private:
	String activeTaskName = "";		  // Name of active task
	bool   isTaskStarted  = false;	  // State of active task

	/************
	*  Logging  *
	*************/
	elapsedMillis loggingRuntimeMillis = 0;	   // Runtime for logging timer

	/************************
	*  Discrimination Task  *
	*************************/
	public:
	void DT_StartTask();	// Start the discrimination task
	private:
	enum discriminationTaskStateEnum { IDLE, WAIT_RANDOM, TASK_RUNNING, TASK_COMPLETE };
	discriminationTaskStateEnum DT_state				 = IDLE;	// Current DT state
	elapsedMillis				DT_elapsedTaskTimeMillis = 0;		// Elapsed task time
	elapsedMillis				DT_elapsedCueTimeMillis	 = 0;		// Elapsed cue time
	inline static int8_t		randomDirection			 = 0;		// Random chosen direction

	inline static String cardinalStringArray[4] = { "RIGHT", "UP", "DOWN", "LEFT" };	// Array of string names
};