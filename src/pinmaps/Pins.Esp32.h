// -------------------------------------------------------------------------------------------------
// Pin map for ESP32
#pragma once

#if defined(ESP32)
  #define PINMAP_STR "ESP32"

  #if KEYPAD_JOYSTICK_ANALOG == JS1
    #ifndef B_PIN0
    #define B_PIN0             25  // Shift
    #endif
    #ifndef B_PIN1
    #define B_PIN1             33  // N
    #endif
    #ifndef B_PIN2
    #define B_PIN2             33  // S
    #endif
    #ifndef B_PIN3
    #define B_PIN3             32  // E
    #endif
    #ifndef B_PIN4
    #define B_PIN4             32  // W
    #endif
    #ifndef B_PIN5
    #define B_PIN5             34  // F
    #endif
    #ifndef B_PIN6
    #define B_PIN6             35  // f
    #endif
  #else
    #ifndef B_PIN0
    #define B_PIN0             33  // Shift
    #endif
    #ifndef B_PIN1
    #define B_PIN1             25  // N
    #endif
    #ifndef B_PIN2
    #define B_PIN2             32  // S
    #endif
    #ifndef B_PIN3
    #define B_PIN3             36  // E
    #endif
    #ifndef B_PIN4
    #define B_PIN4             34  // W
    #endif
    #ifndef B_PIN5
    #define B_PIN5             39  // F
    #endif
    #ifndef B_PIN6
    #define B_PIN6             35  // f
    #endif
  #endif

  #define ST4_W_PIN            23  // ST4 (OnStep) RA- West,  send data to OnStep
  #define ST4_S_PIN            14  // ST4 (OnStep) DE- South, clock input to ISR
  #define ST4_N_PIN            27  // ST4 (OnStep) DE+ North, recv data from OnStep
  #define ST4_E_PIN            26  // ST4 (OnStep) RA+ East,  always 12.5 Hz square wave on this pin

  #if ST4_AUX_INTERFACE == ON
    #define ST4_AUX_W_PIN      19  // ST4 (Aux) RA- West
    #define ST4_AUX_S_PIN      18  // ST4 (Aux) DE- South
    #define ST4_AUX_N_PIN      17  // ST4 (Aux) DE+ North
    #define ST4_AUX_E_PIN      5   // ST4 (Aux) RA+ East
  #endif

  #define UTILITY_LIGHT_PIN    13  // GPIO13 used as PWM output to drive utility lamp LED

  #define DISPLAY_RESET_PIN    16  // GPIO16 used to reset some displays, i.e. the SSD1309

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
