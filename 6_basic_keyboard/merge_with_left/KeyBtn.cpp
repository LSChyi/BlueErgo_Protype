#include <Arduino.h>
#include "KeyBtn.h"

KeyBtn::KeyBtn() {
    last_act_time = 0;
    last_state = HIGH; // treat every key is unpressed at boot
}

bool KeyBtn::state_changed(bool read_state) {
    if (read_state != last_state) {
        unsigned long now = millis();
        if (now - last_act_time > BOUNCE_TIME) {
            last_act_time = now;
            last_state = read_state;
            return true;
        }
    }
    return false;
}
