// -----------------------------------------------------------------------------------
// UserInterface, handle display and keypad

#include "UserInterface.h"

#include "../lib/tasks/OnTask.h"
#include "../lib/nv/Nv.h"

#include "../libApp/st4Aux/St4Aux.h"

#include "../catalogs/Catalog.h"
#include "bitmaps/Bitmaps.h"

bool xBusy = false;

void updateWrapper() { userInterface.poll(); }
void keyPadWrapper() { keyPad.poll(); }
#if ST4_AUX_INTERFACE == ON
  void auxST4Wrapper() { auxST4.poll(); }
#endif

void UI::init(const char version[], const KeyPad::Pin pins[7], const int SerialBaud, const OLED model) {
  serialBaud = SerialBaud;

  // get nv ready
  if (!nv.isKeyValid(INIT_NV_KEY)) {
    VF("MSG: NV, invalid key wipe "); V(nv.size); VLF(" bytes");
    if (nv.verify()) { VLF("MSG: NV, ready for reset to defaults"); }
  } else { VLF("MSG: NV, correct key found"); }

  // get wireless ready
  #if SERIAL_IP_MODE != OFF
    wifiManager.readSettings();
  #endif
  #if SERIAL_BT_MODE != OFF
  bluetoothManager.readSettings();
  #endif

  // confirm the data structure size
  if (DisplaySettingsSize < sizeof(DisplaySettings)) { nv.initError = true; DL("ERR: UserInterface::setup(); DisplaySettingsSize error NV subsystem writes disabled"); }

  // write the default settings to NV
  if (!nv.hasValidKey()) {
    VLF("MSG: UserInterface, writing defaults to NV");
    nv.writeBytes(NV_DISPLAY_SETTINGS_BASE, &displaySettings, sizeof(DisplaySettings));
  }

  // read the settings
  nv.readBytes(NV_DISPLAY_SETTINGS_BASE, &displaySettings, sizeof(DisplaySettings));

  // init is done, write the NV key if necessary
  if (!nv.hasValidKey()) {
    nv.writeKey((uint32_t)INIT_NV_KEY);
    nv.wait();
    if (!nv.isKeyValid(INIT_NV_KEY)) { DLF("ERR: NV, failed to read back key!"); } else { VLF("MSG: NV, reset complete"); }
  }

  connectionSelection = (ConnectSelection)nv.readUC(NV_SERIAL_BOOT_FLAG_BASE);
  if (connectionSelection != CS_NONE) {
    VF("MSG: UserInterface, direct boot flag set to "); VL((int)connectionSelection);
    nv.write(NV_SERIAL_BOOT_FLAG_BASE, (uint8_t)CS_NONE);
  }

  if (strlen(version) <= 19) strcpy(_version, version);

  status.lastState = 0;
  #if KEYPAD_JOYSTICK_ANALOG == JS1
    pinMode(B_PIN1, INPUT_PULLUP);
    pinMode(B_PIN3, INPUT_PULLUP);
    delay(100);
    int thresholdEW = analogRead(B_PIN1);
    int thresholdNS = analogRead(B_PIN3);
    keyPad.init(pin, active, thresholdNS, thresholdEW);
  #else
    keyPad.init(pins, 0, 0);
  #endif
  VF("MSG: UserInterface, start KeyPad monitor task (rate 10ms priority 1)... ");
  if (tasks.add(10, 0, true, 1, keyPadWrapper, "Keypad")) { VLF("success"); } else { VLF("FAILED!"); }

  #if ST4_AUX_INTERFACE == ON
    auxST4.init();
    VF("MSG: UserInterface, start AuxST4 monitor task (rate 10ms priority 1)... ");
    if (tasks.add(10, 0, true, 1, auxST4Wrapper, "AuxST4")) { VLF("success"); } else { VLF("FAILED!"); }
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
  if (model == OLED_SH1106_4W_SW_SPI) display = new U8G2_EXT_SH1106_128X64_NONAME_1_4W_SW_SPI(U8G2_R0); else
  if (model == OLED_SH1106_4W_HW_SPI) display = new U8G2_EXT_SH1106_128X64_NONAME_1_4W_HW_SPI(U8G2_R0);
  if (model == OLED_SSD1306) display = new U8G2_EXT_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0); else
  if (model == OLED_SSD1309) display = new U8G2_EXT_SSD1309_128X64_NONAME_F_HW_I2C(U8G2_R0); else
  if (model == OLED_SSD1309_4W_SW_SPI) display = new U8G2_EXT_SSD1309_128X64_NONAME_F_4W_SW_SPI(U8G2_R0); else
  if (model == OLED_SSD1309_4W_HW_SPI) display = new U8G2_EXT_SSD1309_128X64_NONAME_F_4W_HW_SPI(U8G2_R0);

  display->begin();
  display->setFont(LF_STANDARD);
  message.init(display);
  display->setContrast(UI::Contrast[displaySettings.maxContrastSelection]);

  // display the splash screen
  if (connectionSelection == CS_NONE) {
    drawIntro();
    delay(2000);
  }

  VF("MSG: UserInterface, start UI update task (rate 30ms priority 6)... ");
  if (tasks.add(30, 0, true, 6, updateWrapper, "UIupd")) { VLF("success"); } else { VLF("FAILED!"); }
}

void UI::poll() {
  // -----------------------------------------------------------------------------------------------------
  // deep sleep
  #ifdef DEEP_SLEEP_WAKEUP_PIN
    if (keyPad.s->isDown() && keyPad.shift->isDown()) {
      VLF("MSG: Entering deep sleep...");
      message.show(L_POWERING, L_OFF "...", 1000);
      esp_deep_sleep_enable_gpio_wakeup(BIT(DEEP_SLEEP_WAKEUP_PIN), ESP_GPIO_WAKEUP_GPIO_LOW);
    }
  #endif

  // -----------------------------------------------------------------------------------------------------
  // connect/reconnect
  static unsigned long lastConnectedTime = 0;
  if (!status.connected && (long)(millis() - lastConnectedTime) > 2000) {
    if (!firstConnect) message.show(L_LOST_MSG, L_CONNECTION, 1000);
    connect();
    reconnectionCount++;
  } else lastConnectedTime = millis();

  unsigned long time_now = millis();

  // -----------------------------------------------------------------------------------------------------
  // sleep and wake up display
  if (keyPad.anyPressed()) {
    if (sleepDisplay) {
      display->setContrast(UI::Contrast[displaySettings.maxContrastSelection]);
      display->sleepOff();
      sleepDisplay = false;
      lowContrast = false;
      status.backgroundCommandRate = FOREGROUND_CMD_RATE;
      keyPad.clearAllPressed();
      time_last_action = millis();
    }
    if (lowContrast) {
      display->setContrast(UI::Contrast[displaySettings.maxContrastSelection]);
      lowContrast = false;
      status.backgroundCommandRate = FOREGROUND_CMD_RATE;
      time_last_action = time_now;
    }
  } else
  if (sleepDisplay) {
    if ((long)time_now - time_keep_alive > 10000) {
      onStepLx200.SetF(":#");
      time_keep_alive = millis();
    }
    return;
  }

  if (displaySettings.blankTimeout && (long)(time_now - time_last_action) > displaySettings.blankTimeout) {
    display->sleepOn();
    time_keep_alive = millis();
    sleepDisplay = true;
    return;
  }

  if (displaySettings.dimTimeout && !lowContrast && (long)(time_now - time_last_action) > displaySettings.dimTimeout) {
    display->setContrast(0);
    lowContrast = true;
    status.backgroundCommandRate = BACKGROUND_CMD_RATE;
    return;
  }

  // -----------------------------------------------------------------------------------------------------
  // main display

  // show align state
  if (status.alignSelectStar()) {
    char alignMessage[10] = L_STAR "# ?";
    if (status.align == Status::ALI_SELECT_STAR_1) strcpy(alignMessage, L_STAR " #1");
    if (status.align == Status::ALI_SELECT_STAR_2) strcpy(alignMessage, L_STAR " #2");
    if (status.align == Status::ALI_SELECT_STAR_3) strcpy(alignMessage, L_STAR " #3");
    if (status.align == Status::ALI_SELECT_STAR_4) strcpy(alignMessage, L_STAR " #4");
    if (status.align == Status::ALI_SELECT_STAR_5) strcpy(alignMessage, L_STAR " #5");
    if (status.align == Status::ALI_SELECT_STAR_6) strcpy(alignMessage, L_STAR " #6");
    if (status.align == Status::ALI_SELECT_STAR_7) strcpy(alignMessage, L_STAR " #7");
    if (status.align == Status::ALI_SELECT_STAR_8) strcpy(alignMessage, L_STAR " #8");
    if (status.align == Status::ALI_SELECT_STAR_9) strcpy(alignMessage, L_STAR " #9");
    message.show(L_ALIGN_MSG1, L_ALIGN_MSG2, "", alignMessage, -1);

    // bring up the list of stars so user can goto the alignment star
    if (!SelectStarAlign()) { message.show(L_ALIGNMENT, L_ABORTED, -1); status.align = Status::ALI_OFF; return; }
    
    // mark this as the next alignment star 
    status.align = static_cast<Status::AlignState>(status.align + 1);
  } else

  // otherwise update the main display
  if ((long)(time_now - lastpageupdate) > status.backgroundCommandRate/2) updateMainDisplay(page);

  // -----------------------------------------------------------------------------------------------------
  // keypad

  // stop gotos
  if (status.getTrackingState() == Status::TRK_SLEWING || status.getParkState() == Status::PRK_PARKING) {
    if (keyPad.nsewPressed()) {
      onStepLx200.Set(":Q#");
      // if aligning, try another align star
      if (status.align != Status::ALI_OFF) status.align = static_cast<Status::AlignState>(status.align - 1);
      time_last_action = millis();
      display->sleepOff();
      keyPad.clearAllPressed();
      message.show(L_SLEW_MSG1, L_SLEW_MSG2 "!", 1000);
      return;
    }
  } else

  // guiding
  {
    buttonCommand = false;
    #if ST4_AUX_INTERFACE == ON
      if (!moveEast  && (keyPad.e->isDown() || auxST4.e->isDown())) { moveEast = true;   onStepLx200.SetF(ccMe); buttonCommand = true; } else
      if ( moveEast  && (keyPad.e->isUp()   && auxST4.e->isUp()))   { moveEast = false;  onStepLx200.SetF(ccQe); buttonCommand = true; keyPad.e->clearPress(); auxST4.e->clearPress(); }
      if (!moveWest  && (keyPad.w->isDown() || auxST4.w->isDown())) { moveWest = true;   onStepLx200.SetF(ccMw); buttonCommand = true; } else
      if ( moveWest  && (keyPad.w->isUp()   && auxST4.w->isUp()))   { moveWest = false;  onStepLx200.SetF(ccQw); buttonCommand = true; keyPad.w->clearPress(); auxST4.w->clearPress(); }
      if (!moveNorth && (keyPad.n->isDown() || auxST4.n->isDown())) { moveNorth = true;  onStepLx200.SetF(ccMn); buttonCommand = true; } else
      if ( moveNorth && (keyPad.n->isUp()   && auxST4.n->isUp()))   { moveNorth = false; onStepLx200.SetF(ccQn); buttonCommand = true; keyPad.n->clearPress(); auxST4.n->clearPress(); }
      if (!moveSouth && (keyPad.s->isDown() || auxST4.s->isDown())) { moveSouth = true;  onStepLx200.SetF(ccMs); buttonCommand = true; } else
      if ( moveSouth && (keyPad.s->isUp()   && auxST4.s->isUp()))   { moveSouth = false; onStepLx200.SetF(ccQs); buttonCommand = true; keyPad.s->clearPress(); auxST4.s->clearPress(); }
    #else
      if (!moveEast  && (keyPad.e->isDown())) { moveEast = true;   onStepLx200.SetF(ccMe); buttonCommand = true; } else
      if ( moveEast  && (keyPad.e->isUp()  )) { moveEast = false;  onStepLx200.SetF(ccQe); buttonCommand = true; keyPad.e->clearPress(); }
      if (!moveWest  && (keyPad.w->isDown())) { moveWest = true;   onStepLx200.SetF(ccMw); buttonCommand = true; } else
      if ( moveWest  && (keyPad.w->isUp()  )) { moveWest = false;  onStepLx200.SetF(ccQw); buttonCommand = true; keyPad.w->clearPress(); }
      if (!moveNorth && (keyPad.n->isDown())) { moveNorth = true;  onStepLx200.SetF(ccMn); buttonCommand = true; } else
      if ( moveNorth && (keyPad.n->isUp()  )) { moveNorth = false; onStepLx200.SetF(ccQn); buttonCommand = true; keyPad.n->clearPress(); }
      if (!moveSouth && (keyPad.s->isDown())) { moveSouth = true;  onStepLx200.SetF(ccMs); buttonCommand = true; } else
      if ( moveSouth && (keyPad.s->isUp()  )) { moveSouth = false; onStepLx200.SetF(ccQs); buttonCommand = true; keyPad.s->clearPress(); }
    #endif
    if (buttonCommand) { time_last_action = millis(); return; }
  }

  // feature keys
  buttonCommand = false;
  if (status.align != Status::ALI_OFF) featureKeyMode = 1;
  switch (featureKeyMode) {
    // adjust guide rate
    case 1:
      if (keyPad.F->wasPressed()) { activeGuideRate--; message.brief(L_FKEY_GUIDE_DN); buttonCommand = true; } else
      if (keyPad.f->wasPressed()) { activeGuideRate++; message.brief(L_FKEY_GUIDE_UP); buttonCommand = true; }
      if (buttonCommand) {
        if (activeGuideRate < 4)  activeGuideRate = 4;
        if (activeGuideRate > 10) activeGuideRate = 10;
        char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate - 1;
        message.show(onStepLx200.Set(cmd));
      }
    break;

    // adjust pulse guide rate
    case 2:
      if (keyPad.F->wasPressed()) { activeGuideRate--; message.brief(L_FKEY_PGUIDE_DN); buttonCommand = true; } else
      if (keyPad.f->wasPressed()) { activeGuideRate++; message.brief(L_FKEY_PGUIDE_UP); buttonCommand = true; }
      if (buttonCommand) {
        if (activeGuideRate < 1) activeGuideRate = 1;
        if (activeGuideRate > 3) activeGuideRate = 3;
        char cmd[5] =  ":Rn#"; cmd[2] = '0' + activeGuideRate - 1;
        message.show(onStepLx200.Set(cmd));
      }
    break;

    // util. light
    case 3:
      #if UTILITY_LIGHT != OFF
        if (keyPad.F->wasPressed()) { current_selection_utility_light--; message.brief(L_FKEY_LAMP_DN); buttonCommand = true; } else
        if (keyPad.f->wasPressed()) { current_selection_utility_light++; message.brief(L_FKEY_LAMP_UP); buttonCommand = true; }
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

    // reticle
    case 4:
      if (keyPad.F->wasPressed()) { onStepLx200.SetF(":B-#"); message.brief(L_FKEY_RETI_DN); } else
      if (keyPad.f->wasPressed()) { onStepLx200.SetF(":B+#"); message.brief(L_FKEY_RETI_UP); }
    break;

    // rotator
    case 5:
      if (rotState == RS_STOPPED && keyPad.F->isDown()) { rotState = RS_CCW_SLOW; onStepLx200.SetF(":r1#:rc#:r<#"); buttonCommand = true; }
      else if ((rotState == RS_CCW_SLOW || rotState == RS_CCW_MID || rotState == RS_CCW_FAST) && keyPad.F->isUp()) { rotState = RS_STOPPED; onStepLx200.SetF(":rQ#"); buttonCommand = true; keyPad.F->clearPress(); }
      else if (rotState == RS_STOPPED && keyPad.f->isDown()) { rotState = RS_CW_SLOW;  onStepLx200.SetF(":r1#:rc#:r>#"); buttonCommand = true; }
      else if ((rotState == RS_CW_SLOW || rotState == RS_CW_MID || rotState == RS_CW_FAST) && keyPad.f->isUp()) { rotState = RS_STOPPED; onStepLx200.SetF(":rQ#"); buttonCommand = true; keyPad.f->clearPress(); }
      else if (rotState == RS_CCW_MID && keyPad.F->isDown() && keyPad.F->timeDown() > 6000) { rotState = RS_CCW_FAST; onStepLx200.SetF(":r4#:rc#:r<#"); }
      else if (rotState == RS_CCW_SLOW && keyPad.F->isDown() && keyPad.F->timeDown() > 3000) { rotState = RS_CCW_MID; onStepLx200.SetF(":r2#:rc#:r<#"); }
      else if (rotState == RS_CW_MID  && keyPad.f->isDown() && keyPad.f->timeDown() > 6000) { rotState = RS_CW_FAST;  onStepLx200.SetF(":r4#:rc#:r>#"); }
      else if (rotState == RS_CW_SLOW  && keyPad.f->isDown() && keyPad.f->timeDown() > 3000) { rotState = RS_CW_MID;  onStepLx200.SetF(":r4#:rc#:r>#"); }
      if (rotState > RS_STOPPED) nextRotMessageUpdateCycles = 120;
      if (nextRotMessageUpdateCycles > 0) {
        nextRotMessageUpdateCycles--;
        float rpos = status.getRotatorPosition();
        if (!isnan(rpos)) {
          char temp[24];
          if (rotState == RS_CCW_SLOW || rotState == RS_CCW_MID || rotState == RS_CCW_FAST) sprintf(temp, L_ROTATE " < %d°", (int)round(rpos)); else
          if (rotState == RS_CW_SLOW || rotState == RS_CW_MID || rotState == RS_CW_FAST) sprintf(temp, L_ROTATE " > %d°", (int)round(rpos)); else
          sprintf(temp, L_ROTATE "    %d°", (int)round(rpos));
          message.brief(temp);
        } else message.brief("?");
      }
    break;

    // focusers
    case 6: case 7: case 8: case 9: case 10: case 11:
      #ifdef FOCUSER_ACCELERATE_DISABLE_ON
        if (focusState == FS_STOPPED && keyPad.F->isDown()) { focusState = FS_OUT_SLOW; onStepLx200.SetF(":F2#:F+#"); buttonCommand = true; }
        else if ((focusState == FS_OUT_SLOW || focusState == FS_OUT_MID || focusState == FS_OUT_FAST) && keyPad.F->isUp()) { focusState = FS_STOPPED; onStepLx200.SetF(":FQ#"); buttonCommand = true; keyPad.F->clearPress(); }
        else if (focusState == FS_STOPPED && keyPad.f->isDown()) { focusState = FS_IN_SLOW;  onStepLx200.SetF(":F2#:F-#"); buttonCommand = true; }
        else if ((focusState == FS_IN_SLOW || focusState == FS_IN_MID || focusState == FS_IN_FAST) && keyPad.f->isUp()) { focusState = FS_STOPPED; onStepLx200.SetF(":FQ#"); buttonCommand = true; keyPad.f->clearPress(); }
      #else
        if (focusState == FS_STOPPED && keyPad.F->isDown()) { focusState = FS_OUT_SLOW; onStepLx200.SetF(":F1#:F+#"); buttonCommand = true; }
        else if ((focusState == FS_OUT_SLOW || focusState == FS_OUT_MID || focusState == FS_OUT_FAST) && keyPad.F->isUp()) { focusState = FS_STOPPED; onStepLx200.SetF(":FQ#"); buttonCommand = true; keyPad.F->clearPress(); }
        else if (focusState == FS_STOPPED && keyPad.f->isDown()) { focusState = FS_IN_SLOW;  onStepLx200.SetF(":F1#:F-#"); buttonCommand = true; }
        else if ((focusState == FS_IN_SLOW || focusState == FS_IN_MID || focusState == FS_IN_FAST) && keyPad.f->isUp()) { focusState = FS_STOPPED; onStepLx200.SetF(":FQ#"); buttonCommand = true; keyPad.f->clearPress(); }
        else if (focusState == FS_OUT_MID && keyPad.F->isDown() && keyPad.F->timeDown() > 6000) { focusState = FS_OUT_FAST; onStepLx200.SetF(":F4#:F+#"); }
        else if (focusState == FS_OUT_SLOW && keyPad.F->isDown() && keyPad.F->timeDown() > 3000) { focusState = FS_OUT_MID; onStepLx200.SetF(":F2#:F+#"); }
        else if (focusState == FS_IN_MID  && keyPad.f->isDown() && keyPad.f->timeDown() > 6000) { focusState = FS_IN_FAST;  onStepLx200.SetF(":F4#:F-#"); }
        else if (focusState == FS_IN_SLOW  && keyPad.f->isDown() && keyPad.f->timeDown() > 3000) { focusState = FS_IN_MID;  onStepLx200.SetF(":F2#:F-#"); }
      #endif
      if (focusState > FS_STOPPED) nextFocuserMessageUpdateCycles = 120;
      if (nextFocuserMessageUpdateCycles > 0) {
        nextFocuserMessageUpdateCycles--;
        float fpos = status.getFocuserPosition();
        if (!isnan(fpos)) {
          char temp[24];
          if (focusState == FS_OUT_SLOW || focusState == FS_OUT_MID || focusState == FS_OUT_FAST) sprintf(temp, L_FOCUS " \\/ %dum", (int)round(fpos)); else
          if (focusState == FS_IN_SLOW || focusState == FS_IN_MID || focusState == FS_IN_FAST) sprintf(temp, L_FOCUS " /\\ %dum", (int)round(fpos)); else
          sprintf(temp, L_FOCUS "    %dum", (int)round(fpos));
          message.brief(temp);
        } else message.brief("?");
      }
    break;

    // auxiliary features
    case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
	    char cmd[20];
      char line2[20];
      if (status.featureSelectByOrder(featureKeyMode - 11)) {
        if (keyPad.F->wasPressed()) {
          if (status.featurePurpose() == ANALOG_OUT) {
            status.featureUpdate(featureKeyMode - 11);
            // ANALOG_OUT but in units of 5%
            int v = lround(status.featureValue1()/12.75F) - 2;
            if (v < 0) v = 0;
            sprintf(cmd, ":SXX%i,V%i#", featureKeyMode - 11, (int)lround(v*12.75F));
            onStepLx200.Set(cmd);
            sprintf(line2, "%i%%", v*5);
            message.show(status.featureName(), line2, 1000);
          } else {
            sprintf(cmd, ":SXX%i,V%i#", featureKeyMode - 11, 0);
            onStepLx200.Set(cmd);
            sprintf(line2, "%s", L_OFF);
            message.show(status.featureName(), line2, 1000);
          }
        } else
        if (keyPad.f->wasPressed()) {
          if (status.featurePurpose() == ANALOG_OUT) {
            status.featureUpdate(featureKeyMode - 11);
            int v = lround(status.featureValue1()/12.75F) + 2;
            if (v > 20) v = 20;
            sprintf(cmd, ":SXX%i,V%i#", featureKeyMode - 11, (int)lround(v*12.75F));
            onStepLx200.Set(cmd);
            sprintf(line2, "%i%%", v*5);
            message.show(status.featureName(), line2, 1000);
          } else {
            sprintf(cmd, ":SXX%i,V%i#", featureKeyMode - 11, 1);
            onStepLx200.Set(cmd);
            sprintf(line2, "%s",  L_ON);
            message.show(status.featureName(), line2, 1000);
          }
        }
      } else { DL("WRN: aux feature not found"); }
    break;
  }
  if (buttonCommand) { time_last_action = millis(); return; }

  // a long press brings up the main menu
  if (keyPad.shift->isDown()) {
    if (keyPad.shift->timeDown() > 1000 && status.align == Status::ALI_OFF) {
      // bring up the menus
      menuMain();
      time_last_action = millis();
    }
  } else

  // check for a double click
  {
    // wait long enough that a double press can happen before picking up other press events
    if (keyPad.shift->timeUp() > 250) {

      if (keyPad.shift->wasDoublePressed()) {
        if (status.align == Status::ALI_OFF) {
          // bring up the feature key menu
          menuFeatureKey();
        } else {
          // if aligning, go back and select a different star
          if (status.alignRecenterStar()) {
            status.align = static_cast<Status::AlignState>(status.align - 2);
          }
        }
        time_last_action = millis();
      } else
      if (keyPad.shift->wasPressed()) {
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
          if (status.alignRecenterStar()) {
            if (status.alignAddStar()) { if (status.align == Status::ALI_OFF) message.show(L_ALIGNMENT, L_SUCCESS "!", 2000); else message.show(L_ADD_STAR, L_SUCCESS "!", 2000); } else message.show(L_ADD_STAR, L_FAILED "!", -1);
          }
        }
        time_last_action = millis();
      }
    }
  }
}

void UI::updateMainDisplay(u8g2_uint_t page) {
  display->setFont(LF_LARGE);
  u8g2_uint_t line_height = display->getAscent() - display->getDescent() + 1;
  #ifdef ESP32
    line_height -= 3;
  #endif

  // get the status
  status.connected = true;
  status.updateSeq++;
  status.updateTel();
  if (status.connected == false) return;

  // detect align mode
  if (status.hasTelStatus && status.align != Status::ALI_OFF) {
    // really make sure we have the status
    status.updateTel(true);
    Status::TrackState curT = status.getTrackingState();
    if (curT != Status::TRK_SLEWING && status.alignSlewStar()) {
      status.align = static_cast<Status::AlignState > (status.align + 1);
    }
    page = 4;
  }

  // update status info.
  if (page == 0) status.updateRaDec();
  else if (page == 1) status.updateAzAlt();
  else if (page == 2) status.updateTime();

  // the graphics loop
  display->firstPage();
  do {
    u8g2_uint_t x = display->getDisplayWidth();

    // OnStep status
    if (status.hasTelStatus) { 

      // update guide rate (if available)
      if (status.getGuideRate() >= 0) {
        char string_Speed[][8] = {"¼x","½x","1x","2x","4x","8x","20x","48x","½Mx","Max"};
        char string_PSpeed[][6] = {" ¼x"," ½x"," 1x"};
        int gr = status.getGuideRate();
        activeGuideRate = gr + 1;
        int pgr = status.getPulseGuideRate();
        if (pgr != gr && pgr >= 0 && pgr < 3) strcat(string_Speed[gr], string_PSpeed[pgr]); 
        if (gr >= 0 && gr <= 9) {
          display->setFont(LF_STANDARD);
          display->drawUTF8(0, icon_height - 2, string_Speed[gr]);
          display->setFont(LF_LARGE);
        }
      }

      Status::ParkState curP = status.getParkState();
      Status::TrackState curT = status.getTrackingState();
      Status::TrackRate curTR = status.getTrackingRate();

      #ifdef BATTERY_VOLTAGE_PIN
        static float batteryVoltage = 3.7F;
        float v = analogReadMilliVolts(BATTERY_VOLTAGE_PIN)/1000.0F;
        if (BATTERY_VOLTAGE_FORMULA(v) < 5.0F) batteryVoltage = (batteryVoltage*9.0F + BATTERY_VOLTAGE_FORMULA(v))/10.0F;

        if (batteryVoltage < BATTERY_VOLTAGE_0) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, battery_0_percent_bits); x -= icon_narrow_width + 1; } else
        if (batteryVoltage < BATTERY_VOLTAGE_25) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, battery_25_percent_bits); x -= icon_narrow_width + 1; } else
        if (batteryVoltage < BATTERY_VOLTAGE_50) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, battery_50_percent_bits); x -= icon_narrow_width + 1; } else
        if (batteryVoltage < BATTERY_VOLTAGE_75) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, battery_75_percent_bits); x -= icon_narrow_width + 1; } else
        { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, battery_100_percent_bits); x -= icon_narrow_width + 1; }
      #endif

      if (curP == Status::PRK_PARKED)  { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parked_bits); x -= icon_width + 1; } else
      if (curP == Status::PRK_PARKING) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parking_bits); x -= icon_width + 1; } else
      if (status.atHome())             { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, home_bits); x -= icon_width + 1;  } else 
      {
        if (curT == Status::TRK_SLEWING) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, slewing_bits); x -= icon_width + 1; } else
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
          if (curTR == Status::TR_KING) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_kng_bits); x -= icon_width + 1; }
        } else
        if (curT == Status::TRK_OFF) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, no_tracking_bits); x -= icon_width + 1; }

        if (curP == Status::PRK_FAILED) { display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parkingFailed_bits); x -= icon_width + 1; }

        Status::PierState CurP = status.getPierState();
        if (CurP == Status::PIER_E) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, E_bits); x -= icon_narrow_width + 1; } else 
        if (CurP == Status::PIER_W) { display->drawXBMP(x - icon_narrow_width, 0, icon_narrow_width, icon_height, W_bits); x -= icon_narrow_width + 1; }

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
          case Status::ERR_NONE: break;
          case Status::ERR_MOTOR_FAULT: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMot_bits);  x -= icon_width + 1; break;
          case Status::ERR_ALT_MIN:
            if (status.getOnStepVersion() < 1000)
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAltLowHigh_bits);
            else
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAltLow_bits);
            x -= icon_width + 1;
          break;
          case Status::ERR_LIMIT_SENSE: display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrLimitSW_bits);  x -= icon_width + 1; break;
          case Status::ERR_DEC:
            if (status.isMountFork())
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrDecFork_bits);
            else
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrDecGEM_bits);
            x -= icon_width + 1;
          break;
          case Status::ERR_AZM:         display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAzmDOB_bits);  x -= icon_width + 1; break;
          case Status::ERR_UNDER_POLE:
            if (status.isMountFork())
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrRaFork_bits);
            else
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrRaGEM_bits);
            x -= icon_width + 1;
          break;
          case Status::ERR_MERIDIAN:    display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMER_bits);  x -= icon_width + 1; break;
          case Status::ERR_WEATHER_INIT:display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrWeather_bits);  x -= icon_width + 1; break;
          case Status::ERR_SITE_INIT:   display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrSite_bits);  x -= icon_width + 1; break;
          case Status::ERR_NV_INIT:     display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrNV_bits);  x -= icon_width + 1; break;
          case Status::ERR_ALT_MAX:     display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrAltHigh_bits); x -= icon_width + 1; break;
          default:                      display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrOther_bits); x -= icon_width + 1; break;
        }
      }
    }

    // show brief message
    if (message.briefShowing()) message.briefShow(); else

    // show equatorial coordinates
    if (page == 0) {
      if (status.hasInfoRa && status.hasInfoDec) {
        char rs[20]; strcpy(rs, status.TempRa);
        int l = strlen(rs);
        if (l > 1) rs[l - 1] = 0;
        u8g2_uint_t x = display->getDisplayWidth() - display->getUTF8Width("000000000");
        u8g2_uint_t y = 36;

        display->drawUTF8(0, y, L_RA);
        display->DrawFwNumeric(x, y, rs);

        char ds[20]; strcpy(ds, status.TempDec);
        l = strlen(ds);
        if (l > 1) ds[l - 1] = 0;
        if (l > 8) { ds[3] = '\xb0'; ds[6] = '\''; }
        x = display->getDisplayWidth() - display->getUTF8Width("0000000000");
        y += line_height + 4;
        display->drawUTF8(0, y, L_DEC); 
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
        x = display->getDisplayWidth() - display->getUTF8Width("0000000000");
        u8g2_uint_t y = 36;
        display->drawUTF8(0, y, L_AZ);
        display->DrawFwNumeric(x, y, zs);

        char as[20]; strcpy(as, status.TempAlt);
        l = strlen(as);
        if (l > 1) as[l - 1] = 0;
        if (l > 8) { as[3] = '\xb0'; as[6] = '\''; }
        y += line_height + 4;
        display->drawUTF8(0, y, L_ALT);
        display->DrawFwNumeric(x, y, as);
      }
    } else
    
    // show time
    if (page == 2) {
      if (status.hasInfoUTC && status.hasInfoSidereal) {
        char us[20]; strcpy(us, status.TempUniversalTime);
        int l = strlen(us);
        if (l > 1) us[l - 1] = 0;
        x = display->getDisplayWidth() - display->getUTF8Width("000000000");
        u8g2_uint_t y = 36;
        display->setFont(LF_STANDARD);
        display->drawUTF8(0, y, "UT");
        display->setFont(LF_LARGE);
        display->DrawFwNumeric(x, y, us);

        char ss[20]; strcpy(ss,status.TempSidereal);
        l = strlen(ss);
        if (l > 1) ss[l - 1] = 0;
        y += line_height + 4;
        display->drawUTF8(0, y, "LST");
        display->DrawFwNumeric(x, y, ss);
      }
    } else

    if (page == 3) {
      // T24.6 P997mb
      // H46% DP13.7C
      display->setFont(LF_STANDARD);

      double T, P, H, DP;
      if (status.getT(T) && status.getP(P) && status.getH(H) && status.getDP(DP)) {
        char temp[32], line[64];
        u8g2_uint_t y = 36;
        u8g2_uint_t dx = display->getDisplayWidth();

        if (UNITS == IMPERIAL) {
        dtostrf((T*(9.0/5.0) + 32.0), 2, 0, temp);
        sprintf(line, "T%s\xb0%s", temp, "F");
        display->DrawFwNumeric(0, y, line);

        dtostrf((P/33.864), 3, 1, temp); //drew
        sprintf(line, "P%s\x22%s", temp, "Hg");
        }
        else {
        dtostrf(T, 3, 1, temp);
        sprintf(line, "T%s\xb0%s", temp, "C");
        display->DrawFwNumeric(0, y, line);

        sprintf(line, "P%dmb",(int)round(P));          
        }
        display->DrawFwNumeric(dx - display->GetFwNumericWidth(line), y, line);

        y += line_height + 4;
        sprintf(line, "H%d%%", (int)round(H));
        display->DrawFwNumeric(0, y, line);

        if (UNITS == IMPERIAL) {
        dtostrf((DP*(9.0/5.0) + 32.0), 3, 1, temp);
        sprintf(line, "DP%s\xb0%s", temp, "F");
        }
        else {
        dtostrf(DP, 3, 1, temp);
        sprintf(line, "DP%s\xb0%s", temp, "C"); 
        }
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
      display->drawUTF8(0, y, txt);

      y += line_height + 4;
      display->setFont(LF_GREEK);
      display->drawGlyph(0, y, 945 + cat_mgr.bayerFlam());

      display->setFont(LF_LARGE);
      display->drawUTF8(16, y, cat_mgr.constellationStr());
    }

  } while (display->nextPage());
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
    if (display->UserInterfaceCatalog(&keyPad, L_SELECT_STAR)) {
      bool ok = message.show(onStepLx200.SyncSelectedStar(cat_mgr.getIndex()),false);
      return ok;
    }
  }
  return false;
}

void UI::connect() {

if (connectionSelection == CS_CONNECT_MENU) connectionSelection = CS_NONE;
connectAgain:

  // count down during reconnect attempts
  if (--skipConnectionSelection < 0) skipConnectionSelection = 0;
  if (!skipConnectionSelection) {

    // shutdown any open connection
    if (!firstConnect) {
      #if SERIAL_ONSTEP != OFF
        if (onStep.connectionMode == CM_SERIAL) SERIAL_ONSTEP.end();
      #endif

      #if SERIAL_IP_MODE != OFF
        if (onStep.connectionMode == CM_WIFI) { SERIAL_IP.end(); wifiManager.disconnect(); }
      #endif

      if ((onStep.connectionMode == CM_BLUETOOTH) || REBOOT_TO_CONNECT_MENU == ON) {
        VLF("MSG: Connect, setting boot flag for menu (restarting...)");
        nv.write(NV_SERIAL_BOOT_FLAG_BASE, (uint8_t)CS_CONNECT_MENU);
        message.show(L_SCANNING, L_PLEASE_WAIT "...", 10);
        tasks.yield(NV_WAIT + 500);
        HAL_RESET();
      }
    }

    #if SERIAL_ONSTEP != OFF && SERIAL_IP_MODE == OFF && SERIAL_BT_MODE == OFF
      // flag serial only operation
      connectionSelection = CS_SERIAL;
    #else
      // allow the user to make a selection
      if (connectionSelection == CS_NONE) menuWireless();
    #endif

    // start the connection
    #if SERIAL_ONSTEP != OFF
      if (connectionSelection == CS_SERIAL) {
        onStep.connectionMode = CM_SERIAL;
        connectionSelection = CS_NONE;

        #if defined(SERIAL_ONSTEP_RX) && defined(SERIAL_ONSTEP_TX)
          SERIAL_ONSTEP.begin(serialBaud, SERIAL_8N1, SERIAL_ONSTEP_RX, SERIAL_ONSTEP_TX);
        #else
          SERIAL_ONSTEP.begin(serialBaud);
        #endif
      }
    #endif

    #if SERIAL_IP_MODE != OFF
      if (connectionSelection >= CS_WIFI_STA1 && connectionSelection <= CS_WIFI_STA6) {
        wifiManager.setStation((connectionSelection - (int)CS_WIFI_STA1) + 1);
        onStep.connectionMode = CM_WIFI;
        connectionSelection = CS_NONE;

        if (!wifiManager.active) {
          VLF("MSG: Connect, WiFi starting");
          message.show(L_WIFI_CONNECTION, wifiManager.sta->ssid, 100);
          delay(500);

          wifiManager.staNameLookup = true;
          if (!wifiManager.init()) {
            VLF("MSG: Connect, WiFi start failed");
            message.show(L_WIFI_CONNECTION, L_FAILED, 2000);
            delay(5000);
            goto connectAgain;
          }
        }

        message.show(L_CONNECTING, IPAddress(wifiManager.sta->target).toString().c_str(), 1000);
        if (!SERIAL_IP.begin(serialBaud)) {
          VLF("MSG: Connect, to target failed");
          wifiManager.disconnect();
          delay(2000);
          message.show(L_CONNECTING, L_FAILED, 2000);
          goto connectAgain;
        }
      }
    #endif

    #if SERIAL_BT_MODE != OFF
      if (connectionSelection >= CS_BT_STA1 && connectionSelection <= CS_BT_STA8) {
        bluetoothManager.setStation((connectionSelection - (int)CS_BT_STA1) + 1);
        onStep.connectionMode = CM_BLUETOOTH;
        connectionSelection = CS_NONE;

        if (!bluetoothManager.active) {
          VLF("MSG: Connect, Bluetooth starting");
          message.show(L_BT_CONNECTION, bluetoothManager.sta->address, 100);
          delay(500);

          SERIAL_BT.begin(SERIAL_BT_NAME, true);

          if (!bluetoothManager.init()) {
            VLF("MSG: Connect, Bluetooth start failed");
            message.show(L_BT_CONNECTION, L_FAILED, 2000);
            delay(5000);
            goto connectAgain;
          }
        }
      }
    #endif

    firstConnect = false;
  }

  VLF("MSG: Connect, looking for OnStep...");

  queryTry = 0;
queryOnStep:
  if (queryTry % 1 == 0) message.show(L_LOOKING, "OnStep", 500); else message.show(L_LOOKING, "...", 500);

  for (int i = 0; i < 3; i++) {
    delay(100);
    onStepLx200.SetF(":#");

    delay(300);
    #if SERIAL_IP_MODE != OFF
      if (onStep.connectionMode == CM_WIFI) SERIAL_IP.flush();
    #endif
    #if SERIAL_ONSTEP != OFF
      if (onStep.connectionMode == CM_SERIAL) SERIAL_ONSTEP.flush();
    #endif
  }

  char s[80] = "";
  CMD_RESULT r = onStepLx200.Get(":GVP#", s);
  if (r != CR_VALUE_GET || !strstr(s, "On-Step")) {
    if (++queryTry < 3) goto queryOnStep;

    #if SERIAL_IP_MODE != OFF
      if (onStep.connectionMode == CM_WIFI) {
        SERIAL_IP.end();
        wifiManager.disconnect();
      }
    #endif
    #if SERIAL_ONSTEP != OFF
      if (onStep.connectionMode == CM_SERIAL) SERIAL_ONSTEP.end();
    #endif

    delay(1000);

    goto connectAgain;
  }

  onStepLx200.Get(":GVN#", s);

  VF("MSG: Connect, found OnStep "); VL(s);

  initGuideCommands();

  #if SERIAL_IP_MODE != OFF
    if (onStep.connectionMode == CM_WIFI) skipConnectionSelection = 3;
  #endif
  #if SERIAL_BT_MODE != OFF
    if (onStep.connectionMode == CM_BLUETOOTH) skipConnectionSelection = 2;
  #endif
  #if SERIAL_ONSTEP != OFF
    if (onStep.connectionMode == CM_SERIAL) skipConnectionSelection = 2;
  #endif

  queryTry = 0;
queryCoordinateSystem:
  delay(500);

  // OnStep coordinate mode for getting and setting RA/Dec
  // 0 = OBSERVED_PLACE (same as not supported)
  // 1 = TOPOCENTRIC (does refraction)
  // 2 = ASTROMETRIC_J2000 (does refraction and precession/nutation)
  if (onStepLx200.Get(":GXEE#", s) == CR_VALUE_GET && s[0] >= '0' && s[0] <= '3' && s[1] == 0) {
    if (s[0] == '0') {
      VLF("MSG: Connect, coords Observed Place");
      telescopeCoordinates = OBSERVED_PLACE; 
      message.show(L_CONNECTION, L_OK "!", 500);
    } else 
    if (s[0] == '1') {
      VLF("MSG: Connect, coords Topocentric");
      telescopeCoordinates = TOPOCENTRIC; 
      message.show(L_CONNECTION, L_OK "!", 500);
    } else 
    if (s[0] == '2') {
      VLF("MSG: Connect, coords J2000");
      telescopeCoordinates = ASTROMETRIC_J2000;
      message.show(L_CONNECTION, L_OK "!", 500);
    }
  } else {
    if (++queryTry <= 3) goto queryCoordinateSystem;
    DLF("WRN: Connect, get coords failed");
    VLF("MSG: Connect, fallback Observed Place");
    telescopeCoordinates = OBSERVED_PLACE;
    message.show(L_CONNECTION, L_WARNING "!", 1000);
    message.show(L_COORDINATES, L_OBSERVED_PLACE ".", 2000);
  }

  // check to see if we have auxiliary features
  hasAuxFeatures = status.featureScan();

  status.connected = true;
}

// get guide commands ready, use single byte for SerialST4 or normal LX200 otherwise
void UI::initGuideCommands() {

  // SerialST4 always returns 0 "may block", Teensy and ESP32 always return > 0
  bool useFastGuides = false;
  #if SERIAL_ONSTEP != OFF
    if (onStep.connectionMode == CM_SERIAL && SERIAL_ONSTEP.availableForWrite() == 0) useFastGuides = true;
  #endif

  if (useFastGuides) {
    VLF("MSG: UserInterface, using fast single byte guide commands ");
    strcpy(ccMe, "\x0e"); // 14
    strcpy(ccMw, "\x0f"); // 15
    strcpy(ccMn, "\x10"); // 16
    strcpy(ccMs, "\x11"); // 17
    strcpy(ccQe, "\x12"); // 18
    strcpy(ccQw, "\x13"); // 19
    strcpy(ccQn, "\x14"); // 20
    strcpy(ccQs, "\x15"); // 21
  } else {
    VLF("MSG: UserInterface, using LX200 guide commands ");
    strcpy(ccMe, ":Me#");
    strcpy(ccMw, ":Mw#");
    strcpy(ccMn, ":Mn#");
    strcpy(ccMs, ":Ms#");
    strcpy(ccQe, ":Qe#");
    strcpy(ccQw, ":Qw#");
    strcpy(ccQn, ":Qn#");
    strcpy(ccQs, ":Qs#");
  }
}

UI userInterface;
