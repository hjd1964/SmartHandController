// -------------------------------------------------------------------------------------------------
// Pin map for ESP32
#pragma once

#if defined(ESP32)
  #define PINMAP_STR "ESP32"

  // workaround for ESP32 crashing on EEPROM commit
  #define DISABLE_EEPROM_COMMIT_ON

  #if KEYPAD_JOYSTICK_ANALOG == JS1
    #define B_PIN0 25   // Shift
    #define B_PIN1 33   // N
    #define B_PIN2 33   // S
    #define B_PIN3 32   // E
    #define B_PIN4 32   // W
    #define B_PIN5 34   // F
    #define B_PIN6 35   // f
  #else
    #define B_PIN0 33   // Shift
    #define B_PIN1 25   // N
    #define B_PIN2 32   // S
    #define B_PIN3 36   // E
    #define B_PIN4 34   // W
    #define B_PIN5 39   // F
    #define B_PIN6 35   // f
  #endif

  #define ST4RAw 23   // ST4 (OnStep) RA- West,  send data to OnStep
  #define ST4DEs 14   // ST4 (OnStep) DE- South, clock input to ISR
  #define ST4DEn 27   // ST4 (OnStep) DE+ North, recv data from OnStep
  #define ST4RAe 26   // ST4 (OnStep) RA+ East,  always 12.5 Hz square wave on this pin

  #if ST4_INTERFACE == ON
    #define ST4AuxRAw 19 // ST4 (Aux) RA- West
    #define ST4AuxDEs 18 // ST4 (Aux) DE- South
    #define ST4AuxDEn 17 // ST4 (Aux) DE+ North
    #define ST4AuxRAe 5  // ST4 (Aux) RA+ East
  #endif

  #define UTILITY_LIGHT_PIN  13    // GPIO13 used as PWM output to drive utility lamp LED

  #define DISPLAY_RESET_PIN  16    // GPIO16 used to reset some displays, i.e. the SSD1309
#endif
