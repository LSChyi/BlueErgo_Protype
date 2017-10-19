#include "KeyboardMgr.h"
#define LEFT_PIN    A1
#define RIGHT_PIN   4
#define UP_PIN      16
#define DOWN_PIN    A2
#define BLUE_PIN    6
#define RED_PIN     5
#define GREEN_PIN   9
#define WHITE_PIN   10

KeyboardMgr keyboardMgr(8, 7, 15, 14, 0x20, LEFT_PIN, RIGHT_PIN, UP_PIN, DOWN_PIN, BLUE_PIN, RED_PIN, GREEN_PIN, WHITE_PIN);

void setup() {
    delay(300);
    if (UDADDR & _BV(ADDEN)) {
        keyboardMgr.begin(true);
    } else {
        keyboardMgr.begin(false);
    }
    Serial.begin(19200);
}

void loop() {
    keyboardMgr.exec();
}
