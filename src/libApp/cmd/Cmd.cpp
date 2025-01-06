// -----------------------------------------------------------------------------------
// Help with commands, etc.

#include "Cmd.h"

#include "..\..\lib\tasks\OnTask.h"

// smart LX200 aware command and response (up to 80 chars) over serial
bool OnStepCmd::processCommand(const char* cmd, char* response, long timeOutMs) {

  // clear the queues and send the command
  #if SERIAL_IP_MODE != OFF
    if (connectionMode == CM_WIFI) {
      SERIAL_IP.flush();
      SERIAL_IP.setTimeout(timeOutMs);
      while (SERIAL_IP.available() > 0) SERIAL_IP.read();
      SERIAL_IP.print(cmd);
      SERIAL_IP.flush();
    }
  #endif
  #if SERIAL_BT_MODE != OFF
    if (connectionMode == CM_BLUETOOTH) {
      SERIAL_BT.flush();
      SERIAL_BT.setTimeout(timeOutMs);
      while (SERIAL_BT.available() > 0) SERIAL_BT.read();
      SERIAL_BT.print(cmd);
      SERIAL_BT.flush();
    }
  #endif
  #if SERIAL_ONSTEP != OFF
    if (connectionMode == CM_SERIAL) {
      SERIAL_ONSTEP.flush();
      SERIAL_ONSTEP.setTimeout(timeOutMs);
      while (SERIAL_ONSTEP.available() > 0) SERIAL_ONSTEP.read();
      SERIAL_ONSTEP.print(cmd);
      SERIAL_ONSTEP.flush();
    }
  #endif

  response[0] = 0;
  bool noResponse = false;
  bool shortResponse = false;
  if (cmd[0] == (char)6 && cmd[1] == 0) shortResponse = true;
  if (cmd[0] == ':' || cmd[0] == ';') {
    if (cmd[1] == 'G') {
      if (strchr("RDE", cmd[2])) { if (timeOutMs < 300) timeOutMs = 300; }
      if (cmd[2] == 'X' && cmd[3] == 'E' && cmd[4] == 'E') shortResponse = true;
    } else
    if (cmd[1] == 'M') {
      if (strchr("ewnsg", cmd[2])) noResponse = true;
      if (strchr("ADNPS", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'Q') {
      if (strchr("#ewns", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'A') {
      if (strchr("W123456789+",cmd[2])) { shortResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
    } else
    if ((cmd[1] == 'F') || (cmd[1] == 'f')) {
      if (strchr("123456", cmd[2]) && cmd[3] != '#') {
        // direct focuser select command?
        if (strchr("+-QZHhF", cmd[3])) noResponse = true;
        if (strchr("1234", cmd[3])) noResponse = true;
        if (strchr("Aapc",cmd[3])) shortResponse = true;
      } else {
        // normal command
        if (strchr("+-QZHhF", cmd[2])) noResponse = true;
        if (strchr("1234", cmd[2])) noResponse = true;
        if (strchr("Aapc",cmd[2])) shortResponse = true;
      }
    } else
    if (cmd[1] == 'r') {
      if (strchr("+-PRFC<>Q1234", cmd[2])) noResponse = true;
      if (strchr("~S",cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'R') {
      if (strchr("AEGCMS0123456789", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'S') {
      if (strchr("CLSGtgMNOPrdhoTBX", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'L') {
      if (strchr("BNCDL!",cmd[2])) noResponse = true;
      if (strchr("o$W", cmd[2])) { shortResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
    } else
    if (cmd[1] == 'B') {
      if (strchr("+-", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'C') {
      if (strchr("S", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'h') {
      if (strchr("FC", cmd[2])) { noResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
      if (strchr("QPR", cmd[2])) { shortResponse = true; if (timeOutMs < 300) timeOutMs = 300; }
    } else
    if (cmd[1] == 'T') {
      if (strchr("QR+-SLK", cmd[2])) noResponse = true;
      if (strchr("edrn", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'U') {
      noResponse = true; 
    } else
    if (cmd[1] == 'W') {
      if (strchr("R", cmd[2])) {
        if (strchr("+-", cmd[3])) shortResponse = true; else noResponse = true; // WR+ WR- else WR
      }
      if (strchr("S", cmd[2])) shortResponse = true;  // WS
      if (strchr("0123", cmd[2])) noResponse = true;  // W0 W1 W2 W3
    } else
    if (cmd[1] == '$' && cmd[2] == 'Q' && cmd[3] == 'Z') {
      if (strchr("+-Z/!", cmd[4])) noResponse = true;
    }

    // override for checksum protocol
    if (cmd[0] == ';') { noResponse = false; shortResponse = false; }
  }

  unsigned long timeout = millis() + (unsigned long)timeOutMs;
  if (noResponse) {
    response[0] = 0;
    return true;
  } else
  if (shortResponse) {
    while ((long)(timeout - millis()) > 0) {
      int available;
      #if SERIAL_IP_MODE != OFF
        if (connectionMode == CM_WIFI) available = SERIAL_IP.available();
      #endif
      #if SERIAL_BT_MODE != OFF
        if (connectionMode == CM_BLUETOOTH) available = SERIAL_BT.available();
      #endif
      #if SERIAL_ONSTEP != OFF
        if (connectionMode == CM_SERIAL) available = SERIAL_ONSTEP.available();
      #endif

      if (available) {
        int length = 0;
        #if SERIAL_IP_MODE != OFF
          if (connectionMode == CM_WIFI) length = SERIAL_IP.readBytes(response, 1);
        #endif
        #if SERIAL_BT_MODE != OFF
          if (connectionMode == CM_BLUETOOTH) length = SERIAL_BT.readBytes(response, 1);
        #endif
        #if SERIAL_ONSTEP != OFF
          if (connectionMode == CM_SERIAL) length = SERIAL_ONSTEP.readBytes(response, 1);
        #endif

        response[length] = 0;
        break;
      }
    }
    if ((long)(timeout - millis()) <= 0) { DF("WRN: cmd "); D(cmd); DLF(" timed out"); }

    return (response[0] != 0);
  } else {
    // get full response, '#' terminated
    int responsePos = 0;
    char b = 0;
    while ((long)(timeout - millis()) > 0 && b != '#') {
      int available = 0;
      #if SERIAL_IP_MODE != OFF
        if (connectionMode == CM_WIFI) available = SERIAL_IP.available();
      #endif
      #if SERIAL_BT_MODE != OFF
        if (connectionMode == CM_BLUETOOTH) available = SERIAL_BT.available();
      #endif
      #if SERIAL_ONSTEP != OFF
        if (connectionMode == CM_SERIAL) available = SERIAL_ONSTEP.available();
      #endif

      if (available) {
        #if SERIAL_IP_MODE != OFF
          if (connectionMode == CM_WIFI) b = SERIAL_IP.read();
        #endif
        #if SERIAL_BT_MODE != OFF
          if (connectionMode == CM_BLUETOOTH) b = SERIAL_BT.read();
        #endif
        #if SERIAL_ONSTEP != OFF
          if (connectionMode == CM_SERIAL) b = SERIAL_ONSTEP.read();
        #endif

        response[responsePos] = b;
        responsePos++;
        if (responsePos > 79) responsePos = 79;
        response[responsePos] = 0;
      }
      Y;
    }
    if ((long)(timeout - millis()) <= 0) { DF("WRN: cmd \""); D(cmd); DLF("\" timed out"); }

    return response[strlen(response) - 1] == '#';
  }
}

void OnStepCmd::commandDirect(const char* command) {
  #if SERIAL_IP_MODE != OFF
    if (connectionMode == CM_WIFI) SERIAL_IP.write(command);
  #endif
  #if SERIAL_BT_MODE != OFF
    if (connectionMode == CM_BLUETOOTH) SERIAL_BT.print(command);
  #endif
  #if SERIAL_ONSTEP != OFF
    if (connectionMode == CM_SERIAL) SERIAL_ONSTEP.write(command);
  #endif
}

OnStepCmd onStep;
