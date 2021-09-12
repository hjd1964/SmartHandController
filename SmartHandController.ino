/*
* Title       Smart Hand Controller (based on TeenAstro)
*
* Copyright (C) 2018 to 2021 Charles Lemaire, Howard Dutton, and Others
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
* for the actual hardware see: https://easyeda.com/hdutton/HC-20e242d665db4c85bb565a0cd0b52233
*/

#define Product               "SHC"
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "1"
#define FirmwareVersionPatch  "a"

#include "src/Common.h"
NVS nv;
#include "src/tasks/OnTask.h"
Tasks tasks;
#include "src/userInterface/UserInterface.h"

const char Version[] = "Version " FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch;
const int pin[7] = { B_PIN0,B_PIN1,B_PIN2,B_PIN3,B_PIN4,B_PIN5,B_PIN6 };
const bool active[7] = { B_PIN_UP_0,B_PIN_UP_1,B_PIN_UP_2,B_PIN_UP_3,B_PIN_UP_4,B_PIN_UP_5,B_PIN_UP_6 };

UI userInterface;
bool connected = false;

void systemServices() {
  nv.poll();
}

void setup(void) {
  HAL_INIT();
  
  // start debug serial port
  if (DEBUG == ON || DEBUG == VERBOSE) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  VF(Abv "Smart Hand Controller "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
  VF(Abv "MCU = "); VLF(MCU_STR);
  
  // System services
  // add task for system services, runs at 10ms intervals so commiting 1KB of NV takes about 10 seconds
  VF(Abv "Setup, starting system services task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!"); }

  userInterface.setup(Version, pin, active, SERIAL_BAUD_DEFAULT, static_cast<UI::OLED>(DISPLAY_OLED));

  VLF(Abv "Starting UI loop");
}

void loop() {
  userInterface.update();
  tasks.yield();
}
