/*
* Title       Smart Hand Controller (based on TeenAstro)
*
* Copyright (C) 2018 to 2025 Charles Lemaire, Howard Dutton, and Others
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Revision History, see GitHub
*
* Author: Charles Lemaire, https://pixelstelescopes.wordpress.com/teenastro/
* Author: Howard Dutton, http://www.stellarjourney.com, hjd1964@gmail.com
*
* Description
*
* Smart Hand controller addon for OnStep
* for the actual hardware see: https://oshwlab.com/hdutton/smart-hand-controller2-plus
*/

#define Product               "SHC"
#define FirmwareVersionMajor  "4"
#define FirmwareVersionMinor  "02"
#define FirmwareVersionPatch  "j"

#include "src/Common.h"
#include "src/Validate.h"

#include "src/lib/nv/Nv.h"
#include "src/lib/tasks/OnTask.h"
#include "src/lib/convert/Convert.h"

#include "src/libApp/weather/Weather.h"

#include "src/userInterface/UserInterface.h"

#if DEBUG == PROFILER
  extern void profiler();
#endif

const char Version[] = "Version " FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch;
const int pin[7] = {B_PIN0, B_PIN1, B_PIN2, B_PIN3, B_PIN4, B_PIN5, B_PIN6};
const int active[7] = {B_PIN0_ACTIVE_STATE, B_PIN1_ACTIVE_STATE, B_PIN2_ACTIVE_STATE, B_PIN3_ACTIVE_STATE, B_PIN4_ACTIVE_STATE, B_PIN5_ACTIVE_STATE, B_PIN6_ACTIVE_STATE};

void systemServices() {
  nv.poll();
}

#if WEATHER != OFF
  void weatherServices() {
    static int i = 0;
    char command[80];

    switch (i++ % 3) {
      case 0: sprintF(command, ":SX9A,%0.1f#", weather.getTemperature()); onStepLx200.Set(command); break;
      case 1: sprintF(command, ":SX9B,%0.1f#", weather.getPressure()); onStepLx200.Set(command); break;
      case 2: sprintF(command, ":SX9C,%0.1f#", weather.getHumidity()); onStepLx200.Set(command); break;
    }
  }
#endif

void setup(void) {
  
  // start debug serial port
  if (DEBUG == ON || DEBUG == VERBOSE) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  VF("MSG: Smart Hand Controller "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
  VF("MSG: MCU = "); VLF(MCU_STR);
  
  HAL_INIT();
  #if SERIAL_BT_MODE != OFF
    SERIAL_BT.begin(SERIAL_BT_NAME, true);
  #endif
  nv.init();
  
  // System services
  // add task for system services, runs at 10ms intervals so commiting 1KB of NV takes about 10 seconds
  VF("MSG: Setup, starting system services task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!"); }

  userInterface.init(Version, pin, active, SERIAL_ONSTEP_BAUD_DEFAULT, static_cast<OLED>(DISPLAY_OLED));

  #if WEATHER != OFF
    // get any BME280 or BMP280 ready
    weather.init();

    // add task to forward readings to OnStep
    VF("MSG: Setup, starting weather services task (rate 3333ms priority 7)... ");
    if (tasks.add(3333, 0, true, 7, weatherServices, "WeaFwd")) { VL("success"); } else { VL("FAILED!"); }
  #endif

  // start task manager debug events
  #if DEBUG == PROFILER
    tasks.add(142, 0, true, 7, profiler, "Profilr");
  #endif

  VLF("MSG: Starting UI loop");
}

void loop() {
  tasks.yield();
}
