#include <Keyboard.h>
#include <Wire.h>
#include "KeyboardMgr.h"

byte row_codes[] = { 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

KeyboardMgr::KeyboardMgr(int Rx_pin, int Tx_pin, byte _mcp23018_addr) {
    left_ble = new SoftwareSerial(Rx_pin, Tx_pin);
    mcp23018_addr = _mcp23018_addr;
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

    key_def[7] = '=';
    key_def[8] = '6';
    key_def[9] = '7';
    key_def[10] = '8';
    key_def[11] = '9';
    key_def[12] = '0';
    key_def[13] = KEY_BACKSPACE;
    key_def[21] = '=';
    key_def[22] = 'y';
    key_def[23] = 'u';
    key_def[24] = 'i';
    key_def[25] = 'o';
    key_def[26] = 'p';
    key_def[27] = '\\';
    key_def[36] = 'h';
    key_def[37] = 'j';
    key_def[38] = 'k';
    key_def[39] = 'l';
    key_def[40] = ';';
    key_def[41] = '\'';
    key_def[49] = '-';
    key_def[50] = 'n';
    key_def[51] = 'm';
    key_def[52] = ',';
    key_def[53] = '.';
    key_def[54] = '/';
    key_def[55] = KEY_RIGHT_SHIFT;
    key_def[66] = KEY_LEFT_ARROW;
    key_def[67] = KEY_DOWN_ARROW;
    key_def[68] = KEY_UP_ARROW;
    key_def[69] = KEY_RIGHT_ARROW;
    key_def[78] = KEY_RIGHT_GUI;
    key_def[80] = KEY_RETURN;
    key_def[82] = KEY_RIGHT_SHIFT;
}

void KeyboardMgr::begin() {
    left_ble->begin(BAUD_RATE);
    Keyboard.begin();
    Wire.begin();
    Wire.setClock(400000UL);

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x01);
    Wire.write(0xFF);
    Wire.endTransmission();

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x0D);
    Wire.write(0xFF);
    Wire.endTransmission();
}

void KeyboardMgr::exec() {
    for (uint8_t row = 0; row < 7; ++row) {
        control_row(row_codes[row]);

        Wire.beginTransmission(mcp23018_addr);
        Wire.write(0x13);
        Wire.endTransmission();
        Wire.requestFrom(mcp23018_addr, (uint8_t)1);
        byte col_data = Wire.read();
        for (uint8_t col = 0; col < 6; ++col) {
            bool key_state = col_decoder(col_data, col);
            if (keys[row][col].state_changed(key_state)) {
                right_data.pressed = key_state;
                right_data.matrix_idx = row + col * 14 + 7;
                processKeyEvent(right_data);
            }
        }

        while (read_from_left()) {
            processKeyEvent(left_data);
        }
    }
}

void KeyboardMgr::processKeyEvent(ParsedData data) {
    char key_char = key_def[data.matrix_idx];
    if (key_char != 0) {
        if (data.pressed) {
            Keyboard.press(key_char);
        } else {
            Keyboard.release(key_char);
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

bool KeyboardMgr::col_decoder(byte val, uint8_t col) {
    return !(0x01 & (val >> col));
}

void KeyboardMgr::control_row(byte val) {
    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x12);
    Wire.write(val);
    Wire.endTransmission();
}
