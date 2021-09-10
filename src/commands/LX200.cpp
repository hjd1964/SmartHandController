// -----------------------------------------------------------------------------------
// LX200 command processing

#include <stdint.h>
#include <Ephemeris.h> // https://github.com/MarScaper/ephemeris
#include "../catalogs/Catalog.h"
#include "../userInterface/UserInterface.h"
#include "LX200.h"

// integer numeric conversion with error checking
boolean atoi2(char *a, int *i) {
  char *conv_end;
  long l = strtol(a, &conv_end, 10);

  if (l < -32767 || l > 32768 || &a[0] == conv_end) return false;
  *i = l;
  return true;
}

void serialRecvFlush() {
  while (SERIAL_ONSTEP.available() > 0) SERIAL_ONSTEP.read();
}

void char2RA(char* txt, unsigned int& hour, unsigned int& minute, unsigned int& second) {
  char* pEnd;
  hour = (int)strtol(&txt[0], &pEnd, 10);
  minute = (int)strtol(&txt[3], &pEnd, 10);
  second = (int)strtol(&txt[6], &pEnd, 10);
}

void char2DEC(char* txt, int& deg, unsigned int& min, unsigned int& sec) {
  char* pEnd;
  deg = (int)strtol(&txt[0], &pEnd, 10);
  min = (int)strtol(&txt[4], &pEnd, 10);
  sec = (int)strtol(&txt[7], &pEnd, 10);
}

// smart LX200 aware command and response over serial
bool processCommand(char* command, char* response, unsigned long timeOutMs) {
  SERIAL_ONSTEP.setTimeout(timeOutMs);

  // clear the read/write buffers
  SERIAL_ONSTEP.flush();
  serialRecvFlush();

  // send the command
  SERIAL_ONSTEP.print(command);

  boolean noResponse = false;
  boolean shortResponse = false;
  if ((command[0] == (char)6) && (command[1] == 0)) shortResponse = true;
  if (command[0] == ':') {
    if (command[1] == '%') {
      SERIAL_ONSTEP.setTimeout(timeOutMs * 4);
    }
    if (command[1] == 'A') {
      if (strchr("W123456789+", command[2])) { shortResponse = true; SERIAL_ONSTEP.setTimeout(1000); }
    }
    if ((command[1] == 'F') || (command[1] == 'f')) {
      if (strchr("+-QZHhFS1234", command[2])) noResponse = true;
      if (strchr("Apc",command[2]) || (strchr("B",command[2])&&command[3]!='#') || (strchr("C",command[2])&&command[3]!='#') || (strchr("D",command[2])&&command[3]!='#')) shortResponse=true;
    }
    if (command[1] == 'M') {
      if (strchr("ewnsg", command[2])) noResponse = true;
      if (strchr("SAP", command[2])) shortResponse = true;
    }
    if (command[1] == 'Q') {
      if (strchr("#ewns", command[2])) noResponse = true;
    }
    if (command[1] == 'R') {
      if (strchr("AEGCMS0123456789", command[2])) noResponse = true;
    }
    if (command[1] == 'S') {
      if (strchr("CLSGtgMNOPrdhoTB", command[2])) shortResponse = true;
    }
    if (command[1] == 'L') {
      if (strchr("BNCDL!", command[2])) noResponse = true;
      if (strchr("o$W", command[2])) shortResponse = true;
    }
    if (command[1] == 'B') {
      if (strchr("+-", command[2])) noResponse = true;
    }
    if (command[1] == 'C') {
      if (strchr("S", command[2])) noResponse = true;
    }
    if (command[1] == 'h') {
      if (strchr("F", command[2])) noResponse = true;
      if (strchr("COPQR", command[2])) { shortResponse = true; SERIAL_ONSTEP.setTimeout(timeOutMs * 2); }
    }
    if (command[1] == 'T') {
      if (strchr("QR+-SLK", command[2])) noResponse = true;
      if (strchr("edrn", command[2])) shortResponse = true;
    }
    if (command[1] == 'U') noResponse = true;
    if (command[1] == 'W' && command[2] != '?') { noResponse = true; }
    if (command[1] == '$' && command[2] == 'Q' && command[3] == 'Z') {
      if (strchr("+-Z/!", command[4])) noResponse = true;
    }
    if (command[1] == 'G') {
      if (strchr("RD", command[2])) { timeOutMs *= 2; }
    }
  }

  if (noResponse) {
    response[0] = 0;
    return true;
  }
  else
    if (shortResponse) {
      response[SERIAL_ONSTEP.readBytes(response, 1)] = 0;
      return (response[0] != 0);
    }
    else {
      // get full response, '#' terminated
      unsigned long start = millis();
      int responsePos = 0;
      char b = 0;
      while ((millis() - start < timeOutMs) && (b != '#')) {
        if (SERIAL_ONSTEP.available()) {
          b = SERIAL_ONSTEP.read();
          response[responsePos] = b; responsePos++; if (responsePos > 19) responsePos = 19; response[responsePos] = 0;
        }
        userInterface.tickButtons();
      }
      return (response[0] != 0);
    }
}

bool isOk(LX200RETURN val) {
  return val >= LX200OK;
}

LX200RETURN SetLX200(char* command) {
  char out[20];
  memset(out, 0, sizeof(*out));
  if (processCommand(command, out, TIMEOUT_CMD))
    return  LX200VALUESET;
  else
    return LX200SETVALUEFAILED;
}

LX200RETURN SetLX200(const char* command) {
  return SetLX200((char *)command);
}

LX200RETURN GetLX200(char* command, char* output) {
  memset(output, 0, sizeof(*output));
  if (processCommand(command, output, TIMEOUT_CMD))
    return LX200VALUEGET;
  else
    return LX200GETVALUEFAILED;
}

LX200RETURN GetLX200(const char* command, char* output) {
  return GetLX200((char *)command, output);
}

LX200RETURN GetLX200Trim(char* command, char* output) {
  memset(output, 0, sizeof(*output));
  if (processCommand(command, output, TIMEOUT_CMD)) {
    if ((strlen(output) > 0) && (output[strlen(output) - 1] == '#')) output[strlen(output) - 1] = 0;
    return LX200VALUEGET;
  } else
    return LX200GETVALUEFAILED;
}

LX200RETURN GetLX200Trim(const char* command, char* output) {
  return GetLX200Trim((char *)command, output);
}

LX200RETURN GetTimeLX200(unsigned int &hour, unsigned int &minute, unsigned int &second, boolean ut) {
  char out[20];
  if (ut) {
    if (GetLX200(":GX80#", out) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  } else {
    if (GetLX200(":GL#", out) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  }
  char2RA(out, hour, minute, second);

  return LX200VALUEGET;
}

LX200RETURN GetTimeLX200(long &value, boolean ut) {
  unsigned int hour, minute, second;
  // this had a not (!) before GetTimeLX200 below, no reason it should be there?
  if (GetTimeLX200(hour, minute, second, ut) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  value = hour * 60 + minute;
  value *= 60;
  value += second;
  return LX200VALUEGET;
}

LX200RETURN SetTimeLX200(long &value) {
  char out[20];
  unsigned int hour, minute, second;
  second = value % 60;
  value /= 60;
  minute = value % 60;
  value /= 60;
  hour = value;
  sprintf(out, ":SL%02d:%02d:%02d#", hour, minute, second);
  return SetLX200(out);
}

LX200RETURN GetSiteLX200(int& value) {
  char out[20];
  if (GetLX200(":W?#", out) == LX200VALUEGET) {
    value = (int)strtol(&out[0], NULL, 10) + 1;
    return LX200VALUEGET;
  }
  return LX200GETVALUEFAILED;
}

void SetSiteLX200(int& value) {
  char out[20];
  sprintf(out, ":W%d#", value);
  SERIAL_ONSTEP.print(out);
  SERIAL_ONSTEP.flush();
}

LX200RETURN Move2TargetLX200() {
  char out[20];
  memset(out, 0, sizeof(out));
  processCommand((char *)":MS#", out, TIMEOUT_CMD);
  LX200RETURN response;

  int result = (out[0] - '0');
  if (result == 0) { response = LX200_GOTO_GOINGTO; } else
  if (result > 0 && result <= 9) { response = (LX200RETURN)(result + (int)LX200_GOTO_ERR_BELOW_HORIZON - 1); } else 
  response = LX200_GOTO_ERR_UNSPECIFIED;
  
  return response;
}

LX200RETURN SetTargetRaLX200(int vr1, int vr2, int vr3) {
  char cmd[20], out[20];
  int iter = 0;
  if (vr1 < 0 || vr1 > 24 || vr2 < 0 || vr2 > 59 || vr3 < 0 || vr3 > 59) return  LX200SETVALUEFAILED;
  sprintf(cmd, ":Sr%02d:%02d:%02d#", vr1, vr2, vr3);
  while (iter < 3) {
    if (SetLX200(cmd) ==  LX200VALUESET) {
      if (GetLX200(":Gr#", out) == LX200VALUEGET) {
        unsigned int hour, minute, second;
        char2RA(out, hour, minute, second);
        if ((int)hour == vr1 && (int)minute == vr2 && (int)second == vr3) {
          return LX200VALUESET;
        }
      }
    }
    iter++;
  }
  return LX200SETVALUEFAILED;
}

LX200RETURN SetTargetDecLX200(char sign, int vd1, int vd2, int vd3) {
  char  cmd[20], out[20];
  int iter = 0;
  if ((sign != '-' && sign != '+') || vd1 < 0 || vd1 > 90 || vd2 < 0 || vd2 > 59 || vd3 < 0 || vd3 > 59) return  LX200SETVALUEFAILED;
  sprintf(cmd, ":Sd%c%02d:%02d:%02d#", sign, vd1, vd2, vd3);
  while (iter < 3) {
    if (SetLX200(cmd) ==  LX200VALUESET) {
      if (GetLX200(":Gd#", out) == LX200VALUEGET) {
        int deg;
        unsigned int min, sec;
        char2DEC(out, deg, min, sec);
        if (out[0]==sign && abs(deg) == vd1 && (int)min == vd2 && (int)sec == vd3) {
          return LX200VALUESET;
        }
      }
    }
    iter++;
  }
  return LX200SETVALUEFAILED;
}

LX200RETURN SyncGotoLX200(bool sync, float &Ra, float &Dec) {
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

  if (SetTargetRaLX200(ivr1, ivr2, (int)fvr3) ==  LX200VALUESET && SetTargetDecLX200(sign, ivd1, ivd2, (int)fvd3) == LX200VALUESET) {
    if (sync) {
      SERIAL_ONSTEP.print(":CS#");
      SERIAL_ONSTEP.flush();
      return LX200SYNCED;
    } else return Move2TargetLX200();
  } else return LX200SETTARGETFAILED;
}

LX200RETURN GetDateLX200(unsigned int &day, unsigned int &month, unsigned int &year, boolean ut) {
  char out[20];
  if (ut) {
    if (GetLX200(":GX81#", out) != LX200VALUEGET) return LX200GETVALUEFAILED;
  } else {
    if (GetLX200(":GC#", out) != LX200VALUEGET) return LX200GETVALUEFAILED;
  }
  char* pEnd;
  month = strtol(&out[0], &pEnd, 10);
  day = strtol(&out[3], &pEnd, 10);
  year = strtol(&out[6], &pEnd, 10) + 2000L;
  return LX200VALUEGET;
}

LX200RETURN GetLatitudeLX200(int &degree, int &minute, int &second) {
  char out[20];
  if (GetLX200(":Gt#", out) != LX200VALUEGET) return LX200GETVALUEFAILED;
  char* pEnd;
  degree = strtol(&out[0], &pEnd, 10);
  minute = strtol(&out[4], &pEnd, 10);
  second = 0;
  return LX200VALUEGET;
}

LX200RETURN GetLongitudeLX200(int &degree, int &minute, int &second) {
  char out[20];
  if (GetLX200(":Gg#", out) != LX200VALUEGET) return LX200GETVALUEFAILED;
  char* pEnd;
  degree = strtol(&out[0], &pEnd, 10);
  minute = strtol(&out[5], &pEnd, 10);
  second = 0;
  return LX200VALUEGET;
}

LX200RETURN SyncGotoCatLX200(bool sync) {
  int epoch;
  unsigned int day, month, year;
  if (GetDateLX200(day, month, year, true) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  if (!cat_mgr.isStarCatalog() && !cat_mgr.isDsoCatalog()) return LX200UNKOWN;
  EquatorialCoordinates coo;
  coo.ra = cat_mgr.rah();
  coo.dec = cat_mgr.dec();
  epoch=cat_mgr.epoch(); if (epoch==0) return LX200GETVALUEFAILED;
  EquatorialCoordinates cooNow;
  cooNow = Ephemeris::equatorialEquinoxToEquatorialJNowAtDateAndTime(coo, epoch, day, month, year, 0, 0, 0);
  return SyncGotoLX200(sync, cooNow.ra, cooNow.dec);
}

LX200RETURN SyncGotoPlanetLX200(bool sync, unsigned short objSys) {
  Ephemeris Eph;

  unsigned int day, month, year;
  unsigned int hour, minute, second;
  if (GetDateLX200(day, month, year, true) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  if (GetTimeLX200(hour, minute, second, true) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;

  int longD, longM, longS;
  int latD, latM, latS;
  if (GetLongitudeLX200(longD, longM, longS) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;
  if (GetLatitudeLX200(latD, latM, latS) == LX200GETVALUEFAILED) return LX200GETVALUEFAILED;

  Eph.setLocationOnEarth(latD,latM,latS,longD,longM,longS); // Set Latitude and Longitude
  Eph.flipLongitude(true);                                  // East is negative and West is positive
  Eph.setAltitude(100);                                     // A good average altitude (in meters?) for observers?
  
  SolarSystemObjectIndex objI = static_cast<SolarSystemObjectIndex>(objSys);
  SolarSystemObject obj = Eph.solarSystemObjectAtDateAndTime(objI, day, month, year, hour, minute, second);
  return SyncGotoLX200(sync, obj.equaCoordinates.ra, obj.equaCoordinates.dec);
}

LX200RETURN SyncSelectedStarLX200(long alignSelectedStar) {
  if (alignSelectedStar >= 0) return SyncGotoCatLX200(false); else return LX200UNKOWN;
}

LX200RETURN readBacklashLX200(const uint8_t &axis, float &backlash) {
  char out[20];
  LX200RETURN ok = axis == 1 ? GetLX200(":%BR#", out) : GetLX200(":%BD#", out);
  if (ok == LX200VALUEGET) {
    backlash = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

LX200RETURN writeBacklashLX200(const uint8_t &axis, const float &backlash) {
  char text[20];
  sprintf(text, ":$BX%u#", (unsigned int)backlash);
  text[3] = axis == 1 ? 'R' : 'D';
  return SetLX200(text);
}

LX200RETURN readFocTCCoefLX200(const uint8_t &foc, float &tccoef) {
  char out[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  LX200RETURN ok = GetLX200(":FC#", out);
  if (ok == LX200VALUEGET) {
    tccoef = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

LX200RETURN writeFocTCCoefLX200(const uint8_t &foc, const float &tccoef) {
  char text[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  sprintf(text, ":FC%d#", (signed int)tccoef);
  return SetLX200(text);
}
LX200RETURN readFocBacklashLX200(const uint8_t &foc, float &backlash) {
  char out[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  LX200RETURN ok = GetLX200(":FB#", out);
  if (ok == LX200VALUEGET) {
    backlash = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

LX200RETURN writeFocBacklashLX200(const uint8_t &foc, const float &backlash) {
  char text[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  sprintf(text, ":FB%d#", (signed int)backlash);
  return SetLX200(text);
}
LX200RETURN readFocTCDeadbandLX200(const uint8_t &foc, float &tcdeadband) {
  char out[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  LX200RETURN ok = GetLX200(":FD#", out);
  if (ok == LX200VALUEGET)
  {
    tcdeadband = (float)strtol(&out[0], NULL, 10);
  }
  return ok;
}

LX200RETURN writeFocTCDeadbandLX200(const uint8_t &foc, const float &tcdeadband) {
  char text[20];
  if (foc == 1) SetLX200(":FA1#"); else SetLX200(":FA2#");
  sprintf(text, ":FD%d#", (signed int)tcdeadband);
  return SetLX200(text);
}

// convert string in format HH:MM:SS to floating point
// (also handles)           HH:MM.M
boolean hmsToDouble(double *f, char *hms) {
  char h[3], m[5], s[3];
  int  h1, m1, m2 = 0, s1 = 0;
  boolean highPrecision = true;
  
  while (*hms == ' ') hms++; // strip prefix white-space

  if (highPrecision) { if (strlen(hms) != 8) return false; } else if (strlen(hms) != 7) return false;

  h[0] = *hms++; h[1] = *hms++; h[2] = 0; atoi2(h, &h1);
  if (highPrecision) {
    if (*hms++ != ':') return false;
    m[0] = *hms++; m[1] = *hms++; m[2] = 0; atoi2(m, &m1);
    if (*hms++ != ':') return false;
    s[0] = *hms++; s[1] = *hms++; s[2] = 0; atoi2(s, &s1);
  } else {
    if (*hms++!=':') return false;
    m[0] = *hms++; m[1] = *hms++; m[2] = 0; atoi2(m, &m1);
    if (*hms++ != '.') return false;
    m2 = (*hms++) - '0';
  }
  if (h1 < 0 || h1 > 23 || m1 < 0 || m1 > 59 || m2 < 0 || m2 > 9 || s1 < 0 || s1 > 59) return false;
  
  *f = h1 + m1/60.0 + m2/600.0 + s1/3600.0;
  return true;
}

// convert string in format sDD:MM:SS to floating point
// (also handles)           DDD:MM:SS
//                          sDD:MM
//                          DDD:MM
//                          sDD*MM
//                          DDD*MM
boolean dmsToDouble(double *f, char *dms, boolean sign_present, boolean highPrecision) {
  char d[4], m[5], s[3];
  int d1, m1, s1 = 0;
  int lowLimit = 0, highLimit = 360;
  int checkLen, checkLen1;
  double sign = 1.0;
  boolean secondsOff = false;

  while (*dms == ' ') dms++; // strip prefix white-space

  checkLen1 = strlen(dms);

  // determine if the seconds field was used and accept it if so
  if (highPrecision) { 
    checkLen = 9;
    if (checkLen1 != checkLen) return false;
  } else {
    checkLen = 6;
    if (checkLen1 != checkLen) {
      if (checkLen1 == 9) { secondsOff = false; checkLen = 9; } else return false;
    } else secondsOff = true;
  }

  // determine if the sign was used and accept it if so
  if (sign_present) {
    if (*dms == '-') sign = -1.0; else if (*dms == '+') sign = 1.0; else return false; 
    dms++; d[0] = *dms++; d[1] = *dms++; d[2] = 0; if (!atoi2(d, &d1)) return false;
  } else {
    d[0] = *dms++; d[1] = *dms++; d[2] = *dms++; d[3] = 0; if (!atoi2(d, &d1)) return false;
  }

  // make sure the seperator is an allowed character
  if (*dms != ':' && *dms != '*' && *dms != char(223)) return false; else dms++;

  m[0] = *dms++; m[1] = *dms++; m[2] = 0; if (!atoi2(m, &m1)) return false;

  if (highPrecision && !secondsOff) {
    // make sure the seperator is an allowed character
    if (*dms++ != ':') return false; 
    s[0] = *dms++; s[1] = *dms++; s[2] = 0; atoi2(s, &s1);
  }

  if (sign_present) { lowLimit = -90; highLimit = 90; }
  if (d1 < lowLimit || d1 > highLimit || m1 < 0 || m1 > 59 || s1 < 0 || s1 > 59) return false;
  
  *f = sign*(d1 + m1/60.0 + s1/3600.0);
  return true;
}
