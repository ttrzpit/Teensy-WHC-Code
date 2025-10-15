# NURing Device Controller

## Operating Instructions

### Hardware Setup
Note: If the amplifiers have been turned off or reset, and the 
motors have been moved and are not in the home detent position,
step 4 below will need to be repeated
1. Set initial power states
   - Turn Teensy power (5V) switch off
   - Turn Amplifier Logic power (12V) switch off
   - Turn Motor power (24V) switch off
   - Reset E-Stop (to enable)
   - Reset 24V fuse (to enable)
2. Turn on system main power strip
3. Align motors to starting detent position

### Serial Setup
1. Make sure USB cable is connected between Teensy and PC
2. Turn Teensy power (5V) switch on
3. (IF NEEDED) Upload new code

### Operation
1. Turn on Teensy power (5V) switch (if not already on)



## Keyboard commands
`     Set system state = IDLE
1     Start / stop range of motion measurement
2     Start / stop current limit measurement
3     
4     
5     
tNN   Set tension to NN% (and enable tension and output)
T     Toggle tension enable / disable
s     Print system state block
S     Toggle scrolling system state
z     Zero arm platform encoders
Z     Zero motor encoders
e     Enable motor output

