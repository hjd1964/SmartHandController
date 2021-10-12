// -----------------------------------------------------------------------------------------------------------------------------
// Button pad
#include "KeyPad.h"

#include "../../Common.h"

void KeyPad::setup(const int pin[7], const int active[7], int thresholdNS, int thresholdEW) {
  if (ready) return;

  if (thresholdNS == 0 && thresholdEW == 0) {
    shift = new Button(pin[0], INPUT_PULLUP, active[0] | HYST(debounceMs));
    n     = new Button(pin[1], INPUT_PULLUP, active[1] | HYST(debounceMs));
    s     = new Button(pin[2], INPUT_PULLUP, active[2] | HYST(debounceMs));
    e     = new Button(pin[3], INPUT_PULLUP, active[3] | HYST(debounceMs));
    w     = new Button(pin[4], INPUT_PULLUP, active[4] | HYST(debounceMs));
    F     = new Button(pin[5], INPUT_PULLUP, active[5] | HYST(debounceMs));
    f     = new Button(pin[6], INPUT_PULLUP, active[6] | HYST(debounceMs));
  } else {
    shift = new Button(pin[0], INPUT_PULLUP, active[0] | THLD( thresholdNS) | HYST(300));
    n     = new Button(pin[1], INPUT_PULLUP, active[1] | THLD(-thresholdNS) | HYST(300));
    s     = new Button(pin[2], INPUT_PULLUP, active[2] | THLD( thresholdEW) | HYST(300));
    e     = new Button(pin[3], INPUT_PULLUP, active[3] | THLD(-thresholdEW) | HYST(300));
    w     = new Button(pin[4], INPUT_PULLUP, active[4] | HYST(debounceMs));
    F     = new Button(pin[5], INPUT_PULLUP, active[5] | HYST(debounceMs));
    f     = new Button(pin[6], INPUT_PULLUP, active[6] | HYST(debounceMs));
  }

  ready = true;
}

void KeyPad::poll() {
  if (!ready) return;

  shift->poll();
  n->poll();
  s->poll();
  e->poll();
  w->poll();
  F->poll();
  f->poll();
}

bool KeyPad::anyPressed() {
  if (!ready) return false;

  if (shift->isDown() ||
      n->isDown() ||
      s->isDown() ||
      e->isDown() ||
      w->isDown() ||
      f->isDown() ||
      F->isDown()) return true;
  if (shift->wasPressed(true) ||
      n->wasPressed(true) ||
      s->wasPressed(true) ||
      e->wasPressed(true) ||
      w->wasPressed(true) ||
      f->wasPressed(true) ||
      F->wasPressed(true)) return true;
  return false;
}

bool KeyPad::nsewPressed() {
  if (!ready) return false;

  if (n->isDown() ||
      s->isDown() ||
      e->isDown() ||
      w->isDown()) return true;
  if (n->wasPressed(true) ||
      s->wasPressed(true) ||
      e->wasPressed(true) ||
      w->wasPressed(true)) return true;
  return false;
}

void KeyPad::waitForPress() {
  if (!ready) return;

  for (;;) { poll(); delay(5); if (anyPressed()) break; }
}

void KeyPad::clearAllPressed() {
  if (!ready) return;

  shift->clearPress();
  n->clearPress();
  s->clearPress();
  e->clearPress();
  w->clearPress();
  F->clearPress();
  f->clearPress();
}
