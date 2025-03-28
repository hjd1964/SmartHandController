// -----------------------------------------------------------------------------------------------------------------------------
// Button pad
#pragma once

#include "../../Common.h"
#include "../../lib/pushButton/PushButton.h"

class KeyPad {
public:
  struct Pin {
    int pinNumber;
    int activeState;
    int inputMode;
  };
  void init(const Pin pins[7], int thresholdNS, int thresholdEW);

  void poll();
  bool anyPressed();
  bool nsewPressed();  
  void waitForPress();
  void clearAllPressed();

  Button *shift, *n, *s, *e, *w, *F, *f;

private:
  int debounceMs = BUTTON_DEBOUNCE_MS;
  bool ready = false;
};

extern KeyPad keyPad;
