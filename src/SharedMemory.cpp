#include "SharedMemory.h"

SharedMemoryManager SYSTEM_GLOBAL;  // definition

SharedMemoryManager::SharedMemoryManager()
: data(std::make_shared<ManagedSystemDataClass>()) {}

std::shared_ptr<ManagedSystemDataClass> SharedMemoryManager::GetData() {
    return data;
}

void SharedMemoryManager::ResetActionFlag(bool& flagToReset){

    flagToReset = false; 

}