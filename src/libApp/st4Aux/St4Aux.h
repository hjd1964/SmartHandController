// -----------------------------------------------------------------------------------
// ST4 aux port
#pragma once

#include "../../Common.h"

#if defined(ST4_AUX_W_PIN) && defined(ST4_AUX_S_PIN) && defined(ST4_AUX_N_PIN) && defined(ST4_AUX_E_PIN)

#include "../../buttons/PushButton.h"

class AuxST4 {
  public:
    void setup();
    void poll();
    button *n, *s, *e, *w;
  private:
    int debounceMs = 20;
    bool ready = false;
};

extern AuxST4 auxST4;

#endif
