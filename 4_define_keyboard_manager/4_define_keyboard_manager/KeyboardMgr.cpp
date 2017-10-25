#include <Keyboard.h>
#include "KeyboardMgr.h"

KeyboardMgr::KeyboardMgr(int Rx_pin, int Tx_pin) {
    left_ble = new SoftwareSerial(Rx_pin, Tx_pin);
    key_def[0] = KEY_ESC;
    key_def[1] = '1';
    key_def[2] = '2';
    key_def[3] = '3';
    key_def[4] = '4';
    key_def[5] = '5';
    key_def[6] = '`';
    key_def[14] = KEY_TAB;
    key_def[15] = 'q';
    key_def[16] = 'w';
    key_def[17] = 'e';
    key_def[18] = 'r';
    key_def[19] = 't';
    key_def[20] = KEY_LEFT_GUI;
    key_def[28] = KEY_LEFT_CTRL;
    key_def[29] = 'a';
    key_def[30] = 's';
    key_def[31] = 'd';
    key_def[32] = 'f';
    key_def[33] = 'g';
    key_def[42] = KEY_LEFT_SHIFT;
    key_def[43] = 'z';
    key_def[44] = 'x';
    key_def[45] = 'c';
    key_def[46] = 'v';
    key_def[47] = 'b';
    key_def[48] = KEY_LEFT_ALT;
    key_def[59] = KEY_LEFT_CTRL;
    key_def[60] = KEY_LEFT_ALT;
    key_def[71] = KEY_ESC;
    key_def[72] = KEY_LEFT_GUI;
    key_def[73] = ' ';
    key_def[74] = KEY_PAGE_DOWN;
    key_def[76] = KEY_PAGE_UP;
}

void KeyboardMgr::begin() {
    left_ble->begin(BAUD_RATE);
    Keyboard.begin();
}

void KeyboardMgr::exec() {
    while (read_from_left()) {
        char key_char = key_def[left_data.matrix_idx];
        if (key_char != 0) {
            if (left_data.pressed) {
                Keyboard.press(key_char);
            } else {
                Keyboard.release(key_char);
            }
        }
    }
}

bool KeyboardMgr::read_from_left() {
    if (left_ble->available()) {
        uint8_t recv = left_ble->read();
        if (recv & 0b10000000) {
            left_data.pressed = false;
            left_data.matrix_idx = (recv & 0b01111111);
        } else {
            left_data.pressed = true;
            left_data.matrix_idx = recv;
        }
        return true;
    } else {
        return false;
    }
}
