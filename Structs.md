ManagedData (System)
    Drive
        Flags
        Mapping
        PWM
        Tension
    Interface
        GamepadInput
        HWSerial
            Connection
            Packets
        KeyboardInput
        Mapping
        SWSerial
    Sensors
        PlatformEncoders
        MotorCurrent
            CurrentLimits
        MotorEncoders
            EncoderLimits
    Tasks
        DiscriminationTask
            CardinalDirections
            AllDirections
    System
        ActionQueue
        Drive
        Interface
        Sensors


Amplifier
    HWSerial
        ASCII
        Packets
        Properties
    Command
        Flags
        Mapping
        Limits
        PWM
            TensionPWM
    Sensors


    
    
    


/**
 * STRUCTS IN SOFTWARE
 * 
 * HWSerialStruct
 *   AsciiStruct                  - HWSerial commands
 *   PacketsStruct                - HWserial packets
 *   PropertiesStruct             - HWSerial properties (baud, amp name)
 * 
 * Command
 *   FlagsStruct                  - State flags
 *   LimitsStruct                 - Hardware limits
 *   MappingStruct                - Motor mapping values
 *   PWMStruct                    - PWM outputs
 * 
 * Sensors
 * 
 */


/**
 *
 * Discrimination Task
 *   CardinalPoolStruct
 *   CardinalResponsesStruct
 *   trialNumber
 *   delayTime 
 *   trialTime