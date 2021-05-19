// -------------------------------------------------------------------------------------------------
// Pin map for Teensy 3.0, 3.1, 3.2, 3.5, 3.6, 4.0
#pragma once

#if defined(__TEENSYDUINO__)
  #define PINMAP_STR "TEENSY"

  #if KEYPAD_JOYSTICK_ANALOG == JS1
    #define B_PIN0 21   // Shift
    #define B_PIN1 15   // N
    #define B_PIN2 15   // S
    #define B_PIN3 17   // E
    #define B_PIN4 17   // W
    #define B_PIN5 22   // F
    #define B_PIN6 23   // f
  #else
    #define B_PIN0 21   // Shift
    #define B_PIN1 15   // N
    #define B_PIN2 16   // S
    #define B_PIN3 17   // E
    #define B_PIN4 20   // W
    #define B_PIN5 22   // F
    #define B_PIN6 23   // f
  #endif

  #define ST4RAw 2    // ST4 (OnStep) RA- West,  send data to OnStep
  #define ST4DEs 3    // ST4 (OnStep) DE- South, clock input to ISR
  #define ST4DEn 4    // ST4 (OnStep) DE+ North, recv data from OnStep
  #define ST4RAe 5    // ST4 (OnStep) RA+ East,  always 25 Hz square wave on this pin

  #if ST4_INTERFACE == ON
    #define ST4AuxRAw 6 // ST4 (Aux) RA- West
    #define ST4AuxDEs 7 // ST4 (Aux) DE- South
    #define ST4AuxDEn 8 // ST4 (Aux) DE+ North
    #define ST4AuxRAe 9 // ST4 (Aux) RA+ East
  #endif

  #define UTILITY_LIGHT_PIN  10    // Pin10 used as PWM output to drive utility lamp LED

  #define DISPLAY_RESET_PIN  14    // Pin14 used to reset some displays, i.e. the SSD1309
#endif
