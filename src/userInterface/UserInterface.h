// -----------------------------------------------------------------------------------
// UserInterface, handle display and keypad
#pragma once

#include "../Common.h"
#include "../libApp/keyPad/KeyPad.h"
#include "../libApp/u8g2ext/u8g2_ext.h"
#include "../libApp/status/Status.h"
#include "../libApp/cmd/Cmd.h"
#include "message/Message.h"

// coordinate mode for getting and setting RA/Dec
#define OBSERVED_PLACE 1
#define TOPOCENTRIC 2
#define ASTROMETRIC_J2000 3

#define MY_BORDER_SIZE 1
#define icon_width 16
#define icon_height 16

#define onstep_logo_width 128
#define onstep_logo_height 68

enum OLED { OLED_SH1106, OLED_SH1106_4W_SW_SPI, OLED_SH1106_4W_HW_SPI, OLED_SSD1306, OLED_SSD1309, OLED_SSD1309_4W_SW_SPI, OLED_SSD1309_4W_HW_SPI };
#define SH1106 OLED_SH1106
#define SH1106_SW_SPI OLED_SH1106_4W_SW_SPI
#define SH1106_HW_SPI OLED_SH1106_4W_HW_SPI
#define SSD1306 OLED_SSD1306
#define SSD1309 OLED_SSD1309
#define SSD1309_SW_SPI OLED_SSD1309_4W_SW_SPI
#define SSD1309_HW_SPI OLED_SSD1309_4W_HW_SPI

enum MENU_RESULT { MR_OK, MR_CANCEL, MR_QUIT };
enum FocusState {FS_STOPPED, FS_IN_FAST, FS_IN_MID, FS_IN_SLOW, FS_OUT_SLOW, FS_OUT_MID, FS_OUT_FAST};
enum RotState {RS_STOPPED, RS_CW_FAST, RS_CW_MID, RS_CW_SLOW, RS_CCW_SLOW, RS_CCW_MID, RS_CCW_FAST};

#define DisplaySettingsSize 24
typedef struct DisplaySettings {
  int32_t maxContrastSelection;
  int32_t dimTimeoutSelection;
  int32_t blankTimeoutSelection;
  uint8_t unused;
  long blankTimeout;
  long dimTimeout;
} DisplaySettings;

class UI {
public:
  void init(const char version[], const int pin[7], const int active[7], const int SerialBaud, const OLED model);

  void connect();
  void drawIntro();

  void poll();

  int telescopeCoordinates=1;
  #if DISPLAY_24HR_TIME == ON
    boolean hrs24 = true;
  #else
    boolean hrs24 = false;
  #endif

private:
  void updateMainDisplay(u8g2_uint_t page);

  void menuMain();
  void menuFeatureKey();
  #if SERIAL_IP_MODE == STATION
    void menuWifi();
  #endif
  
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
  void menuFeature(uint8_t feature);
  void menuLatitude();
  void menuLongitude();
  void menuZone();
  void menuLimits();
  void menuHorizon();
  void menuOverhead();
  void menuMeridianE();
  void menuMeridianW();
  void menuFirmware();

  Status status;

  U8G2_EXT *display = NULL;

  unsigned long maxT = 0;
  char _version[20] = "Version ?";

  DisplaySettings displaySettings = {DISPLAY_CONTRAST_DEFAULT, 2, 3, 255, 0, 0};

  FocusState focusState = FS_STOPPED;
  int nextFocuserMessageUpdateCycles = 0;
  RotState rotState = RS_STOPPED;
  int nextRotMessageUpdateCycles = 0;

  bool firstConnect = true;
  bool hasAuxFeatures = false;
  bool sleepDisplay = false;
  bool lowContrast = false;
  int Contrast[4] = {1, 66, 192, 255};
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
  unsigned long time_keep_alive = millis();

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

  long serialBaud = 9600;
};

extern UI userInterface;
