// -----------------------------------------------------------------------------------
// ST4 aux port

#include "St4Aux.h"

#if defined(ST4_AUX_W_PIN) && defined(ST4_AUX_S_PIN) && defined(ST4_AUX_N_PIN) && defined(ST4_AUX_E_PIN)

void AuxST4::init() {
  if (ready) return;

  n = new Button(ST4_AUX_N_PIN, INPUT_PULLUP, LOW | HYST(debounceMs));
  s = new Button(ST4_AUX_S_PIN, INPUT_PULLUP, LOW | HYST(debounceMs));
  e = new Button(ST4_AUX_E_PIN, INPUT_PULLUP, LOW | HYST(debounceMs));
  w = new Button(ST4_AUX_W_PIN, INPUT_PULLUP, LOW | HYST(debounceMs));

  ready = true;
}

void AuxST4::poll() {
  if (!ready) return;

  n->poll();
  s->poll();
  e->poll();
  w->poll();
}

AuxST4 auxST4;

#endif
