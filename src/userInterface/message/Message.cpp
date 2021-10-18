
#include "Message.h"

void Message::init(U8G2_EXT *display) {
  this->display = display;
}

void Message::show(const char* txt1, const char* txt2, int duration) {
  u8g2_uint_t x;
  u8g2_uint_t y = 40;

  display->firstPage();
  do {
    if (txt2 != NULL) {
      y = 50;
      x = (display->getDisplayWidth() - display->getStrWidth(txt2)) / 2;
      display->drawStr(x, y, txt2);
      y = 25;
    }

    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
  } while (display->nextPage());

  if (duration >= 0) {
    delay(duration);
  } else {
    keyPad.waitForPress();
    keyPad.clearAllPressed();
  }
}

void Message::show(const char* txt1, const char* txt2, const char* txt3, int duration) {
  u8g2_uint_t x;
  u8g2_uint_t y = 40;

  display->firstPage();
  do {
    if (txt2 != NULL) {
      y = 50;
      x = (display->getDisplayWidth() - display->getStrWidth(txt2)) / 2;
      display->drawStr(x, y, txt2);
      if (txt3 != NULL) {
        x = display->getDisplayWidth() - (display->getStrWidth(txt3) + 15);
        display->drawStr(x, y, txt3);
      }
      y = 25;
    }

    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
  } while (display->nextPage());

  if (duration >= 0) {
    delay(duration);
  } else {
    keyPad.waitForPress();
    keyPad.clearAllPressed();
  }
}

void Message::show(const char* txt1, const char* txt2, const char* txt3, const char* txt4, int duration) {
  display->setFont(LF_STANDARD);
  u8g2_uint_t h = 15;
  u8g2_uint_t x = 0;
  u8g2_uint_t y = h;

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

  if (duration >= 0) delay(duration); else { keyPad.waitForPress(); keyPad.clearAllPressed(); }
  display->setFont(LF_LARGE);
}

bool Message::show(CMD_RESULT cmdResult, bool silentOk) {
  char text1[20] = "";
  char text2[20] = "";
  int time = -1;
  if (cmdResult < CR_OK) {
         if (cmdResult == CR_NOT_OK)                  { sprintf(text1, L_CMD_NOTOK_1); sprintf(text2, L_CMD_NOTOK_2);  }
    else if (cmdResult == CR_SET_VALUE_FAILED)        { sprintf(text1, L_CMD_SETVF_1); sprintf(text2, L_CMD_SETVF_2);  }
    else if (cmdResult == CR_GET_VALUE_FAILED)        { sprintf(text1, L_CMD_GETVF_1); sprintf(text2, L_CMD_GETVF_2);  }
    else if (cmdResult == CR_SET_TARGET_FAILED)       { sprintf(text1, L_CMD_SETTG_1); sprintf(text2, L_CMD_SETTG_2);  }
    else if (cmdResult == CR_NO_OBJECT_SELECTED)      { sprintf(text1, L_CMD_OBJSE_1); sprintf(text2, L_CMD_OBJSE_2);  }
    else if (cmdResult == CR_GOTO_ERR_BELOW_HORIZON)  { sprintf(text1, L_CMD_TGHOR_1); sprintf(text2, L_CMD_TGHOR_2);  }
    else if (cmdResult == CR_GOTO_ERR_ABOVE_OVERHEAD) { sprintf(text1, L_CMD_TGOVH_1); sprintf(text2, L_CMD_TGOVH_2);  }
    else if (cmdResult == CR_GOTO_ERR_STANDBY)        { sprintf(text1, L_CMD_STNBF_1); sprintf(text2, L_CMD_STNBF_2);  }
    else if (cmdResult == CR_GOTO_ERR_PARK)           { sprintf(text1, L_CMD_PARKF_1); sprintf(text2, L_CMD_PARKF_2);  }
    else if (cmdResult == CR_GOTO_ERR_GOTO)           { sprintf(text1, L_CMD_GOGOF_1); sprintf(text2, L_CMD_GOGOF_2);  }
    else if (cmdResult == CR_GOTO_ERR_OUTSIDE_LIMITS) { sprintf(text1, L_CMD_LIMTF_1); sprintf(text2, L_CMD_LIMTF_2);  }
    else if (cmdResult == CR_GOTO_ERR_HARDWARE_FAULT) { sprintf(text1, L_CMD_HARDF_1); sprintf(text2, L_CMD_HARDF_2);  }
    else if (cmdResult == CR_GOTO_ERR_IN_MOTION)      { sprintf(text1, L_CMD_GOMOF_1); sprintf(text2, L_CMD_GOMOF_2);  }
    else if (cmdResult == CR_GOTO_ERR_UNSPECIFIED)    { sprintf(text1, L_CMD_UNSPF_1); sprintf(text2, L_CMD_UNSPF_2);  }
    else                                              { sprintf(text1, L_CMD_ERROR);   sprintf(text2, "-1"); }
    show(text1, text2, -1);
  } else
  if (!silentOk) {
    time = 1000;
         if (cmdResult == CR_OK)            { sprintf(text1, L_CMD_ISAOK_1); sprintf(text2, L_CMD_ISAOK_2);  }
    else if (cmdResult == CR_VALUE_SET)     { sprintf(text1, L_CMD_SETOK_1); sprintf(text2, L_CMD_SETOK_2);  }
    else if (cmdResult == CR_VALUE_GET)     { sprintf(text1, L_CMD_GETOK_1); sprintf(text2, L_CMD_GETOK_2);  }
    else if (cmdResult == CR_SYNCED)        { sprintf(text1, L_CMD_SNCOK_1); sprintf(text2, L_CMD_SNCOK_2);  }
    else if (cmdResult == CR_GOTO_GOING_TO) { sprintf(text1, L_CMD_GOTOK_1); sprintf(text2, L_CMD_GOTOK_2);  }
    show(text1, text2, time);
  }
  return onStep.isOk(cmdResult);
}

void Message::brief(const char* txt1, int duration) {
  strcpy(briefMessage, txt1);
  briefMessageEndTime = millis() + duration;
  showing = true;
}

bool Message::briefShowing() {
  if (showing && (long)(millis() - briefMessageEndTime) > 0) showing = false;
  return showing;
}

void Message::briefShow() {
  if (showing) {
    u8g2_uint_t x = display->getDisplayWidth();
    u8g2_uint_t x1 = display->getUTF8Width(briefMessage);
    u8g2_uint_t y = 36;
    display->drawUTF8(x/2 - x1/2, y + 8, briefMessage);
  }
}

Message message;
