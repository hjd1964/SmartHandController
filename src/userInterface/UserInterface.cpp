// -----------------------------------------------------------------------------------
// UserInterface, handle display and keypad

#include "UserInterface.h"
extern NVS nv;
#include "../st4/St4Aux.h"
#include "../catalogs/Catalog.h"
#include "bitmaps/Bitmaps.h"

extern bool connected;

void gethms(const long& v, uint8_t& v1, uint8_t& v2, uint8_t& v3) {
  v3 = v % 60;
  v2 = (v / 60) % 60;
  v1 = v / 3600;
}

/*
void getdms(const long& v, short& v1, uint8_t& v2, uint8_t& v3) {
  v3 = abs(v) % 60;
  v2 = (abs(v) / 60) % 60;
  v1 = v / 3600;
}
*/

void getDegrees(const long& v, short& v1, uint8_t& v2, uint8_t& v3) {
  v3 = abs(v) % 60;
  v2 = (abs(v) / 60) % 60;
  v1 = v / 3600;
}

void longRa2Ra(long Ra, int& h, int& m, int& s) {
  h = Ra / 30;
  m = (Ra - h * 30) / 60;
  s = (Ra / 30) % 60;
}

void longDec2Dec(long Dec, int& deg, int& min) {
  deg = Dec / 60;
  min = Dec % 60;
}

void UI::setup(const char version[], const int pin[7],const bool active[7], const int SerialBaud, const OLED model) {
  // initialize and read defaults from NV as needed
  initNV();
  readNV();

  if (strlen(version) <= 19) strcpy(_version,version);

  status.lastState = 0;
  #if KEYPAD_JOYSTICK_ANALOG == JS1
    pinMode(B_PIN1,INPUT_PULLUP);
    pinMode(B_PIN2,INPUT_PULLUP);
    delay(100);
    int v1=analogRead(B_PIN1);
    int v2=analogRead(B_PIN3);
    buttonPad.setup( pin, active, v1, v2);
  #else
    buttonPad.setup( pin, active, 0, 0);
  #endif

  #if ST4_INTERFACE == ON
    auxST4.setup();
  #endif

  #if UTILITY_LIGHT != OFF
    #ifdef ESP32
      ledcSetup(0, 5000, 8);
      ledcAttachPin(UTILITY_LIGHT_PIN, 0);
      ledcWrite(0, UTILITY_LIGHT);
    #else
      pinMode(UTILITY_LIGHT_PIN, OUTPUT);
      analogWrite(UTILITY_LIGHT_PIN, UTILITY_LIGHT);
    #endif
  #endif
  
  //choose a 128x64 display supported by U8G2lib (if not listed below there are many many others in u8g2 library example Sketches)
  delay(500);
  if (model == OLED_SH1106) display = new U8G2_EXT_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0); else
  if (model == OLED_SSD1306) display = new U8G2_EXT_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0); else
  if (model == OLED_SSD1309) display = new U8G2_EXT_SSD1309_128X64_NONAME_F_HW_I2C(U8G2_R0);
  
  display->begin();
  display->setContrast(maxContrast);
  display->setFont(LF_STANDARD);

  // establish comms and clear the channel
  SERIAL_ONSTEP.begin(SerialBaud);

  delay(500);
  
  // display the splash screen
  drawIntro();
  tickButtons();

  for (int i = 0; i < 3; i++) {
    SERIAL_ONSTEP.print(":#");
    delay(400);
    SERIAL_ONSTEP.flush();
    delay(100);
  }

  DisplayMessage(L_ESTABLISHING, L_CONNECTION, 1000);

  // OnStep establish connection
  char s[20] = "";
  int thisTry = 0;
again1:
  delay(2000);

  VLF(Abv "Attempting to connect");
  LX200RETURN r = GetLX200(":GVP#", s);
  if (r != LX200VALUEGET || (!strstr(s, "On-Step") && !strstr(s, "OnStepX"))) { if (++thisTry <= 8) goto again1; }
  VLF(Abv "Connection established");

again2:
  delay(2000);

  // OnStep coordinate mode for getting and setting RA/Dec
  // 0 = OBSERVED_PLACE (same as not supported)
  // 1 = TOPOCENTRIC (does refraction)
  // 2 = ASTROMETRIC_J2000 (does refraction and precession/nutation)
  thisTry = 0;
  if (GetLX200(":GXEE#", s) == LX200VALUEGET && s[0] >= '0' && s[0] <= '3' && s[1] == 0) {
    if (s[0] == '0') {
      VLF(Abv "Coordinates Observed Place");
      telescopeCoordinates = OBSERVED_PLACE; 
      DisplayMessage(L_CONNECTION, L_OK "!", 1000);
      connected = true;
    } else 
    if (s[0] == '1') {
      VLF(Abv "Coordinates Topocentric");
      telescopeCoordinates = TOPOCENTRIC; 
      DisplayMessage(L_CONNECTION, L_OK "!", 1000);
      connected = true;
    } else 
    if (s[0] == '2') {
      VLF(Abv "Coordinates J2000");
      telescopeCoordinates = ASTROMETRIC_J2000;
      DisplayMessage(L_CONNECTION, L_OK "!", 1000);
      connected = true;
    }
  } else {
    if (++thisTry <= 3) goto again2;
    VLF(Abv "Warning, get coords failed");
    VLF(Abv "fallback to Observed Place");
    telescopeCoordinates = OBSERVED_PLACE;
    DisplayMessage(L_CONNECTION, L_WARNING "!", 1000);
    DisplayMessage(L_COORDINATES, L_OBSERVED_PLACE ".", 2000);
  }
}

void UI::tickButtons() {
  buttonPad.tickButtons();
  #if ST4_INTERFACE == ON
    auxST4.tickButtons();
  #endif
}

void UI::update()
{
  tickButtons();
  unsigned long top = millis();

  // sleep and wake up display
  if (buttonPad.anyPressed()) {
    if (sleepDisplay) { display->setContrast(maxContrast); display->sleepOff(); sleepDisplay = false; lowContrast = false; buttonPad.clearAllPressed(); time_last_action = millis(); }
    if (lowContrast)  { display->setContrast(maxContrast); lowContrast = false; time_last_action = top; }
  } else if (sleepDisplay) return;
  if (display_blank_time && top - time_last_action > display_blank_time) { display->sleepOn(); sleepDisplay = true; return; }
  if (display_dim_time && top - time_last_action > display_dim_time && !lowContrast) { display->setContrast(0); lowContrast = true; return; }

  // show align state
  if (status.align == Status::ALI_SELECT_STAR_1 || status.align == Status::ALI_SELECT_STAR_2 || status.align == Status::ALI_SELECT_STAR_3 ||
      status.align == Status::ALI_SELECT_STAR_4 || status.align == Status::ALI_SELECT_STAR_5 || status.align == Status::ALI_SELECT_STAR_6 ||
      status.align == Status::ALI_SELECT_STAR_7 || status.align == Status::ALI_SELECT_STAR_8 || status.align == Status::ALI_SELECT_STAR_9) {
    char message[10] = L_STAR "# ?";
    if (status.align == Status::ALI_SELECT_STAR_1) strcpy(message, L_STAR " #1");
    if (status.align == Status::ALI_SELECT_STAR_2) strcpy(message, L_STAR " #2");
    if (status.align == Status::ALI_SELECT_STAR_3) strcpy(message, L_STAR " #3");
    if (status.align == Status::ALI_SELECT_STAR_4) strcpy(message, L_STAR " #4");
    if (status.align == Status::ALI_SELECT_STAR_5) strcpy(message, L_STAR " #5");
    if (status.align == Status::ALI_SELECT_STAR_6) strcpy(message, L_STAR " #6");
    if (status.align == Status::ALI_SELECT_STAR_7) strcpy(message, L_STAR " #7");
    if (status.align == Status::ALI_SELECT_STAR_8) strcpy(message, L_STAR " #8");
    if (status.align == Status::ALI_SELECT_STAR_9) strcpy(message, L_STAR " #9");
    DisplayLongMessage(L_ALIGN_MSG1, L_ALIGN_MSG2, "", message, -1);

    // bring up the list of stars so user can goto the alignment star
    if (!SelectStarAlign()) { DisplayMessage(L_ALIGNMENT, L_ABORTED, -1); status.align = Status::ALI_OFF; return; }
    
    // mark this as the next alignment star 
    status.align = static_cast<Status::AlignState>(status.align + 1);
  } else

  // otherwise update the main display
  if (top - lastpageupdate > BACKGROUND_CMD_RATE/2) updateMainDisplay(page);

  // let the user know if the comms are down
  if (status.connected == false) DisplayMessage(L_DISCONNECT_MSG1, L_DISCONNECT_MSG2, -1);

  // -------------------------------------------------------------------------------------------------------------------
  // handle gotos and guiding
  if (status.connected && (status.getTrackingState() == Status::TRK_SLEWING || status.getParkState() == Status::PRK_PARKING)) {
    // gotos
    if (buttonPad.nsewPressed()) {
      SERIAL_ONSTEP.print(":Q#"); SERIAL_ONSTEP.flush();
      if (status.align != Status::ALI_OFF) status.align = static_cast<Status::AlignState>(status.align - 1); // try another align star?
      time_last_action = millis();
      display->sleepOff();
      buttonPad.clearAllPressed();
      DisplayMessage(L_SLEW_MSG1, L_SLEW_MSG2 "!", 1000);
      return;
    }
  } else {
    // guiding
    buttonCommand = false;
    #if ST4_INTERFACE == ON
      if (!moveEast  && (buttonPad.e.isDown() || auxST4.e.isDown())) { moveEast = true;   SERIAL_ONSTEP.write(ccMe); buttonCommand = true; } else
      if (moveEast   && (buttonPad.e.isUp()   && auxST4.e.isUp()))   { moveEast = false;  SERIAL_ONSTEP.write(ccQe); buttonCommand = true; buttonPad.e.clearPress(); auxST4.e.clearPress(); }
      if (!moveWest  && (buttonPad.w.isDown() || auxST4.w.isDown())) { moveWest = true;   SERIAL_ONSTEP.write(ccMw); buttonCommand = true; } else
      if (moveWest   && (buttonPad.w.isUp()   && auxST4.w.isUp()))   { moveWest = false;  SERIAL_ONSTEP.write(ccQw); buttonCommand = true; buttonPad.w.clearPress(); auxST4.w.clearPress(); }
      if (!moveNorth && (buttonPad.n.isDown() || auxST4.n.isDown())) { moveNorth = true;  SERIAL_ONSTEP.write(ccMn); buttonCommand = true; } else
      if (moveNorth  && (buttonPad.n.isUp()   && auxST4.n.isUp()))   { moveNorth = false; SERIAL_ONSTEP.write(ccQn); buttonCommand = true; buttonPad.n.clearPress(); auxST4.n.clearPress(); }
      if (!moveSouth && (buttonPad.s.isDown() || auxST4.s.isDown())) { moveSouth = true;  SERIAL_ONSTEP.write(ccMs); buttonCommand = true; } else
      if (moveSouth  && (buttonPad.s.isUp()   && auxST4.s.isUp()))   { moveSouth = false; SERIAL_ONSTEP.write(ccQs); buttonCommand = true; buttonPad.s.clearPress(); auxST4.s.clearPress(); }
    #else
      if (!moveEast  && (buttonPad.e.isDown())) { moveEast = true;   SERIAL_ONSTEP.write(ccMe); buttonCommand = true; } else
      if (moveEast   && (buttonPad.e.isUp()  )) { moveEast = false;  SERIAL_ONSTEP.write(ccQe); buttonCommand = true; buttonPad.e.clearPress(); }
      if (!moveWest  && (buttonPad.w.isDown())) { moveWest = true;   SERIAL_ONSTEP.write(ccMw); buttonCommand = true; } else
      if (moveWest   && (buttonPad.w.isUp()  )) { moveWest = false;  SERIAL_ONSTEP.write(ccQw); buttonCommand = true; buttonPad.w.clearPress(); }
      if (!moveNorth && (buttonPad.n.isDown())) { moveNorth = true;  SERIAL_ONSTEP.write(ccMn); buttonCommand = true; } else
      if (moveNorth  && (buttonPad.n.isUp()  )) { moveNorth = false; SERIAL_ONSTEP.write(ccQn); buttonCommand = true; buttonPad.n.clearPress(); }
      if (!moveSouth && (buttonPad.s.isDown())) { moveSouth = true;  SERIAL_ONSTEP.write(ccMs); buttonCommand = true; } else
      if (moveSouth  && (buttonPad.s.isUp()  )) { moveSouth = false; SERIAL_ONSTEP.write(ccQs); buttonCommand = true; buttonPad.s.clearPress(); }
    #endif
    if (buttonCommand) { time_last_action = millis(); return; }
  }

  // -------------------------------------------------------------------------------------------------------------------
  // handle the feature buttons
  enum FocusState {FS_STOPPED, FS_IN_FAST, FS_IN_SLOW, FS_OUT_SLOW, FS_OUT_FAST};
  static FocusState focusState = FS_STOPPED;
  enum RotState {RS_STOPPED, RS_CW_FAST, RS_CW_SLOW, RS_CCW_SLOW, RS_CCW_FAST};
  static RotState rotState = RS_STOPPED;
  buttonCommand = false;
  if (status.align != Status::ALI_OFF) featureKeyMode = 1;
  switch (featureKeyMode) {
    case 1:   // guide rate
      if (buttonPad.F.wasPressed()) { activeGuideRate--; strcpy(briefMessage, L_FKEY_GUIDE_DN); buttonCommand = true; } else
      if (buttonPad.f.wasPressed()) { activeGuideRate++; strcpy(briefMessage, L_FKEY_GUIDE_UP); buttonCommand = true; }
      if (buttonCommand) {
        if (activeGuideRate < 4)  activeGuideRate = 4;
        if (activeGuideRate > 10) activeGuideRate = 10;
        char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate - 1;
        DisplayMessageLX200(SetLX200(cmd));
      }
    break;
    case 2:   // pulse guide rate
      if (buttonPad.F.wasPressed()) { activeGuideRate--; strcpy(briefMessage, L_FKEY_PGUIDE_DN); buttonCommand = true; } else
      if (buttonPad.f.wasPressed()) { activeGuideRate++; strcpy(briefMessage, L_FKEY_PGUIDE_UP); buttonCommand = true; }
      if (buttonCommand) {
        if (activeGuideRate < 1) activeGuideRate = 1;
        if (activeGuideRate > 3) activeGuideRate = 3;
        char cmd[5] =  ":Rn#"; cmd[2] = '0' + activeGuideRate - 1;
        DisplayMessageLX200(SetLX200(cmd));
      }
    break;
    case 3:   // util. light
      #if UTILITY_LIGHT != OFF
        if (buttonPad.F.wasPressed()) { current_selection_utility_light--; strcpy(briefMessage, L_FKEY_LAMP_DN); buttonCommand = true; } else
        if (buttonPad.f.wasPressed()) { current_selection_utility_light++; strcpy(briefMessage, L_FKEY_LAMP_UP); buttonCommand = true; }
        if (buttonCommand) {
          if (current_selection_utility_light < 1) current_selection_utility_light = 1;
          if (current_selection_utility_light > 6) current_selection_utility_light = 6;
          int i; switch(current_selection_utility_light) { case 1: i = 0; break; case 2: i = 15; break; case 3: i = 31; break; case 4: i = 63; break; case 5: i = 127; break; case 6: i = 255; break; default: i = 127; break; }
          #ifdef ESP32
            ledcWrite(0, i);
          #else
            analogWrite(UTILITY_LIGHT_PIN, i);
          #endif
        }
      #endif
    break;
    case 4:  // reticule
      if (buttonPad.F.wasPressed()) { SERIAL_ONSTEP.print(":B-#"); strcpy(briefMessage, L_FKEY_RETI_DN); } else
      if (buttonPad.f.wasPressed()) { SERIAL_ONSTEP.print(":B+#"); strcpy(briefMessage, L_FKEY_RETI_UP); }
    break;
    case 5: case 6:  // focuser1/2
      if (focusState == FS_STOPPED && buttonPad.F.isDown()) { focusState = FS_OUT_SLOW; SERIAL_ONSTEP.print(":FS#:F+#"); strcpy(briefMessage,L_FKEY_FOC_DN); buttonCommand = true; }
      else if ((focusState == FS_OUT_SLOW || focusState == FS_OUT_FAST) && buttonPad.F.isUp()) { focusState = FS_STOPPED; SERIAL_ONSTEP.print(":FQ#"); buttonCommand = true; buttonPad.F.clearPress(); }
      else if (focusState == FS_STOPPED && buttonPad.f.isDown()) { focusState = FS_IN_SLOW;  SERIAL_ONSTEP.print(":FS#:F-#"); strcpy(briefMessage,L_FKEY_FOC_UP); buttonCommand = true; }
      else if ((focusState == FS_IN_SLOW || focusState == FS_IN_FAST) && buttonPad.f.isUp()) { focusState = FS_STOPPED; SERIAL_ONSTEP.print(":FQ#"); buttonCommand = true; buttonPad.f.clearPress(); }
      #ifndef FOCUSER_ACCELERATE_DISABLE_ON
        else if (focusState == FS_OUT_SLOW && buttonPad.F.isDown() && buttonPad.F.timeDown() > 5000) { focusState=FS_OUT_FAST; SERIAL_ONSTEP.print(":FF#:F+#"); strcpy(briefMessage, L_FKEY_FOCF_DN); }
        else if (focusState == FS_IN_SLOW  && buttonPad.f.isDown() && buttonPad.f.timeDown() > 5000) { focusState=FS_IN_FAST;  SERIAL_ONSTEP.print(":FF#:F-#"); strcpy(briefMessage, L_FKEY_FOCF_UP); }
      #endif
    break;
    case 7:  // rotator
      if (rotState == RS_STOPPED && buttonPad.F.isDown()) { rotState = RS_CCW_SLOW; SERIAL_ONSTEP.print(":r2#:rc#:r<#"); strcpy(briefMessage, L_FKEY_ROT_DN); buttonCommand = true; }
      else if ((rotState == RS_CCW_SLOW || rotState == RS_CCW_FAST) && buttonPad.F.isUp()) { rotState = RS_STOPPED; SERIAL_ONSTEP.print(":rQ#"); buttonCommand = true; buttonPad.F.clearPress(); }
      else if (rotState == RS_STOPPED && buttonPad.f.isDown()) { rotState = RS_CW_SLOW;  SERIAL_ONSTEP.print(":r2#:rc#:r>#"); strcpy(briefMessage, L_FKEY_ROT_UP); buttonCommand = true; }
      else if ((rotState == RS_CW_SLOW || rotState == RS_CW_FAST) && buttonPad.f.isUp()) { rotState = RS_STOPPED; SERIAL_ONSTEP.print(":rQ#"); buttonCommand = true; buttonPad.f.clearPress(); }
      else if (rotState == RS_CCW_SLOW && buttonPad.F.isDown() && buttonPad.F.timeDown() > 5000) { rotState = RS_CCW_FAST; SERIAL_ONSTEP.print(":r4#:rc#:r<#"); strcpy(briefMessage, L_FKEY_ROTF_DN); }
      else if (rotState == RS_CW_SLOW  && buttonPad.f.isDown() && buttonPad.f.timeDown() > 5000) { rotState = RS_CW_FAST;  SERIAL_ONSTEP.print(":r4#:rc#:r>#"); strcpy(briefMessage, L_FKEY_ROTF_UP); }
    break;
  }
  if (buttonCommand) { time_last_action = millis(); return; }

  // -------------------------------------------------------------------------------------------------------------------
  // handle shift button features
  if (buttonPad.shift.isDown()) {
    // a long press brings up the main menu
    if (buttonPad.shift.timeDown() > 1000 && status.align == Status::ALI_OFF) { menuMain(); time_last_action = millis(); } // bring up the menus
  } else {
    // wait long enough that a double press can happen before picking up other press events
    if (buttonPad.shift.timeUp() > 250) {
      if (buttonPad.shift.wasDoublePressed()) { 
        if (status.align == Status::ALI_OFF) {
          // display feature key menu OR...
          menuFeatureKey();
        } else {
          // ...if aligning, go back and select a different star
          if (status.align == Status::ALI_RECENTER_1 || status.align == Status::ALI_RECENTER_2 || status.align == Status::ALI_RECENTER_3 ||
             status.align == Status::ALI_RECENTER_4 || status.align == Status::ALI_RECENTER_5 || status.align == Status::ALI_RECENTER_6 ||
             status.align == Status::ALI_RECENTER_7 || status.align == Status::ALI_RECENTER_8 || status.align == Status::ALI_RECENTER_9) {
            status.align = static_cast<Status::AlignState>(status.align - 2);
          }
        }
        time_last_action = millis();
      } else
      if (buttonPad.shift.wasPressed()) {
        if (status.align == Status::ALI_OFF) {
          // cycles through disp of Eq, Hor, Time, Ambient OR...
          page++;
          #if DISPLAY_AMBIENT_CONDITIONS == ON
            if (page > 3) page = 0;
          #else
            if (page > 2) page = 0;
          #endif
        } else {
          // ...if aligning, accept the align star
          if ((status.align == Status::ALI_RECENTER_1 || status.align == Status::ALI_RECENTER_2 || status.align == Status::ALI_RECENTER_3 ||
             status.align == Status::ALI_RECENTER_4 || status.align == Status::ALI_RECENTER_5 || status.align == Status::ALI_RECENTER_6 ||
             status.align == Status::ALI_RECENTER_7 || status.align == Status::ALI_RECENTER_8 || status.align == Status::ALI_RECENTER_9)) {
            if (status.addStar()) { if (status.align == Status::ALI_OFF) DisplayMessage(L_ALIGNMENT, L_SUCCESS "!", 2000); else DisplayMessage(L_ADD_STAR, L_SUCCESS "!", 2000); } else DisplayMessage(L_ADD_STAR, L_FAILED "!", -1);
          }
        }
        time_last_action = millis();
      }
    }
  }
}

void UI::updateMainDisplay( u8g2_uint_t page) {
  u8g2_t *u8g2 = display->getU8g2();
  display->setFont(LF_LARGE);
  u8g2_uint_t line_height = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) + MY_BORDER_SIZE;

  // get the status
  status.connected = true;
  status.updateSeq++;
  status.updateTel();
  if (status.connected == false) return;

  // detect align mode
  if (status.hasTelStatus && status.align != Status::ALI_OFF) {
    status.updateTel(true); // really make sure we have the status
    Status::TrackState curT = status.getTrackingState();
    if (curT != Status::TRK_SLEWING && 
       (status.align == Status::ALI_SLEW_STAR_1 || status.align == Status::ALI_SLEW_STAR_2 || status.align == Status::ALI_SLEW_STAR_3 || 
        status.align == Status::ALI_SLEW_STAR_4 || status.align == Status::ALI_SLEW_STAR_5 || status.align == Status::ALI_SLEW_STAR_6 ||
        status.align == Status::ALI_SLEW_STAR_7 || status.align == Status::ALI_SLEW_STAR_8 || status.align == Status::ALI_SLEW_STAR_9)
       ) status.align = static_cast<Status::AlignState > (status.align + 1);
    page = 4;
  }

  // update status info.
  if (page == 0) status.updateRaDec();
  else if (page == 1) status.updateAzAlt();
  else if (page == 2) status.updateTime();

  // prep. brief message, simply place the message in the briefMessage string and it'll show for one second
  static char lastMessage[40] = "";
  static unsigned long startTime = 0;
  if (strlen(briefMessage) != 0) { startTime=millis(); strcpy(lastMessage, briefMessage); strcpy(briefMessage, ""); }
  if (strlen(lastMessage) != 0) {
    if ((long)(millis()-startTime) > 1000) strcpy(lastMessage,"");
  }
 
  // the graphics loop
  u8g2_FirstPage(u8g2);
  do {
    u8g2_uint_t x = u8g2_GetDisplayWidth(u8g2);

    // OnStep status
    if (status.hasTelStatus) { 

      // update guide rate (if available)
      if (status.getGuideRate()>=0) {
        char string_Speed[][8] = {"¼x","½x","1x","2x","4x","8x","20x","48x","½Mx","Max"};
        char string_PSpeed[][6] = {" ¼x"," ½x"," 1x"};
        int gr = status.getGuideRate();
        activeGuideRate = gr + 1;
        int pgr = status.getPulseGuideRate();
        if (pgr != gr && pgr >= 0 && pgr < 3) strcat(string_Speed[gr], string_PSpeed[pgr]); 
        if (gr >= 0 && gr <= 9) {
          display->setFont(LF_STANDARD);
          u8g2_DrawUTF8(u8g2, 0, icon_height, string_Speed[gr]);
          display->setFont(LF_LARGE);
        }
      }

      Status::ParkState curP = status.getParkState();
      Status::TrackState curT = status.getTrackingState();
      Status::TrackRate curTR = status.getTrackingRate();
    
      if (curP == Status::PRK_PARKED)  { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parked_bits); x -= icon_width + 1; } else
      if (curP == Status::PRK_PARKING) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parking_bits); x -= icon_width + 1; } else
      if (status.atHome())               { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, home_bits); x -= icon_width + 1;  } else 
      {
        if (curT == Status::TRK_SLEWING) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, sleewing_bits); x -= icon_width + 1; } else
        if (curT == Status::TRK_ON) {
          if (curTR == Status::TR_SIDEREAL) {
            switch (status.getRateCompensation()) {
              case Status::RC_NONE:      display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sid_bits);    x -= icon_width + 1; break;
              case Status::RC_REFR_RA:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sid_r_bits);  x -= icon_width + 1; break;
              case Status::RC_REFR_BOTH: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sid_rd_bits); x -= icon_width + 1; break;
              case Status::RC_FULL_RA:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sid_f_bits);  x -= icon_width + 1; break;
              case Status::RC_FULL_BOTH: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sid_fd_bits); x -= icon_width + 1; break;
              default: break;
            }
          } else
          if (curTR == Status::TR_LUNAR) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_lun_bits); x -= icon_width + 1; } else
          if (curTR == Status::TR_SOLAR) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sol_bits); x -= icon_width + 1; } else
                                         { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_S_bits); x -= icon_width + 1; }
        } else
        if (curT == Status::TRK_OFF) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, no_tracking_bits); x -= icon_width + 1; }

        if (curP == Status::PRK_FAILED) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parkingFailed_bits); x -= icon_width + 1; }

        Status::PierState CurP = status.getPierState();
        if (CurP == Status::PIER_E) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, E_bits); x -= icon_width + 1; } else 
        if (CurP == Status::PIER_W) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, W_bits); x -= icon_width + 1; }

        if (status.align != Status::ALI_OFF) {
          switch (status.aliMode) {
            case Status::ALIM_ONE:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align1_bits); x -= icon_width + 1; break;
            case Status::ALIM_TWO:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align2_bits); x -= icon_width + 1; break;
            case Status::ALIM_THREE: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align3_bits); x -= icon_width + 1; break;
            case Status::ALIM_FOUR:  display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align4_bits); x -= icon_width + 1; break;
            case Status::ALIM_FIVE:  display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align5_bits); x -= icon_width + 1; break;
            case Status::ALIM_SIX:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align6_bits); x -= icon_width + 1; break;
            case Status::ALIM_SEVEN: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align7_bits); x -= icon_width + 1; break;
            case Status::ALIM_EIGHT: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align8_bits); x -= icon_width + 1; break;
            case Status::ALIM_NINE:  display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align9_bits); x -= icon_width + 1; break;
            default: break;
          }
        }

        if (status.isPecPlaying())   { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, pec_play_bits);   x -= icon_width + 1; } else
        if (status.isPecRecording()) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, pec_record_bits); x -= icon_width + 1; } else
        if (status.isPecWaiting())   { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, pec_wait_bits);   x -= icon_width + 1; }

        if (status.isGuiding()) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_bits); x -= icon_width + 1; }
      }

      if (!status.isGuiding()) {
        switch (status.getError()) {
          case Status::ERR_NONE: break;                                                                                                        // no error
          case Status::ERR_MOTOR_FAULT: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMf_bits); x -= icon_width + 1; break; // motor fault
          case Status::ERR_ALT_MIN:     display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAlt_bits);x -= icon_width + 1; break; // above below horizon
          case Status::ERR_LIMIT_SENSE: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrLs_bits); x -= icon_width + 1; break; // physical limit switch triggered
          case Status::ERR_DEC:         display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrDe_bits); x -= icon_width + 1; break; // past the rarely used Dec limit
          case Status::ERR_AZM:         display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAz_bits); x -= icon_width + 1; break; // for AltAz mounts, past limit in Az
          case Status::ERR_UNDER_POLE:  display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrUp_bits); x -= icon_width + 1; break; // for Eq mounts, past limit in HA
          case Status::ERR_MERIDIAN:    display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMe_bits); x -= icon_width + 1; break; // for Eq mounts, past meridian limit
          case Status::ERR_ALT_MAX:     display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAlt_bits);x -= icon_width + 1; break; // above overhead
          default:                      display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrOth_bits);x -= icon_width + 1; break; // other error
        }
      }
    }

    // show brief message
    if (strlen(lastMessage) != 0) {
      x = u8g2_GetDisplayWidth(u8g2);  u8g2_uint_t y = 36;  u8g2_uint_t x1 = u8g2_GetStrWidth(u8g2, lastMessage);
      u8g2_DrawUTF8(u8g2, (x/2) - (x1/2), y + 8, lastMessage);
    } else

    // show equatorial coordinates
    if (page == 0) {
      if (status.hasInfoRa && status.hasInfoDec) {
        char rs[20]; strcpy(rs, status.TempRa);
        int l = strlen(rs);
        if (l > 1) rs[l - 1] = 0;
        u8g2_uint_t x = u8g2_GetDisplayWidth(u8g2) - u8g2_GetUTF8Width(u8g2, "00000000");
        u8g2_uint_t y = 36;

        u8g2_DrawUTF8(u8g2, 0, y, L_RA);
        display->DrawFwNumeric(x, y, rs);

        char ds[20]; strcpy(ds, status.TempDec);
        l = strlen(ds);
        if (l > 1) ds[l - 1] = 0;
        if (l > 8) { ds[3] = '\xb0'; ds[6] = '\''; }
        x = u8g2_GetDisplayWidth(u8g2) - u8g2_GetUTF8Width(u8g2, "000000000");
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, L_DEC); 
        display->DrawFwNumeric(x, y, ds);
      }
    } else

    // show horizon coordinates
    if (page == 1) {
      if (status.hasInfoAz && status.hasInfoAlt)
      {
        char zs[20]; strcpy(zs, status.TempAz);
        int l = strlen(zs);
        if (l > 1) zs[l - 1] = 0;
        if (l > 8) { zs[3] = '\xb0'; zs[6] = '\''; }
        x = u8g2_GetDisplayWidth(u8g2) - u8g2_GetUTF8Width(u8g2, "000000000");
        u8g2_uint_t y = 36;
        u8g2_DrawUTF8(u8g2, 0, y, L_AZ);
        display->DrawFwNumeric(x, y, zs);

        char as[20]; strcpy(as, status.TempAlt);
        l = strlen(as);
        if (l > 1) as[l - 1] = 0;
        if (l > 8) { as[3] = '\xb0'; as[6] = '\''; }
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, L_ALT);
        display->DrawFwNumeric(x, y, as);
      }
    } else
    
    // show time
    if (page == 2) {
      if (status.hasInfoUTC && status.hasInfoSidereal) {
        char us[20]; strcpy(us, status.TempUniversalTime);
        int l = strlen(us);
        if (l > 1) us[l - 1] = 0;
        x = u8g2_GetDisplayWidth(u8g2) - u8g2_GetUTF8Width(u8g2, "00000000");
        u8g2_uint_t y = 36;
        display->setFont(LF_STANDARD); u8g2_DrawUTF8(u8g2, 0, y, "UT"); display->setFont(LF_LARGE);
        display->DrawFwNumeric(x, y, us);

        char ss[20]; strcpy(ss,status.TempSidereal);
        l = strlen(ss);
        if (l > 1) ss[l - 1] = 0;
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, "LST");
        display->DrawFwNumeric(x, y, ss);
      }
    } else

    if (page == 3) {
      // T24.6 P997mb
      // H46% DP13.7C
      display->setFont(LF_STANDARD);

      double T, P, H, DP;
      if (status.getT(T) && status.getP(P) && status.getH(H) && status.getDP(DP)) {
        char temp[20], line[20];
        u8g2_uint_t y = 36;
        u8g2_uint_t dx = u8g2_GetDisplayWidth(u8g2);

        dtostrf(T, 3, 1, temp);
        sprintf(line, "T%s\xb0%s", temp, "C");
        display->DrawFwNumeric(0, y, line);

        sprintf(line, "P%dmb",(int)round(P));
        display->DrawFwNumeric(dx - display->GetFwNumericWidth(line), y, line);

        y += line_height + 4;
        sprintf(line, "H%d%%", (int)round(H));
        display->DrawFwNumeric(0, y, line);

        dtostrf(DP, 3, 1, temp);
        sprintf(line, "DP%s\xb0%s", temp, "C");
        display->DrawFwNumeric(dx-display->GetFwNumericWidth(line), y, line);
      }
      
      display->setFont(LF_LARGE);
    } else

    // show align status
    if (page == 4) {
      u8g2_uint_t y = 36;

      char txt[20];
      if ((status.align - 1) % 3 == 1) sprintf(txt, L_SLEWING_TO_STAR " %u", (status.align - 1) / 3 + 1); else
      if ((status.align - 1) % 3 == 2) sprintf(txt, L_RECENTER_STAR " %u", (status.align - 1) / 3 + 1);
      u8g2_DrawUTF8(u8g2, 0, y, txt);

      y += line_height + 4;
      u8g2_SetFont(u8g2, LF_GREEK);
      u8g2_DrawGlyph(u8g2, 0, y, 945 + cat_mgr.bayerFlam());

      const uint8_t* myfont = u8g2->font; u8g2_SetFont(u8g2, myfont);
      u8g2_DrawUTF8(u8g2, 16, y, cat_mgr.constellationStr());
    }
    
  } while (u8g2_NextPage(u8g2));
  lastpageupdate = millis();
}

void UI::drawIntro() {
  display->firstPage();
  do {
    display->drawXBMP(0, 0, onstep_logo_width, onstep_logo_height, onstep_logo_bits);
  } while (display->nextPage());
  delay(1000);
}

// misc.
bool UI::SelectStarAlign() {
  cat_mgr.setLat(status.getLat()); cat_mgr.setLstT0(status.getLstT0());
  cat_mgr.select(0);

  cat_mgr.filtersClear();
  cat_mgr.filterAdd(FM_ALIGN_ALL_SKY);

  cat_mgr.setIndex(0);
  if (cat_mgr.isInitialized()) {
    if (display->UserInterfaceCatalog(&buttonPad, L_SELECT_STAR)) {
      bool ok = DisplayMessageLX200(SyncSelectedStarLX200(cat_mgr.getIndex()),false);
      return ok;
    }
  }
  return false;
}

void UI::DisplayMessage(const char* txt1, const char* txt2, int duration) {
  uint8_t x;
  uint8_t y = 40;
  display->firstPage();
  do {
    if (txt2 != NULL)
    {
      y = 50;
      x = (display->getDisplayWidth() - display->getStrWidth(txt2)) / 2;
      display->drawStr(x, y, txt2);
      y = 25;
    }
    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
  } while (display->nextPage());
  if (duration >= 0) delay(duration); else { buttonPad.waitForPress(); buttonPad.clearAllPressed(); }
}

void UI::DisplayLongMessage(const char* txt1, const char* txt2, const char* txt3, const char* txt4, int duration) {
  display->setFont(LF_STANDARD);
  uint8_t h = 15;
  uint8_t x = 0;
  uint8_t y = h;
  display->firstPage();
  do {
    y = h;
    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
    y += h;
    if (txt2 != NULL) {
      x = 0;
      display->drawStr(x, y, txt2);
    } else {
      y -= 7;
    }
    y += 15;
    if (txt3 != NULL) {
      x = 0;
      display->drawStr(x, y, txt3);
    }

    y += 15;
    if (txt4 != NULL) {
      x = 0;
      display->drawStr(x, y, txt4);
    }
  } while (display->nextPage());
  if (duration >= 0) delay(duration); else { buttonPad.waitForPress(); buttonPad.clearAllPressed(); }
  display->setFont(LF_LARGE);
}

bool UI::DisplayMessageLX200(LX200RETURN val, bool silentOk) {
  char text1[20] = "";
  char text2[20] = "";
  int time = -1;
  if (val < LX200OK) {
         if (val == LX200NOTOK)                    { sprintf(text1, L_LX200_NOTOK_1); sprintf(text2, L_LX200_NOTOK_2);  }
    else if (val == LX200SETVALUEFAILED)           { sprintf(text1, L_LX200_SETVF_1); sprintf(text2, L_LX200_SETVF_2);  }
    else if (val == LX200GETVALUEFAILED)           { sprintf(text1, L_LX200_GETVF_1); sprintf(text2, L_LX200_GETVF_2);  }
    else if (val == LX200SETTARGETFAILED)          { sprintf(text1, L_LX200_SETTG_1); sprintf(text2, L_LX200_SETTG_2);  }
    else if (val == LX200NOOBJECTSELECTED)         { sprintf(text1, L_LX200_OBJSE_1); sprintf(text2, L_LX200_OBJSE_2);  }
    else if (val == LX200_GOTO_ERR_BELOW_HORIZON)  { sprintf(text1, L_LX200_TGHOR_1); sprintf(text2, L_LX200_TGHOR_2);  }
    else if (val == LX200_GOTO_ERR_ABOVE_OVERHEAD) { sprintf(text1, L_LX200_TGOVH_1); sprintf(text2, L_LX200_TGOVH_2);  }
    else if (val == LX200_GOTO_ERR_STANDBY)        { sprintf(text1, L_LX200_STNBF_1); sprintf(text2, L_LX200_STNBF_2);  }
    else if (val == LX200_GOTO_ERR_PARK)           { sprintf(text1, L_LX200_PARKF_1); sprintf(text2, L_LX200_PARKF_2);  }
    else if (val == LX200_GOTO_ERR_GOTO)           { sprintf(text1, L_LX200_GOGOF_1); sprintf(text2, L_LX200_GOGOF_2);  }
    else if (val == LX200_GOTO_ERR_OUTSIDE_LIMITS) { sprintf(text1, L_LX200_LIMTF_1); sprintf(text2, L_LX200_LIMTF_2);  }
    else if (val == LX200_GOTO_ERR_HARDWARE_FAULT) { sprintf(text1, L_LX200_HARDF_1); sprintf(text2, L_LX200_HARDF_2);  }
    else if (val == LX200_GOTO_ERR_IN_MOTION)      { sprintf(text1, L_LX200_GOMOF_1); sprintf(text2, L_LX200_GOMOF_2);  }
    else if (val == LX200_GOTO_ERR_UNSPECIFIED)    { sprintf(text1, L_LX200_UNSPF_1); sprintf(text2, L_LX200_UNSPF_2);  }
    else { sprintf(text1, L_LX200_ERROR); sprintf(text2, "-1"); }
    DisplayMessage(text1, text2, -1);
  } else if (!silentOk) {
    time = 1000;
         if (val == LX200OK)            { sprintf(text1, L_LX200_ISAOK_1); sprintf(text2, L_LX200_ISAOK_2);  }
    else if (val == LX200VALUESET)      { sprintf(text1, L_LX200_SETOK_1); sprintf(text2, L_LX200_SETOK_2);  }
    else if (val == LX200VALUEGET)      { sprintf(text1, L_LX200_GETOK_1); sprintf(text2, L_LX200_GETOK_2);  }
    else if (val == LX200SYNCED)        { sprintf(text1, L_LX200_SNCOK_1); sprintf(text2, L_LX200_SNCOK_2);  }
    else if (val == LX200_GOTO_GOINGTO) { sprintf(text1, L_LX200_GOTOK_1); sprintf(text2, L_LX200_GOTOK_2);  }
    DisplayMessage(text1, text2, time);
  }
  return isOk(val);
}
