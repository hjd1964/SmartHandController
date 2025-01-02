// -----------------------------------------------------------------------------------
// OnStep command processing

#include <Ephemeris.h> // https://github.com/MarScaper/ephemeris

#include "../../lib/tasks/OnTask.h"
#include "../../catalogs/Catalog.h"
#include "../../userInterface/UserInterface.h"
#include "CmdLx200.h"

bool CmdLx200::isOk(CMD_RESULT val) {
  return val >= CR_OK;
}

void CmdLx200::SetF(const char* command) {
  onStep.commandDirect(command);
}

CMD_RESULT CmdLx200::Set(char* command) {
  char out[80];
  if (onStep.processCommand(command, out, TIMEOUT_CMD))
    return CR_VALUE_SET;
  else
    return CR_SET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::Set(const char* command) {
  return Set((char *)command);
}

CMD_RESULT CmdLx200::Get(char* command, char* output) {
  if (onStep.processCommand(command, output, TIMEOUT_CMD))
    return CR_VALUE_GET;
  else
    return CR_GET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::Get(const char* command, char* output) {
  return Get((char *)command, output);
}

CMD_RESULT CmdLx200::GetTrim(char* command, char* output) {
  if (onStep.processCommand(command, output, TIMEOUT_CMD)) {
    if ((strlen(output) > 0) && (output[strlen(output) - 1] == '#')) output[strlen(output) - 1] = 0;
    return CR_VALUE_GET;
  } else
    return CR_GET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::GetTrim(const char* command, char* output) {
  return GetTrim((char *)command, output);
}

CMD_RESULT CmdLx200::GetTime(unsigned int &hour, unsigned int &minute, unsigned int &second, boolean ut) {
  char out[80];

  if (ut) {
    if (Get(":GX80#", out) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  } else {
    if (Get(":GL#", out) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  }
  char2RA(out, hour, minute, second);

  return CR_VALUE_GET;
}

CMD_RESULT CmdLx200::GetTime(long &value, boolean ut) {
  unsigned int hour, minute, second;

  if (GetTime(hour, minute, second, ut) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  value = hour * 60 + minute;
  value *= 60;
  value += second;

  return CR_VALUE_GET;
}

CMD_RESULT CmdLx200::SetTime(long &value) {
  char out[80];

  unsigned int hour, minute, second;
  second = value % 60;
  value /= 60;
  minute = value % 60;
  value /= 60;
  hour = value;
  sprintf(out, ":SL%02d:%02d:%02d#", hour, minute, second);

  return Set(out);
}

CMD_RESULT CmdLx200::GetSite(int& value) {
  char out[80];

  if (Get(":W?#", out) == CR_VALUE_GET) {
    value = (int)strtol(&out[0], NULL, 10) + 1;
    return CR_VALUE_GET;
  }

  return CR_GET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::SetSite(int& value) {
  char out[80];

  sprintf(out, ":W%d#", value);

  return Set(out);
}

CMD_RESULT CmdLx200::Move2Target() {
  char out[20];
  memset(out, 0, sizeof(out));
  onStep.processCommand((char *)":MS#", out, TIMEOUT_CMD);
  CMD_RESULT response;

  int result = (out[0] - '0');
  if (result == 0) { response = CR_GOTO_GOING_TO; } else
  if (result > 0 && result <= 9) { response = (CMD_RESULT)(result + (int)CR_GOTO_ERR_BELOW_HORIZON - 1); } else 
  response = CR_GOTO_ERR_UNSPECIFIED;
  
  return response;
}

CMD_RESULT CmdLx200::SetTargetRa(int vr1, int vr2, int vr3) {
  char cmd[20], out[80];

  int iter = 0;
  if (vr1 < 0 || vr1 > 24 || vr2 < 0 || vr2 > 59 || vr3 < 0 || vr3 > 59) return  CR_SET_VALUE_FAILED;
  sprintf(cmd, ":Sr%02d:%02d:%02d#", vr1, vr2, vr3);
  while (iter < 3) {
    if (Set(cmd) ==  CR_VALUE_SET) {
      if (Get(":Gr#", out) == CR_VALUE_GET) {
        unsigned int hour, minute, second;
        char2RA(out, hour, minute, second);
        if ((int)hour == vr1 && (int)minute == vr2 && (int)second == vr3) {
          return CR_VALUE_SET;
        }
      }
    }
    iter++;
  }

  return CR_SET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::SetTargetDec(char sign, int vd1, int vd2, int vd3) {
  char  cmd[20], out[80];
  int iter = 0;

  if ((sign != '-' && sign != '+') || vd1 < 0 || vd1 > 90 || vd2 < 0 || vd2 > 59 || vd3 < 0 || vd3 > 59) return  CR_SET_VALUE_FAILED;
  sprintf(cmd, ":Sd%c%02d:%02d:%02d#", sign, vd1, vd2, vd3);
  while (iter < 3) {
    if (Set(cmd) ==  CR_VALUE_SET) {
      if (Get(":Gd#", out) == CR_VALUE_GET) {
        int deg;
        unsigned int min, sec;
        char2DEC(out, deg, min, sec);
        if (out[0]==sign && abs(deg) == vd1 && (int)min == vd2 && (int)sec == vd3) {
          return CR_VALUE_SET;
        }
      }
    }
    iter++;
  }

  return CR_SET_VALUE_FAILED;
}

CMD_RESULT CmdLx200::SyncGoto(bool sync, float &Ra, float &Dec) {
  int ivr1, ivr2, ivd1, ivd2;
  float fvr3, fvd3;
  char sign='+';

  // apply refraction if required, converts from the "Topocentric" to "Observed"
  if (userInterface.telescopeCoordinates == OBSERVED_PLACE) cat_mgr.topocentricToObservedPlace(&Ra,&Dec);
  
  Ephemeris::floatingHoursToHoursMinutesSeconds(Ra, &ivr1, &ivr2, &fvr3);
  Ephemeris::floatingDegreesToDegreesMinutesSeconds(Dec, &ivd1, &ivd2, &fvd3);

  // Make sure that degrees does not have a sign, if we are south of the celestial equator
  ivd1 = abs(ivd1);

  if (Dec<0.0) sign='-';

  if (SetTargetRa(ivr1, ivr2, (int)fvr3) ==  CR_VALUE_SET && SetTargetDec(sign, ivd1, ivd2, (int)fvd3) == CR_VALUE_SET) {
    if (sync) {
      onStepLx200.Set(":CS#");
      return CR_SYNCED;
    } else return Move2Target();
  } else return CR_SET_TARGET_FAILED;
}

CMD_RESULT CmdLx200::GetDate(unsigned int &day, unsigned int &month, unsigned int &year, boolean ut) {
  char out[80];

  if (ut) {
    if (Get(":GX81#", out) != CR_VALUE_GET) return CR_GET_VALUE_FAILED;
  } else {
    if (Get(":GC#", out) != CR_VALUE_GET) return CR_GET_VALUE_FAILED;
  }
  char* pEnd;
  month = strtol(&out[0], &pEnd, 10);
  day = strtol(&out[3], &pEnd, 10);
  year = strtol(&out[6], &pEnd, 10) + 2000L;

  return CR_VALUE_GET;
}

CMD_RESULT CmdLx200::SyncGotoCat(bool sync) {
  int epoch;
  unsigned int day, month, year;

  if (GetDate(day, month, year, true) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  if (!cat_mgr.isStarCatalog() && !cat_mgr.isDsoCatalog()) return CR_UNKOWN;
  EquatorialCoordinates coo;
  coo.ra = cat_mgr.rah();
  coo.dec = cat_mgr.dec();
  epoch=cat_mgr.epoch(); if (epoch==0) return CR_GET_VALUE_FAILED;
  EquatorialCoordinates cooNow;
  cooNow = Ephemeris::equatorialEquinoxToEquatorialJNowAtDateAndTime(coo, epoch, day, month, year, 0, 0, 0);

  return SyncGoto(sync, cooNow.ra, cooNow.dec);
}

CMD_RESULT CmdLx200::SyncGotoPlanet(bool sync, unsigned short objSys) {
  Ephemeris Eph;

  unsigned int day, month, year;
  unsigned int hour, minute, second;
  if (GetDate(day, month, year, true) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  if (GetTime(hour, minute, second, true) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;

  int longD, longM, longS;
  int latD, latM, latS;
  if (GetLongitude(longD, longM, longS) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;
  if (GetLatitude(latD, latM, latS) == CR_GET_VALUE_FAILED) return CR_GET_VALUE_FAILED;

  Eph.setLocationOnEarth(latD,latM,latS,longD,longM,longS); // Set Latitude and Longitude
  Eph.flipLongitude(true);                                  // East is negative and West is positive
  Eph.setAltitude(100);                                     // A good average altitude (in meters?) for observers?
  
  SolarSystemObjectIndex objI = static_cast<SolarSystemObjectIndex>(objSys);
  SolarSystemObject obj = Eph.solarSystemObjectAtDateAndTime(objI, day, month, year, hour, minute, second);

  return SyncGoto(sync, obj.equaCoordinates.ra, obj.equaCoordinates.dec);
}

CMD_RESULT CmdLx200::SyncSelectedStar(long alignSelectedStar) {
  if (alignSelectedStar >= 0) return SyncGotoCat(false); else return CR_UNKOWN;
}

CMD_RESULT CmdLx200::readBacklash(const uint8_t &axis, float &backlash) {
  char out[80];

  CMD_RESULT ok = axis == 1 ? Get(":%BR#", out) : Get(":%BD#", out);
  if (ok == CR_VALUE_GET) {
    backlash = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

CMD_RESULT CmdLx200::writeBacklash(const uint8_t &axis, const float &backlash) {
  char text[20];

  sprintf(text, ":$BX%u#", (unsigned int)backlash);
  text[3] = axis == 1 ? 'R' : 'D';

  return Set(text);
}

CMD_RESULT CmdLx200::readFocTCCoef(const uint8_t &foc, float &tccoef) {
  char out[80];

  if (foc == 1) Set(":FA1#"); else Set(":FA2#");
  CMD_RESULT ok = Get(":FC#", out);
  if (ok == CR_VALUE_GET) {
    tccoef = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

CMD_RESULT CmdLx200::writeFocTCCoef(const uint8_t &foc, const float &tccoef) {
  char cmd[20];

  sprintf(cmd, ":FA%u#", foc);
  Set(cmd);
  sprintf(cmd, ":FC%d#", (signed int)tccoef);

  return Set(cmd);
}

CMD_RESULT CmdLx200::readFocBacklash(const uint8_t &foc, float &backlash) {
  char cmd[20], out[80];

  sprintf(cmd, ":FA%u#", foc);
  Set(cmd);
  CMD_RESULT ok = Get(":FB#", out);
  if (ok == CR_VALUE_GET) {
    backlash = (float)strtol(&out[0], NULL, 10);
  }

  return ok;
}

CMD_RESULT CmdLx200::writeFocBacklash(const uint8_t &foc, const float &backlash) {
  char cmd[20];

  sprintf(cmd, ":FA%u#", foc);
  Set(cmd);
  sprintf(cmd, ":FB%d#", (signed int)backlash);

  return Set(cmd);
}

CMD_RESULT CmdLx200::readFocTCDeadband(const uint8_t &foc, float &tcdeadband) {
  char cmd[20], out[80];

  sprintf(cmd, ":FA%u#", foc);
  Set(cmd);
  CMD_RESULT ok = Get(":FD#", out);
  if (ok == CR_VALUE_GET) {
    tcdeadband = (float)strtol(&out[0], NULL, 10);
  }

  return ok;
}

CMD_RESULT CmdLx200::writeFocTCDeadband(const uint8_t &foc, const float &tcdeadband) {
  char cmd[20];

  sprintf(cmd, ":FA%u#", foc);
  Set(cmd);
  sprintf(cmd, ":FD%d#", (signed int)tcdeadband);

  return Set(cmd);
}

CMD_RESULT CmdLx200::GetLatitude(int &degree, int &minute, int &second) {
  char out[80];

  if (Get(":Gt#", out) != CR_VALUE_GET) return CR_GET_VALUE_FAILED;
  char* pEnd;
  degree = strtol(&out[0], &pEnd, 10);
  minute = strtol(&out[4], &pEnd, 10);
  second = 0;

  return CR_VALUE_GET;
}

CMD_RESULT CmdLx200::GetLongitude(int &degree, int &minute, int &second) {
  char out[80];

  if (Get(":Gg#", out) != CR_VALUE_GET) return CR_GET_VALUE_FAILED;
  char* pEnd;
  degree = strtol(&out[0], &pEnd, 10);
  minute = strtol(&out[5], &pEnd, 10);
  second = 0;

  return CR_VALUE_GET;
}

void CmdLx200::char2RA(char* txt, unsigned int& hour, unsigned int& minute, unsigned int& second) {
  char* pEnd;

  hour = (int)strtol(&txt[0], &pEnd, 10);
  minute = (int)strtol(&txt[3], &pEnd, 10);
  second = (int)strtol(&txt[6], &pEnd, 10);
}

void CmdLx200::char2DEC(char* txt, int& deg, unsigned int& min, unsigned int& sec) {
  char* pEnd;

  deg = (int)strtol(&txt[0], &pEnd, 10);
  min = (int)strtol(&txt[4], &pEnd, 10);
  sec = (int)strtol(&txt[7], &pEnd, 10);
}

CmdLx200 onStepLx200;
