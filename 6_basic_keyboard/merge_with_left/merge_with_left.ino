#include "KeyboardMgr.h"

KeyboardMgr keyboardMgr(8, 7, 0x20);

void setup() {
    delay(200);
    keyboardMgr.begin();
}

void loop() {
    keyboardMgr.exec();
}
