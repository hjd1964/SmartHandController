// -----------------------------------------------------------------------------------------------------------------------------
// Button pad
#include "KeyPad.h"

void KeyPad::init(const Pin pins[7], int thresholdNS, int thresholdEW) {
  if (ready) return;

  if (thresholdNS == 0 && thresholdEW == 0) {
    shift = new Button(pins[0].pinNumber, pins[0].inputMode, pins[0].activeState | HYST(debounceMs));
    n     = new Button(pins[1].pinNumber, pins[1].inputMode, pins[1].activeState | HYST(debounceMs));
    s     = new Button(pins[2].pinNumber, pins[2].inputMode, pins[2].activeState | HYST(debounceMs));
    e     = new Button(pins[3].pinNumber, pins[3].inputMode, pins[3].activeState | HYST(debounceMs));
    w     = new Button(pins[4].pinNumber, pins[4].inputMode, pins[4].activeState | HYST(debounceMs));
    F     = new Button(pins[5].pinNumber, pins[5].inputMode, pins[5].activeState | HYST(debounceMs));
    f     = new Button(pins[6].pinNumber, pins[6].inputMode, pins[6].activeState | HYST(debounceMs));
  } else {
    int active1 = pins[1].activeState;
    int active3 = pins[3].activeState;
    if (active1 == LOW) active1 = HIGH; else if (active1 == HIGH) active1 = LOW;
    if (active3 == LOW) active3 = HIGH; else if (active3 == HIGH) active3 = LOW;
    shift = new Button(pins[0].pinNumber, pins[0].inputMode, pins[0].activeState | HYST(debounceMs));
    n     = new Button(pins[1].pinNumber, pins[1].inputMode, active1   | THLD(thresholdNS) | HYST(KEYPAD_JOYSTICK_HYSTERESIS));
    s     = new Button(pins[2].pinNumber, pins[2].inputMode, pins[2].activeState | THLD(thresholdNS) | HYST(KEYPAD_JOYSTICK_HYSTERESIS));
    e     = new Button(pins[3].pinNumber, pins[3].inputMode, active3   | THLD(thresholdEW) | HYST(KEYPAD_JOYSTICK_HYSTERESIS));
    w     = new Button(pins[4].pinNumber, pins[4].inputMode, pins[4].activeState | THLD(thresholdEW) | HYST(KEYPAD_JOYSTICK_HYSTERESIS));
    F     = new Button(pins[5].pinNumber, pins[5].inputMode, pins[5].activeState | HYST(debounceMs));
    f     = new Button(pins[6].pinNumber, pins[6].inputMode, pins[6].activeState | HYST(debounceMs));
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

KeyPad keyPad;
