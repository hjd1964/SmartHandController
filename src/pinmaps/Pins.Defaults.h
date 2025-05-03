// -------------------------------------------------------------------------------------------------
// Null pin map, assigns OFF to all values not already assigned
#pragma once

// default settings for I2C
#ifndef I2C_SDA_PIN
#define I2C_SDA_PIN                 OFF
#endif

#ifndef I2C_SCL_PIN
#define I2C_SCL_PIN                 OFF
#endif

#ifdef ESP32
  #if (defined(I2C_SCL_PIN) && I2C_SCL_PIN != OFF) && (defined(I2C_SDA_PIN) && I2C_SDA_PIN != OFF)
    #define WIRE_INIT() HAL_WIRE.begin(I2C_SDA_PIN, I2C_SCL_PIN); HAL_WIRE_SET_CLOCK()
  #else
    #define WIRE_INIT() HAL_WIRE_SET_CLOCK()
  #endif
#else
  #if (defined(I2C_SCL_PIN) && I2C_SCL_PIN != OFF) && (defined(I2C_SDA_PIN) && I2C_SDA_PIN != OFF)
    #define WIRE_INIT() HAL_WIRE.setSDA(I2C_SDA_PIN); HAL_WIRE.setSCL(I2C_SCL_PIN); HAL_WIRE_SET_CLOCK()
  #else
    #define WIRE_INIT() HAL_WIRE_SET_CLOCK()
  #endif
#endif

// misc
#ifndef UTILITY_LIGHT_PIN
#define UTILITY_LIGHT_PIN           OFF  // GPIO13 used as PWM output to drive utility lamp LED
#endif

#ifndef DISPLAY_RESET_PIN
#define DISPLAY_RESET_PIN           OFF  // GPIO16 used to reset some displays, i.e. the SSD1309
#endif

#ifndef DISPLAY_DC_PIN
#define DISPLAY_DC_PIN              OFF  // SPI display data/clock pin
#endif
#ifndef DISPLAY_SS_PIN
#define DISPLAY_SS_PIN              OFF  // SPI display slave select pin
#endif
#ifndef DISPLAY_MOSI_PIN
#define DISPLAY_MOSI_PIN            OFF  // SPI display master out slave in pin (ignored for hardware SPI connections)
#endif
#ifndef DISPLAY_SCK_PIN
#define DISPLAY_SCK_PIN             OFF  // SPI display clock pin for (ignored for hardware SPI connections)
#endif

#ifndef KEYPAD_JOYSTICK_HYSTERESIS
#define KEYPAD_JOYSTICK_HYSTERESIS  200  // hysteresis for any joystick (in ADC counts) that may be present
#endif

// default button pad pins
#ifndef B_PIN0
#define B_PIN0                      OFF  // Shift
#endif
#ifndef B_PIN1
#define B_PIN1                      OFF  // N
#endif
#ifndef B_PIN2
#define B_PIN2                      OFF  // S
#endif
#ifndef B_PIN3
#define B_PIN3                      OFF  // E
#endif
#ifndef B_PIN4
#define B_PIN4                      OFF  // W
#endif
#ifndef B_PIN5
#define B_PIN5                      OFF  // F
#endif
#ifndef B_PIN6
#define B_PIN6                      OFF  // f
#endif

// default button pad pin state
#ifndef B_PIN0_ACTIVE_STATE
#define B_PIN0_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN1_ACTIVE_STATE
#define B_PIN1_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN2_ACTIVE_STATE
#define B_PIN2_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN3_ACTIVE_STATE
#define B_PIN3_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN4_ACTIVE_STATE
#define B_PIN4_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN5_ACTIVE_STATE
#define B_PIN5_ACTIVE_STATE         LOW
#endif
#ifndef B_PIN6_ACTIVE_STATE
#define B_PIN6_ACTIVE_STATE         LOW
#endif

// default button pad pin mode
#ifndef B_PIN0_INPUT_MODE
#define B_PIN0_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN1_INPUT_MODE
#define B_PIN1_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN2_INPUT_MODE
#define B_PIN2_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN3_INPUT_MODE
#define B_PIN3_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN4_INPUT_MODE
#define B_PIN4_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN5_INPUT_MODE
#define B_PIN5_INPUT_MODE           INPUT_PULLUP
#endif
#ifndef B_PIN6_INPUT_MODE
#define B_PIN6_INPUT_MODE           INPUT_PULLUP
#endif

// default button debounce in milliseconds
#ifndef BUTTON_DEBOUNCE_MS
#define BUTTON_DEBOUNCE_MS          30
#endif

// default ST4 port (to OnStep)
#ifndef ST4_W_PIN
#define ST4_W_PIN                   OFF  // ST4 (OnStep) RA- West,  send data to OnStep
#endif
#ifndef ST4_S_PIN
#define ST4_S_PIN                   OFF  // ST4 (OnStep) DE- South, clock input to ISR
#endif
#ifndef ST4_N_PIN
#define ST4_N_PIN                   OFF  // ST4 (OnStep) DE+ North, recv data from OnStep
#endif
#ifndef ST4_E_PIN
#define ST4_E_PIN                   OFF  // ST4 (OnStep) RA+ East,  always 12.5 Hz square wave on this pin
#endif

// default ST4 additional port
#ifndef ST4_AUX_W_PIN
#define ST4_AUX_W_PIN               OFF  // ST4 (Aux) RA- West
#endif
#ifndef ST4_AUX_S_PIN
#define ST4_AUX_S_PIN               OFF  // ST4 (Aux) DE- South
#endif
#ifndef ST4_AUX_N_PIN
#define ST4_AUX_N_PIN               OFF  // ST4 (Aux) DE+ North
#endif
#ifndef ST4_AUX_E_PIN
#define ST4_AUX_E_PIN               OFF  // ST4 (Aux) RA+ East
#endif
