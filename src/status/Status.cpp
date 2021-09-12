// -----------------------------------------------------------------------------------
// Status, from OnStep

#include "Status.h"
#include "../commands/LX200.h"

void Status::updateRaDec(boolean immediate) {
  if (((millis() - lastStateRaDec > BACKGROUND_CMD_RATE) && connected) || immediate)
  {
    if (updateSeq%3 == 1 || immediate) { hasInfoRa = GetLX200(":GR#", TempRa) == LX200VALUEGET; if (!hasInfoRa) connected = true; }
    if (updateSeq%3 == 2 || immediate) { hasInfoDec = GetLX200(":GD#", TempDec) == LX200VALUEGET; if (!hasInfoDec) connected = true; lastStateRaDec = millis(); }
  }
};

void Status::updateAzAlt(boolean immediate) {
  if (((millis() - lastStateAzAlt > BACKGROUND_CMD_RATE) && connected) || immediate)
  {
    if (updateSeq%3 == 1 || immediate) { hasInfoAz = GetLX200(":GZ#", TempAz) == LX200VALUEGET; if (!hasInfoAz) connected = true; }
    if (updateSeq%3 == 2 || immediate) { hasInfoAlt = GetLX200(":GA#", TempAlt) == LX200VALUEGET; if (!hasInfoAlt) connected = true; lastStateAzAlt = millis(); }
  }
}

void Status::updateTime(boolean immediate) {
  if (((millis() - lastStateTime > BACKGROUND_CMD_RATE) && connected) || immediate)
  {
    if (updateSeq%3 == 1 || immediate) { hasInfoUTC = GetLX200(":GX80#", TempUniversalTime) == LX200VALUEGET; if (!hasInfoUTC) connected = true; }
    if (updateSeq%3 == 2 || immediate) { hasInfoSidereal = GetLX200(":GS#", TempSidereal) == LX200VALUEGET; if (!hasInfoSidereal) connected = true; lastStateTime = millis(); }
  }
};

void Status::updateTel(boolean immediate) {
  if (((millis() - lastStateTel > BACKGROUND_CMD_RATE) && connected) || immediate)
  {
    if (updateSeq%3 == 0 || immediate) { hasTelStatus = GetLX200(":Gu#", TelStatus) == LX200VALUEGET; if (!hasTelStatus) connected = true; lastStateTel = millis(); }
  }
};

bool Status::getRA(double &RA) {
  char temp[20] = "";
  if (GetLX200(":GR#", temp) == LX200VALUEGET) 
  {
    int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
    double f;
    hmsToDouble(&f, temp);
    if (f >= 0 && f <= 24.0) {
      RA = f*15.0;
      return true;
    }
  }
  return false;
};

bool Status::getDec(double &Dec) {
  char temp[20] = "";
  if (GetLX200(":GD#", temp) == LX200VALUEGET) 
  {
    int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
    double f;
    dmsToDouble(&f, temp, true, true);
    if (f >= -90.0 && f <= 90.0) {
      Dec = f;
      return true;
    }
  }
  return false;
};

double Status::getLstT0() {
  char temp[20] = "";
  double f = 0;
  if (GetLX200(":GS#", temp) == LX200VALUEGET) 
  {
    int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
    hmsToDouble(&f, temp);
  }
  return f;
};

double Status::getLat() {
  char temp[20] = "";
  double f = -10000;
  if (GetLX200(":Gt#", temp) == LX200VALUEGET) {
    int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
    dmsToDouble(&f, temp, true, false);
  }
  return f;
};

int Status::getAlignStars(int *maxStars, int *thisStar, int *numStars) {
  char temp[20] = "";
  if (GetLX200(":A?#", temp) == LX200VALUEGET) {
    int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
    if (l != 4) return false;
    int v = temp[0] - '0'; if (v < 0 || v > 9) return false; *maxStars = v;
    v = temp[1] - '0'; if (v < 0 || v > 9) return false; *thisStar = v;
    v = temp[2] - '0'; if (v < 0 || v > 9) return false; *numStars = v;
  } else return false;
  return true;
}

Status::ParkState Status::getParkState() {
  // Park status: 0 not parked, 1 parking, 2 parked, 3 park failed
  if (strlen(TelStatus) < 6) return PRK_UNKNOW;
  return (ParkState)(TelStatus[5] & 0b00000011);
}

Status::TrackState Status::getTrackingState() {
  if (strlen(TelStatus) < 1) return TRK_UNKNOW;
  bool tracking = false;
  bool slewing = false;
  if (!(TelStatus[0] & 0b00000010)) slewing = true; else tracking = !(TelStatus[0] & 0b00000001);
  if (slewing) return TRK_SLEWING; else
  if (tracking) return TRK_ON; else return TRK_OFF;
}

bool Status::atHome() {
  if (strlen(TelStatus) < 3) return false;
  if (TelStatus[2] & 0b00000001) return true; else return false;
}

bool Status::isPecEnabled() {
  // PEC status: 0 ignore, 1 get ready to play, 2 playing, 3 get ready to record, 4 recording
  if (strlen(TelStatus) < 5) return false;
  if ((TelStatus[4] & 0b00000111) == 0) return false; else return true;
}

bool Status::isPecPlaying() {
  // PEC status: 0 ignore, 1 get ready to play, 2 playing, 3 get ready to record, 4 recording
  if (strlen(TelStatus) < 5) return false;
  if ((TelStatus[4] & 0b00000111) == 2) return true; else return false;
}

bool Status::isPecRecording() {
  if (strlen(TelStatus) < 5) return false;
  if ((TelStatus[4] & 0b00000111) == 4) return true; else return false;
}

bool Status::isPecWaiting() {
  if (strlen(TelStatus) < 5) return false;
  if ((TelStatus[4] & 0b00000111) == 1 || (TelStatus[4] & 0b00000111) == 3) return true; else return false;
}

bool Status::isGuiding() {
  if (strlen(TelStatus) < 1) return false;
  return (TelStatus[0] & 0b00001000);
}

bool Status::isMountGEM() {
  if (strlen(TelStatus)<4) return false;
  return (TelStatus[3] & 0b00000001);
}

bool Status::isMountAltAz() {
  if (strlen(TelStatus) < 4) return false;
  return (TelStatus[3] & 0b00001000);
}

Status::PierState Status::getPierState() {
  if (strlen(TelStatus) < 4) return PIER_UNKNOW;
  if (TelStatus[3] & 0b00010000) return PIER_NONE; else
  if (TelStatus[3] & 0b00100000) return PIER_E; else
  if (TelStatus[3] & 0b01000000) return PIER_W; else return PIER_UNKNOW;
}

Status::TrackRate Status::getTrackingRate() {
  if (strlen(TelStatus) < 2) return TR_UNKNOW;
  return (TrackRate)(TelStatus[1] & 0b00000011);
}

Status::RateCompensation Status::getRateCompensation() {
  RateCompensation rateCompensation = RC_NONE;
  if (TelStatus[0] & 0b00010000) { if (TelStatus[0] & 0b01000000) rateCompensation = RC_REFR_RA; else rateCompensation = RC_REFR_BOTH; } else
  if (TelStatus[0] & 0b00100000) { if (TelStatus[0] & 0b01000000) rateCompensation = RC_FULL_RA; else rateCompensation = RC_FULL_BOTH; } else rateCompensation = RC_NONE;
  return rateCompensation;
}

int Status::getPulseGuideRate() {
  if (strlen(TelStatus) < 7) return -1;
  return TelStatus[6] & 0b00001111;
}

int Status::getGuideRate() {
  if (strlen(TelStatus) < 8) return -1;
  return TelStatus[7] & 0b00001111;
}

Status::Errors Status::getError() {
  if (strlen(TelStatus) < 9) return ERR_NONE;
  return (Errors)(TelStatus[8] & 0b00001111);
}

int Status::getOnStepVersion() {
  static bool processed = false;
  static int version = -1, major, minor;
  static char patch = 0;
  if (!processed) {
    char out[20], ver[20];
    if (GetLX200(":GVN#", out) != LX200VALUEGET || out[0] == 0 ) return -1;
    strcpy(ver, out);
    if (strlen(out) > 0) { patch = out[strlen(out) - 1]; out[strlen(out) - 1] = 0; }
    char *s1 = strchr(out, '.');
    if (s1 != NULL) { s1[0] = 0; s1++; major = atol(out); minor = atol(s1); }
    if (major >= 0 || major <= 99 || minor >= 0 || minor <= 99 || patch >= 'a' || patch <= 'z') {
      processed = true;
      version = major * 100 + minor;
    } else return -1;
  }
  return version;
}

bool Status::hasFocuser(int n) {
  static bool processed = false;
  static bool focuser[6] = {false, false, false, false, false, false};
  if (!processed) {
    char out[20];
    if (getOnStepVersion() < 1000) {
      if ((GetLX200(":FA#", out) == LX200VALUEGET)) focuser[0] = (out[0] == '1');
      if ((GetLX200(":fA#", out) == LX200VALUEGET)) focuser[1] = (out[0] == '1');
    } else {
      if ((GetLX200(":F1a#", out) == LX200VALUEGET)) focuser[0] = (out[0] == '1');
      if ((GetLX200(":F2a#", out) == LX200VALUEGET)) focuser[1] = (out[0] == '1');
      if ((GetLX200(":F3a#", out) == LX200VALUEGET)) focuser[2] = (out[0] == '1');
      if ((GetLX200(":F4a#", out) == LX200VALUEGET)) focuser[3] = (out[0] == '1');
      if ((GetLX200(":F5a#", out) == LX200VALUEGET)) focuser[4] = (out[0] == '1');
      if ((GetLX200(":F6a#", out) == LX200VALUEGET)) focuser[5] = (out[0] == '1');
    }
    for (int i = 0; i < 6; i++) { if (focuser[i]) focuserCount++; }
  }
  return focuser[n];
}

int Status::getFocuserCount() {
  return focuserCount;
}

static int _rotator  = -1;
static int _derotator  = -1;
bool Status::hasRotator() {
  if (_rotator == -1) {
    char out[20];
    if (GetLX200(":GX98#",out) == LX200VALUEGET) {
      _rotator = (out[0] == 'R');
      _derotator = (out[0] == 'D'); if (_derotator) _rotator = true;
    }
  }
  return (_rotator > 0);
}

bool Status::hasDeRotator() {
  if (_rotator == -1) {
    char out[20];
    if (GetLX200(":GX98#",out) == LX200VALUEGET) {
      _rotator = (out[0] == 'R');
      _derotator = (out[0] == 'D'); if (_derotator) _rotator = true;
    }
  }
  return (_derotator > 0);
}

bool Status::hasReticle() {
  return true;
}

bool Status::hasDateTime() {
  static bool dateTimeKnownValid = false;
  if (dateTimeKnownValid) return true; // once OnStep says the date/time has been set no need to keep asking
  bool dateTime = false; // default is to assume the date/time has been set unless OnStep tells us otherwise
  char out[20];
  if ((GetLX200(":GX89#", out) == LX200VALUEGET)) dateTime = (out[0] == '0');
  if (dateTime == true) dateTimeKnownValid = true;
  return dateTime;
}

bool Status::addStar() {
  if (align == ALI_RECENTER_1 || align == ALI_RECENTER_2 || align == ALI_RECENTER_3 || align == ALI_RECENTER_4 || align == ALI_RECENTER_5 || align == ALI_RECENTER_6 || align == ALI_RECENTER_7 || align == ALI_RECENTER_8 || align == ALI_RECENTER_9) {
    if (SetLX200(":A+#") == LX200VALUESET) {
      if (aliMode == ALIM_ONE || 
         (aliMode == ALIM_TWO   && align == ALI_RECENTER_2) ||
         (aliMode == ALIM_THREE && align == ALI_RECENTER_3) ||
         (aliMode == ALIM_FOUR  && align == ALI_RECENTER_4) ||
         (aliMode == ALIM_FIVE  && align == ALI_RECENTER_5) ||
         (aliMode == ALIM_SIX   && align == ALI_RECENTER_6) ||
         (aliMode == ALIM_SEVEN && align == ALI_RECENTER_7) ||
         (aliMode == ALIM_EIGHT && align == ALI_RECENTER_8) ||
         (aliMode == ALIM_NINE  && align == ALI_RECENTER_9)) { align = ALI_OFF; return true; } else { align = static_cast<AlignState>(align+1); return true; }
    } else {
      align = ALI_OFF;
      return false;
    }
  } else {
    align = ALI_OFF;
    return false;
  }
}

bool Status::getT(double &T) {
  char temp[20] = "";
  static double f = 0;
  static bool hasValue = false;
  static unsigned long last = 0;
  if ((millis() - last > 30000) || !hasValue)
    if (GetLX200(":GX9A#", temp) == LX200VALUEGET) 
    {
      int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
      f = atof(temp);
      if (f >= -50.0 && f <= 100.0) { last = millis(); hasValue = true; }
    }
  if (hasValue) { T = f; return true; } else return false;
};

bool Status::getP(double &P) {
  char temp[20] = "";
  static double f = 0;
  static bool hasValue = false;
  static unsigned long last = 0;
  if ((millis() - last > 75000) || !hasValue)
    if (GetLX200(":GX9B#", temp) == LX200VALUEGET) 
    {
      int l=strlen(temp); if (l > 0) temp[l - 1] = 0;
      f = atof(temp);
      if (f >= 500.0 && f <= 2000.0) { last = millis(); hasValue = true; }
    }
  if (hasValue) { P = f; return true; } else return false;
};

bool Status::getH(double &H) {
  char temp[20] = "";
  static double f = 0;
  static bool hasValue = false;
  static unsigned long last = 0;
  if ((millis()-last > 75000) || !hasValue)
    if (GetLX200(":GX9C#", temp) == LX200VALUEGET) 
    {
      int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
      f = atof(temp);
      if (f >= 0.0 && f <= 100.0) { last = millis(); hasValue = true; }
    }
  if (hasValue) { H = f; return true; } else return false;
};

bool Status::getDP(double &DP) {
  char temp[20] = "";
  static double f = 0;
  static bool hasValue = false;
  static unsigned long last = 0;
  if ((millis() - last > 30000) || !hasValue)
    if (GetLX200(":GX9E#", temp) == LX200VALUEGET) 
    {
      int l = strlen(temp); if (l > 0) temp[l - 1] = 0;
      f = atof(temp);
      if (f >= -50.0 && f <= 100.0) { last = millis(); hasValue = true; }
    }
  if (hasValue) { DP = f; return true; } else return false;
};
