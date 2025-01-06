// -----------------------------------------------------------------------------------
// Help with commands, etc.
#pragma once

#include "../../Common.h"

enum CommandErrors {
  CE_NONE, CE_0, CE_CMD_UNKNOWN, CE_REPLY_UNKNOWN, CE_PARAM_RANGE, CE_PARAM_FORM,
  CE_ALIGN_FAIL, CE_ALIGN_NOT_ACTIVE, CE_NOT_PARKED_OR_AT_HOME, CE_PARKED,
  CE_PARK_FAILED, CE_NOT_PARKED, CE_NO_PARK_POSITION_SET, CE_GOTO_FAIL, CE_LIBRARY_FULL,
  CE_GOTO_ERR_BELOW_HORIZON, CE_GOTO_ERR_ABOVE_OVERHEAD, CE_SLEW_ERR_IN_STANDBY, 
  CE_SLEW_ERR_IN_PARK, CE_GOTO_ERR_GOTO, CE_GOTO_ERR_OUTSIDE_LIMITS, CE_SLEW_ERR_HARDWARE_FAULT,
  CE_MOUNT_IN_MOTION, CE_GOTO_ERR_UNSPECIFIED, CE_NULL};

enum ConnectionMode {CM_NONE, CM_SERIAL, CM_WIFI, CM_BLUETOOTH};

class OnStepCmd {
  public:
    // low level smart LX200 aware command and response (up to 80 chars) over serial (includes any '#' frame char)
    bool processCommand(const char* cmd, char* response, long timeOutMs);

    // writes command directly to buffer
    void commandDirect(const char* command);

    ConnectionMode connectionMode = CM_NONE;

  private:
};

extern OnStepCmd onStep;
