#include <U8g2lib.h>
#include "u8g2_ext_event.h"

#include "../../../Config.h"
#include "../../Constants.h"
#include "../../lib/tasks/OnTask.h"

uint8_t ext_GetMenuEvent(KeyPad* extPad) {
  // allow higher priority tasks to run, like the keyPad polling
  Y;

  if (extPad->shift->wasClicked()) return  U8X8_MSG_GPIO_MENU_HOME;
  if ((extPad->n->isDown() && (extPad->n->timeDown() > 1000)) || extPad->n->wasClicked()) {
    extPad->n->clearPress();

    unsigned long t = extPad->n->timeDown();
    if (t > 1000) {
      if (t > 6000) t = 6000;
      t = (5000 - (t - 1000))/20 + 10;
      tasks.yield(t);
    }

    if (extPad->n->timeDown() > 5000) return MSG_MENU_UP_FAST; else return U8X8_MSG_GPIO_MENU_UP;
  }
  if ((extPad->s->isDown() && (extPad->s->timeDown() > 1000)) || extPad->s->wasClicked()) {
    extPad->s->clearPress();

    unsigned long t = extPad->s->timeDown();
    if (t > 1000) {
      if (t > 6000) t = 6000;
      t = (5000 - (t - 1000))/20 + 10;
      tasks.yield(t);
    }

    if (extPad->s->timeDown() > 5000) return MSG_MENU_DOWN_FAST; else return U8X8_MSG_GPIO_MENU_DOWN;
  }
  if (extPad->e->wasClicked()) return U8X8_MSG_GPIO_MENU_PREV;
  if (extPad->w->wasClicked()) return U8X8_MSG_GPIO_MENU_NEXT;
  if (extPad->F->wasClicked()) return U8X8_MSG_GPIO_MENU_SELECT;
  if (extPad->f->wasClicked()) return U8X8_MSG_GPIO_MENU_HOME;
  return 0;
}
