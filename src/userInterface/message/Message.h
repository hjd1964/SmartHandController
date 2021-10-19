
#include "../../libApp/u8g2ext/u8g2_ext.h"
#include "../../libApp/keyPad/KeyPad.h"
#include "../../libApp/cmd/Cmd.h"

class Message {
  public:
    void init(U8G2_EXT *display);

    void show(const char* txt1, const char* txt2, int duration);
    void show(const char* txt1, const char* txt2, const char* txt3, int duration);
    void show(const char* txt1, const char* txt2, const char* txt3, const char* txt4, int duration);
    bool show(CMD_RESULT cmdResult, bool silentOk = true);

    void brief(const char* txt1, int duration = 1000);
    bool briefShowing();
    void briefShow();

  private:
    U8G2_EXT *display;
    char briefMessage[32] = "";
    bool showing = false;
    unsigned long briefMessageEndTime = 0;
};

extern Message message;