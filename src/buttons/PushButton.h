// -----------------------------------------------------------------------------------------------------------------------------
// Push button
#pragma once

#include "../Common.h"

class button {
  public:
    void init(int pin, unsigned long debounceMs, bool pullup, int analogThreshold, bool activeLow = true) {
      this->analogThreshold = analogThreshold;
      this->pin = pin;
      this->debounceMs = debounceMs;
      if (pullup) pinMode(pin, INPUT_PULLUP); else pinMode(pin, INPUT);
      if (activeLow) { UP = HIGH; DOWN = LOW; } else { UP = LOW; DOWN = HIGH; }
    }
    // must be repeatedly called to check status of button
    void poll() {
      int lastState = state;

      if (analogThreshold == 0) {
        // an analog threshold of exactly zero means this is a digital input
        state = digitalRead(pin);
      } else if (analogThreshold < 0) {
        // an analog threshold of < zero means this is an analog input and we are checking the negative value 
        state = analogRead(pin) + analogThreshold; // ranges from about 0 to -500
        if (state < -300) state = LOW; else state = HIGH;
      } else if (analogThreshold > 0) {
        // an analog threshold of > zero means this is an analog input and we are checking the positive value 
        state = analogRead(pin) - analogThreshold; // ranges from about 0 to 500
        if (state > 300) state = LOW; else state = HIGH;
      }
      
      if (lastState != state) { avgPulseDuration = ((avgPulseDuration*49.0) + (double)(millis() - stableStartMs))/50.0; stableStartMs = millis(); }
      stableMs = (long)(millis() - stableStartMs);
      if (stableMs > 3000UL) { avgPulseDuration = ((avgPulseDuration*4.0) + 2000.0)/5.0; }
      if (stableMs > debounceMs) {
        if (lastStableState == UP && state == DOWN) { 
          static unsigned long lastPressTime = 0;
          if (pressed && (long)(millis()-lastPressTime) < 500) doublePressed = true; 
          pressed = true; 
          lastPressTime = millis();
        }
        lastStableState = state;
      }
    }
    // is the button down
    bool isDown() { if (stableMs > debounceMs && state == DOWN) return true; else return false; }
    // was the button down since last checked
    bool wasPressed(boolean peek = false) { if (pressed) { if (!peek) pressed = false; return true; } else return false; }
    // was the button down since last checked
    bool wasDoublePressed(boolean peek = false) { if (doublePressed) { if (!peek) { doublePressed = false; pressed = false; } return true; } else return false; }
    // was the button down since last checked
    bool wasClicked(boolean peek = false) { if (isUp()) return wasPressed(peek); else return false; }
    // clear pressed state
    void clearPress() { pressed = false; }
    // is the button up
    bool isUp() { if (stableMs > debounceMs && state == UP) return true; else return false; }
    // number of ms down
    long timeDown() { if (stableMs > debounceMs && state == DOWN) return stableMs; else return 0; }
    // number of ms up
    long timeUp() { if (stableMs > debounceMs && state == UP) return stableMs; else return 0; }
    // check to see if this button has the SHC tone
    boolean hasTone() { if (fabs(avgPulseDuration - 40.0) < 5.0) return true; else return false; }
    double toneFreq() { return avgPulseDuration; }
  private:
    int pin;
    int analogThreshold;
    int state = HIGH;
    int lastStableState = HIGH;
    unsigned long debounceMs = 0;
    unsigned long stableStartMs = 0;
    unsigned long stableMs = 0;
    bool pressed = false;
    bool doublePressed = false;
    double avgPulseDuration = 2000.0;
    int UP = HIGH;
    int DOWN = LOW;
};
