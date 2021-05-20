// -----------------------------------------------------------------------------------
// ST4 aux port

#include "../st4/St4Aux.h"
#include "../buttons/PushButton.h"

#if defined(ST4AuxRAw) && defined(ST4AuxDEs) && defined(ST4AuxDEn) && defined(ST4AuxRAe)

void Ast4::tickButtons() {
  n.poll();
  s.poll();
  e.poll();
  w.poll();
}

void Ast4::setup() {
  n.init(ST4AuxDEn, debounceMs, false, 0);
  s.init(ST4AuxDEs, debounceMs, false, 0);
  e.init(ST4AuxRAe, debounceMs, false, 0);
  w.init(ST4AuxRAw, debounceMs, false, 0);
}

Ast4 auxST4;

#endif
