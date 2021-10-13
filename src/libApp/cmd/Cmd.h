// -----------------------------------------------------------------------------------
// LX200 command processing
#pragma once

#include "../../Common.h"

enum CMD_RESULT {
  CR_NOT_OK, CR_SET_VALUE_FAILED, CR_GET_VALUE_FAILED, CR_SET_TARGET_FAILED, CR_NO_OBJECT_SELECTED, CR_UNKOWN,
  
  CR_GOTO_ERR_BELOW_HORIZON, CR_GOTO_ERR_ABOVE_OVERHEAD, CR_GOTO_ERR_STANDBY, CR_GOTO_ERR_PARK, CR_GOTO_ERR_GOTO, 
  CR_GOTO_ERR_OUTSIDE_LIMITS, CR_GOTO_ERR_HARDWARE_FAULT, CR_GOTO_ERR_IN_MOTION, CR_GOTO_ERR_UNSPECIFIED,
  
  CR_OK, CR_VALUE_SET, CR_VALUE_GET, CR_SYNCED, CR_GOTO_GOING_TO
};

class OnStepCmd {
  public:
    bool isOk(CMD_RESULT val);
    CMD_RESULT Get(char* command, char* output);
    // overloaded to allow const char* strings without compiler warnings, similar follow below
    CMD_RESULT Get(const char* command, char* output);
    CMD_RESULT GetTrim(char* command, char* output);
    CMD_RESULT GetTrim(const char* command, char* output);
    CMD_RESULT GetTime(unsigned int &hour, unsigned int &minute, unsigned int &second, boolean ut=false);
    CMD_RESULT GetTime(long &value, boolean ut=false);
    CMD_RESULT Set(char* command);
    CMD_RESULT Set(const char* command);
    CMD_RESULT SetTime(long &value);
    CMD_RESULT GetSite(int &value);
    CMD_RESULT SetSite(int &value);
    CMD_RESULT Move2Target();
    CMD_RESULT SetTargetRa(int vr1, int vr2, int vr3);
    CMD_RESULT SetTargetDec(char sign, int vd1, int vd2, int vd3);
    CMD_RESULT SyncGoto(bool, float &Ra, float &Dec);
    CMD_RESULT SyncSelectedStar(long alignSelectedStar);
    CMD_RESULT GetDate(unsigned int &day, unsigned int &month, unsigned int &year, boolean ut=false);
    CMD_RESULT SyncGotoCat(bool sync);
    CMD_RESULT SyncGotoPlanet(bool sync, unsigned short obj);
    CMD_RESULT readBacklash(const uint8_t &axis, float &backlash);
    CMD_RESULT writeBacklash(const uint8_t &axis, const float &backlash);
    CMD_RESULT readFocTCCoef(const uint8_t &foc, float &tccoef);
    CMD_RESULT writeFocTCCoef(const uint8_t &foc, const float &tccoef);
    CMD_RESULT readFocBacklash(const uint8_t &foc, float &tccoef);
    CMD_RESULT writeFocBacklash(const uint8_t &foc, const float &tccoef);
    CMD_RESULT readFocTCDeadband(const uint8_t &foc, float &tccoef);
    CMD_RESULT writeFocTCDeadband(const uint8_t &foc, const float &tccoef);

  private:
    CMD_RESULT GetLatitude(int &degree, int &minute, int &second);
    CMD_RESULT GetLongitude(int &degree, int &minute, int &second);
    void serialRecvFlush();
    void char2RA(char* txt, unsigned int& hour, unsigned int& minute, unsigned int& second);
    void char2DEC(char* txt, int& deg, unsigned int& min, unsigned int& sec);
    bool processCommand(char* command, char* response, unsigned long timeOutMs);

};

extern OnStepCmd onStep;
