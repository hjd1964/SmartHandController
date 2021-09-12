// -----------------------------------------------------------------------------------
// UserInterface, handle display and keypad
#pragma once

#include "../Common.h"
#include "../buttons/Pad.h"
#include "../u8g2ext/u8g2_ext.h"
#include "../status/Status.h"
#include "../commands/LX200.h"

#define SH1106 0
#define SSD1306 1
#define SSD1309 2

// coordinate mode for getting and setting RA/Dec
#define OBSERVED_PLACE 1
#define TOPOCENTRIC 2
#define ASTROMETRIC_J2000 3

#define MY_BORDER_SIZE 1
#define icon_width 16
#define icon_height 16

#define onstep_logo_width 128
#define onstep_logo_height 68

enum MENU_RESULT { MR_OK, MR_CANCEL, MR_QUIT };

class UI {
public:
  enum OLED { OLED_SH1106, OLED_SSD1306, OLED_SSD1309 };
  int telescopeCoordinates=1;
  #if DISPLAY_24HR_TIME == ON
    boolean hrs24 = true;
  #else
    boolean hrs24 = false;
  #endif
  void update();
  void drawIntro();
  void setup(const char version[], const int pin[7], const bool active[7], const int SerialBaud, const OLED model);
  void tickButtons();
private:
  unsigned long maxT=0;

  Pad buttonPad;
  U8G2_EXT *display = NULL;

  Status status;
  char _version[20] = "Version ?";
  char briefMessage[40] = "";

  void updateMainDisplay( u8g2_uint_t page);
  bool sleepDisplay = false;
  bool lowContrast = false;
  uint8_t maxContrast = 255;
  int Contrast[4] = {1, 66, 192, 255};
  unsigned long display_blank_time;
  unsigned long display_dim_time;
  bool buttonCommand = false;
  bool moveNorth = false;
  bool moveSouth = false;
  bool moveEast = false;
  bool moveWest = false;

  // Guide commands
  char ccMe[5];
  char ccMw[5];
  char ccMn[5];
  char ccMs[5];
  char ccQe[5];
  char ccQw[5];
  char ccQn[5];
  char ccQs[5];

  unsigned long lastpageupdate = millis();
  unsigned long time_last_action = millis();

  byte page = 0;
  uint8_t current_selection_L0 = 1;
  uint8_t current_selection_L1 = 1;
  uint8_t current_selection_L2 = 1;
  uint8_t current_selection_L3 = 1;
  uint8_t current_selection_L4 = 1;
  #if UTILITY_LIGHT != OFF
    uint8_t current_selection_utility_light = 5;
  #endif
  bool    current_selection_filter_above = true;
  uint8_t current_selection_filter_con = 1;
  uint8_t current_selection_filter_type = 1;
  uint8_t current_selection_filter_byMag = 1;
  uint8_t current_selection_filter_nearby = 1;
  uint8_t current_selection_filter_dblmin = 1;
  uint8_t current_selection_filter_dblmax = 1;
  uint8_t current_selection_filter_varmax = 1;
  uint8_t activeGuideRate = 5;
  uint8_t featureKeyMode = 1; // guide rate

  long angleRA = 0;
  long angleDEC = 0;

  void initNV();
  void readNV();

  void menuMain();
  void menuFeatureKey();
  
  MENU_RESULT menuSyncGoto(bool sync);
  MENU_RESULT subMenuSyncGoto(char sync, int subMenuNum);
  MENU_RESULT menuCatalog(bool sync, int number);
  MENU_RESULT menuSolarSys(bool sync);
  MENU_RESULT menuFilters();
  void setCatMgrFilters();
  MENU_RESULT menuFilterCon();
  MENU_RESULT menuFilterType();
  MENU_RESULT menuFilterByMag();
  MENU_RESULT menuFilterNearby();
  MENU_RESULT menuFilterDblMinSep();
  MENU_RESULT menuFilterDblMaxSep();
  MENU_RESULT menuFilterVarMaxPer();
  MENU_RESULT menuUser(bool sync);
  MENU_RESULT menuRADec(bool sync);
  
  void menuAlignment();
  void menuParking();
  void menuPEC();
  void menuGotoSpeed();
  void menuBacklash();
  bool menuSetBacklash(uint8_t &axis);
  void menuPier();
  bool SelectStarAlign();
  void menuSettings();
  void menuMount();
  void menuSite();
  void menuSites();
  void menuLocalDateTime();
  void menuDisplay();
  void menuSound();
  void menuMeridianFlips();
  void menuTracking();
  void menuContrast();
  void menuDimTimeout();
  void menuBlankTimeout();
  void menuFocuser(uint8_t foc);
  bool menuSetFocTCCoef(uint8_t &foc);
  bool menuSetFocBacklash(uint8_t &foc);
  bool menuSetFocTCDeadband(uint8_t &foc);
  void menuRotator();
  void menuLatitude();
  void menuLongitude();
  void menuZone();
  void menuLimits();
  void menuHorizon();
  void menuOverhead();
  void menuMeridianE();
  void menuMeridianW();
  void menuFirmware();

  void DisplayMessage(const char* txt1, const char* txt2 = NULL, int duration = 0);
  void DisplayLongMessage(const char* txt1, const char* txt2 = NULL, const char* txt3 = NULL, const char* txt4 = NULL, int duration = 0);
  bool DisplayMessageLX200(LX200RETURN val, bool silentOk = true);
};

extern UI userInterface;
