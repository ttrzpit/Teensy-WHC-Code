#include "TaskSelectorClass.h"


void TaskSelectorClass::Begin() {

	// Generate new random seed
	randomSeed( analogRead( A14 ) );	   // simple entropy
    

}


/**
 * Update
 *      switch active task
 *          DiscriminationTaskUpdate
 *              Switch (discrimination task)
 */