#pragma once

#include <Arduino.h>

#define SERIAL_ST4_SLAVE ON
// activate ST4 port serial interface
#define SERIAL_ST4
#define SERIAL_ST4_SLAVE_PRESENT

#include "Constants.h"
#include "lib/Constants.h"
#include "locales/Locales.h"
#include "../Config.h"
#include "../Extended.config.h"
#include "Config.defaults.h"
#include "HAL/HAL.h"
#include "lib/Macros.h"
#include "pinmaps/Models.h"
#include "lib/debug/Debug.h"
#include "lib/serial/Serial_ST4_Slave.h"
#include "lib/serial/Serial_IP_Wifi_Client.h"
