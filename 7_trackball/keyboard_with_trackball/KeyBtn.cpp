#include <Arduino.h>
#include "KeyBtn.h"

KeyBtn::KeyBtn() {
    last_act_time = 0;
    last_state = HIGH; // treat every key is unpressed at boot
}

bool KeyBtn::state_changed(bool read_state, unsigned long row_start) {
    if (read_state != last_state) {
        if (row_start - last_act_time > BOUNCE_TIME) {
            last_act_time = row_start;
            last_state = read_state;
            return true;
        }
    }
    return false;
}
