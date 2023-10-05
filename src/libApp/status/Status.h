// -----------------------------------------------------------------------------------
// Status, from OnStep
#pragma once

#include "../../Common.h"

class Status {
public:
  enum Errors {
    ERR_NONE, ERR_MOTOR_FAULT, ERR_ALT_MIN, ERR_LIMIT_SENSE, ERR_DEC, ERR_AZM, ERR_UNDER_POLE, ERR_MERIDIAN, ERR_SYNC,
    ERR_PARK, ERR_GOTO_SYNC, ERR_UNSPECIFIED, ERR_ALT_MAX, ERR_WEATHER_INIT, ERR_SITE_INIT
  };

  enum AlignMode {ALIM_OFF, ALIM_ONE, ALIM_TWO, ALIM_THREE, ALIM_FOUR, ALIM_FIVE, ALIM_SIX, ALIM_SEVEN, ALIM_EIGHT, ALIM_NINE};
  enum AlignState {
    ALI_OFF,
    ALI_SELECT_STAR_1, ALI_SLEW_STAR_1, ALI_RECENTER_1,
    ALI_SELECT_STAR_2, ALI_SLEW_STAR_2, ALI_RECENTER_2,
    ALI_SELECT_STAR_3, ALI_SLEW_STAR_3, ALI_RECENTER_3,
    ALI_SELECT_STAR_4, ALI_SLEW_STAR_4, ALI_RECENTER_4,
    ALI_SELECT_STAR_5, ALI_SLEW_STAR_5, ALI_RECENTER_5,
    ALI_SELECT_STAR_6, ALI_SLEW_STAR_6, ALI_RECENTER_6,
    ALI_SELECT_STAR_7, ALI_SLEW_STAR_7, ALI_RECENTER_7,
    ALI_SELECT_STAR_8, ALI_SLEW_STAR_8, ALI_RECENTER_8,
    ALI_SELECT_STAR_9, ALI_SLEW_STAR_9, ALI_RECENTER_9
  };

  enum Mount { GEM, FEM };
  enum TrackState { TRK_OFF, TRK_ON, TRK_SLEWING, TRK_UNKNOW };
  enum TrackRate { TR_SIDEREAL, TR_LUNAR, TR_SOLAR, TR_KING, TR_UNKNOW };
  enum RateCompensation {RC_NONE, RC_REFR_RA, RC_REFR_BOTH, RC_FULL_RA, RC_FULL_BOTH};
  enum ParkState { PRK_UNPARKED, PRK_PARKING, PRK_PARKED, PRK_FAILED, PRK_UNKNOW };
  enum PierState { PIER_NONE, PIER_E, PIER_W, PIER_UNKNOW };

public:
  void updateRaDec(bool immediate = false);
  void updateAzAlt(bool immediate = false);
  void updateTime(bool immediate = false);
  void updateTel(bool immediate = false);
  bool getRA(double &RA);
  bool getDec(double &Dec);
  double getLstT0();
  double getLat();
  int getAlignStars(int *maxStars, int *thisStar, int *numStars);
  ParkState getParkState();
  TrackState getTrackingState();
  bool atHome();
  bool isPecEnabled();
  bool isPecPlaying();
  bool isPecRecording();
  bool isPecWaiting();
  bool isGuiding();
  bool isMountGEM();
  bool isMountAltAz();
  bool getT(double &T);
  bool getP(double &P);
  bool getH(double &H);
  bool getDP(double &DP);
  PierState getPierState();
  TrackRate getTrackingRate();
  RateCompensation getRateCompensation();
  int getGuideRate();
  int getPulseGuideRate();
  Errors getError();
  int getOnStepVersion();
  bool hasFocuser(int n);
  int getFocuserCount();
  float getFocuserPosition();
  bool hasRotator();
  bool hasDeRotator();
  float getRotatorPosition();
  bool hasReticle();
  bool hasDateTime();

  bool alignAddStar();
  bool alignSelectStar();
  bool alignSlewStar();
  bool alignRecenterStar();

  AlignState      align = ALI_OFF;
  AlignMode       aliMode = ALIM_ONE;
  unsigned short  alignSelectedStar = 1;
  int             alignMaxNumStars = -1;

  bool featureScan();
  bool featureUpdate(int index = 0);
  void featureClearAll();
  inline bool featureFound() { return _featureFound; }
  // select auxiliary feature
  // \param index aux feature# 1 to 8
  inline void featureSelect(int index) { featureSelected = index - 1; }
  bool featureSelectByOrder(int order);
  inline int featureNumber() { return featureSelected + 1; }
  inline char* featureName() { return feature[featureSelected].name; }
  inline int featurePurpose() { return feature[featureSelected].purpose; }
  inline int featureValue1() { return feature[featureSelected].value1; }
  inline float featureValue2() { return feature[featureSelected].value2; }
  inline float featureValue3() { return feature[featureSelected].value3; }
  inline float featureValue4() { return feature[featureSelected].value4; }

  bool _featureFound = false;
  bool _featureValid = false;
  int featureSelected = 0;
  typedef struct Features {
    char name[11];
    int purpose;
    int value1;
    float value2;
    float value3;
    float value4;
  } features;
  features feature[8];

  Errors lastError = Status::ERR_NONE;

  char TempRa[20];
  char TempDec[20];
  unsigned long lastStateRaDec;
  char TempAz[20];
  char TempAlt[20];
  unsigned long lastStateAzAlt;
  char TempUniversalTime[20];
  char TempSidereal[20];
  unsigned long lastStateTime;

  char TelStatus[20];
  unsigned long lastStateTel;
  unsigned long updateSeq=0;

  bool connected = false;
  bool hasInfoRa = false;
  bool hasInfoDec = false;
  bool hasInfoAz = false;
  bool hasInfoAlt = false;
  bool hasInfoUTC = false;
  bool hasInfoSidereal = false;
  bool hasTelStatus = false;
  unsigned long lastState;

  int backgroundCommandRate = FOREGROUND_CMD_RATE;

private:
  int focuserCount = 0;
  int switchCount = 0;
};
