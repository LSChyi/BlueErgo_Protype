#include "KeyboardMgr.h"

KeyboardMgr keyboardMgr(8, 7);

void setup() {
    keyboardMgr.begin();
}

void loop() {
    keyboardMgr.exec();
}

