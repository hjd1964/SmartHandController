// -------------------------------------------------------------------------------------------------
// Pin map for Teensy 3.0, 3.1, 3.2, 3.5, 3.6, 4.0
#pragma once

#if defined(__TEENSYDUINO__)
  #define PINMAP_STR "TEENSY"

  #if KEYPAD_JOYSTICK_ANALOG == JS1
    #ifndef B_PIN0
    #define B_PIN0             21  // Shift
    #endif
    #ifndef B_PIN1
    #define B_PIN1             15  // N
    #endif
    #ifndef B_PIN2
    #define B_PIN2             15  // S
    #endif
    #ifndef B_PIN3
    #define B_PIN3             17  // E
    #endif
    #ifndef B_PIN4
    #define B_PIN4             17  // W
    #endif
    #ifndef B_PIN5
    #define B_PIN5             22  // F
    #endif
    #ifndef B_PIN6
    #define B_PIN6             23  // f
    #endif
  #else
    #ifndef B_PIN0
    #define B_PIN0             21  // Shift
    #endif
    #ifndef B_PIN1
    #define B_PIN1             15  // N
    #endif
    #ifndef B_PIN2
    #define B_PIN2             16  // S
    #endif
    #ifndef B_PIN3
    #define B_PIN3             17  // E
    #endif
    #ifndef B_PIN4
    #define B_PIN4             20  // W
    #endif
    #ifndef B_PIN5
    #define B_PIN5             22  // F
    #endif
    #ifndef B_PIN6
    #define B_PIN6             23  // f
    #endif
  #endif

  #define ST4_W_PIN             2  // ST4 (OnStep) RA- West,  send data to OnStep
  #define ST4_S_PIN             3  // ST4 (OnStep) DE- South, clock input to ISR
  #define ST4_N_PIN             4  // ST4 (OnStep) DE+ North, recv data from OnStep
  #define ST4_E_PIN             5  // ST4 (OnStep) RA+ East,  always 25 Hz square wave on this pin

  #if ST4_AUX_INTERFACE == ON
    #define ST4_AUX_W_PIN       6  // ST4 (Aux) RA- West
    #define ST4_AUX_S_PIN       7  // ST4 (Aux) DE- South
    #define ST4_AUX_N_PIN       8  // ST4 (Aux) DE+ North
    #define ST4_AUX_E_PIN       9  // ST4 (Aux) RA+ East
  #endif

  #define UTILITY_LIGHT_PIN    10  // Pin10 used as PWM output to drive utility lamp LED

  #define DISPLAY_RESET_PIN    14  // Pin14 used to reset some displays, i.e. the SSD1309

  #ifndef DISPLAY_DC_PIN
    #define DISPLAY_DC_PIN     -1  // SPI display data/clock pin
  #endif
  #ifndef DISPLAY_SS_PIN
    #define DISPLAY_SS_PIN     -1  // SPI display slave select pin
  #endif
  #ifndef DISPLAY_MOSI_PIN
    #define DISPLAY_MOSI_PIN   -1  // SPI display master out slave in pin (ignored for hardware SPI connections)
  #endif
  #ifndef DISPLAY_SCK_PIN
    #define DISPLAY_SCK_PIN    -1  // SPI display clock pin for (ignored for hardware SPI connections)
  #endif
#endif
