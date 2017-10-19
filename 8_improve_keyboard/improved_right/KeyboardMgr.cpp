#include <Wire.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "KeyboardMgr.h"

#define TOGGLE_LAYER_2  219
#define MOUSE_LEFT      220
#define RELEASE_ALL     221

byte row_codes[] = { 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

KeyboardMgr::KeyboardMgr(int ble_Rx_pin, int ble_Tx_pin, int rn42_Rx_pin, int rn42_Tx_pin, byte _mcp23018_addr, int left_pin, int right_pin, int up_pin, int down_pin, int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin) {
    left_ble = new SoftwareSerial(ble_Rx_pin, ble_Tx_pin);
    rn42 = new SoftwareSerial(rn42_Rx_pin, rn42_Tx_pin);
    trackball = new Trackball(left_pin, right_pin, up_pin, down_pin, red_led_pin, blue_led_pin, green_led_pin, white_led_pin);
    mcp23018_addr = _mcp23018_addr;
    current_layer = layer1;

    layer1[0] = KEY_ESC;
    layer2[0] = KEY_ESC;
    layer1[1] = '1';
    layer2[1] = KEY_F1;
    layer1[2] = '2';
    layer2[2] = KEY_F2;
    layer1[3] = '3';
    layer2[3] = KEY_F3;
    layer1[4] = '4';
    layer2[4] = KEY_F4;
    layer1[5] = '5';
    layer2[5] = KEY_F5;
    layer1[6] = '`';
    layer2[6] = KEY_F11;
    layer1[14] = KEY_TAB;
    layer2[14] = KEY_TAB;
    layer1[15] = 'q';
    layer1[16] = 'w';
    layer1[17] = 'e';
    layer1[18] = 'r';
    layer1[19] = 't';
    layer1[20] = KEY_LEFT_GUI;
    layer2[20] = KEY_LEFT_GUI;
    layer1[28] = KEY_LEFT_CTRL;
    layer2[28] = KEY_LEFT_CTRL;
    layer1[29] = 'a';
    layer1[30] = 's';
    layer1[31] = 'd';
    layer1[32] = 'f';
    layer2[32] = '~';
    layer1[33] = 'g';
    layer1[42] = KEY_LEFT_SHIFT;
    layer2[42] = KEY_LEFT_SHIFT;
    layer1[43] = 'z';
    layer1[44] = 'x';
    layer1[45] = 'c';
    layer1[46] = 'v';
    layer1[47] = 'b';
    layer1[48] = KEY_LEFT_ALT;
    layer2[48] = KEY_LEFT_ALT;
    layer1[59] = KEY_LEFT_CTRL;
    layer2[59] = KEY_LEFT_CTRL;
    layer1[60] = KEY_LEFT_ALT;
    layer2[60] = KEY_LEFT_ALT;
    layer1[61] = MOUSE_LEFT;
    layer2[61] = MOUSE_LEFT;
    layer1[71] = KEY_ESC;
    layer2[71] = KEY_ESC;
    layer1[72] = KEY_LEFT_GUI;
    layer2[72] = KEY_LEFT_GUI;
    layer1[73] = ' ';
    layer1[74] = KEY_PAGE_DOWN;
    layer2[74] = KEY_PAGE_DOWN;
    layer1[76] = KEY_PAGE_UP;
    layer2[76] = KEY_PAGE_UP;

    layer1[7] = '=';
    layer2[7] = KEY_F12;
    layer1[8] = '6';
    layer2[8] = KEY_F6;
    layer1[9] = '7';
    layer2[9] = KEY_F7;
    layer1[10] = '8';
    layer2[10] = KEY_F8;
    layer1[11] = '9';
    layer2[11] = KEY_F9;
    layer1[12] = '0';
    layer2[12] = KEY_F10;
    layer1[13] = KEY_BACKSPACE;
    layer2[13] = KEY_BACKSPACE;
    layer1[21] = '='; 
    layer1[22] = 'y';
    layer1[23] = 'u';
    layer2[23] = KEY_PAGE_DOWN;
    layer1[24] = 'i';
    layer2[24] = KEY_PAGE_UP;
    layer1[25] = 'o';
    layer1[26] = 'p';
    layer2[26] = '[';
    layer1[27] = '\\';
    layer1[36] = 'h';
    layer2[36] = KEY_LEFT_ARROW;
    layer1[37] = 'j';
    layer2[37] = KEY_DOWN_ARROW;
    layer1[38] = 'k';
    layer2[38] = KEY_UP_ARROW;
    layer1[39] = 'l';
    layer2[39] = KEY_RIGHT_ARROW;
    layer1[40] = ';';
    layer2[40] = KEY_BACKSPACE;
    layer1[41] = '\'';
    layer2[41] = ']';
    layer1[49] = '-';
    layer1[50] = 'n';
    layer1[51] = 'm';
    layer1[52] = ',';
    layer1[53] = '.';
    layer1[54] = '/';
    layer1[55] = KEY_RIGHT_SHIFT;
    layer2[55] = KEY_RIGHT_SHIFT;
    layer1[66] = KEY_LEFT_ARROW;
    layer2[66] = KEY_LEFT_ARROW;
    layer1[67] = KEY_DOWN_ARROW;
    layer2[67] = KEY_DOWN_ARROW;
    layer1[68] = KEY_UP_ARROW;
    layer2[68] = KEY_UP_ARROW;
    layer1[69] = KEY_RIGHT_ARROW;
    layer2[69] = KEY_RIGHT_ARROW;
    layer1[77] = RELEASE_ALL;
    layer2[77] = RELEASE_ALL;
    layer1[78] = KEY_RIGHT_GUI;
    layer2[78] = KEY_RIGHT_GUI;
    layer1[80] = KEY_RETURN;
    layer2[80] = KEY_RETURN;
    layer1[81] = TOGGLE_LAYER_2;
    layer2[81] = TOGGLE_LAYER_2;
    layer1[82] = KEY_RIGHT_SHIFT;
    layer2[82] = KEY_RIGHT_SHIFT;
}

void KeyboardMgr::begin(bool _is_usb) {
    is_usb = _is_usb;
    rn42->begin(BAUD_RATE);
    left_ble->begin(BAUD_RATE);
    left_ble->flush();
    left_ble->listen();
    Keyboard.begin(is_usb, rn42);
    if (is_usb) {
        trackball->begin(false);
    } else {
        trackball->begin(true);
    }
    Mouse.begin(is_usb, rn42);
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
        unsigned long now = millis();

        Wire.beginTransmission(mcp23018_addr);
        Wire.write(0x13);
        Wire.endTransmission();
        Wire.requestFrom(mcp23018_addr, (uint8_t)1);
        byte col_data = Wire.read();
        for (uint8_t col = 0; col < 6; ++col) {
            bool key_state = col_decoder(col_data, col);
            if (keys[row][col].state_changed(key_state, now)) {
                right_data.pressed = key_state;
                right_data.matrix_idx = row + col * 14 + 7;
                for (int i = 0; i < 100; ++i) {
                    if (read_from_left()) {
                        processKeyEvent(left_data);
                        break;
                    }
                }
                processKeyEvent(right_data);
            }
            while (read_from_left()) {
                processKeyEvent(left_data);
            }
        }
    }
    trackball->handleTrackball();
}

void KeyboardMgr::processKeyEvent(ParsedData data) {
    char key_char = current_layer[data.matrix_idx];
    if (key_char != 0) {
        if ((uint8_t)key_char <= 218) {
            if (data.pressed) {
                if ((uint8_t)key_char == KEY_RETURN) {
                    for (int i = 0; i < 900; ++i) {
                        if (read_from_left()) {
                            processKeyEvent(left_data);
                            break;
                        }
                    }
                }
                Keyboard.press(key_char);
            } else {
                Keyboard.release(key_char);
            }
        } else {
            uint8_t special_key = key_char;
            if (special_key == TOGGLE_LAYER_2) {
                for (uint8_t i = 0; i < 6; ++i) {
                    Keyboard._keyReport.keys[i] = 0;
                }
                Keyboard.sendReport(&(Keyboard._keyReport));

                if (data.pressed) {
                    current_layer = layer2;
                } else {
                    current_layer = layer1;
                }
            } else if (special_key == MOUSE_LEFT) {
                if (data.pressed) {
                    Mouse.press();
                } else {
                    Mouse.release();
                }
            } else if (special_key == RELEASE_ALL) {
                Keyboard.releaseAll();
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
        if ((left_data.matrix_idx >= 84) || ((left_data.matrix_idx % 14) > 7)) {
            Serial.println("enter error part");
            return false;
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
