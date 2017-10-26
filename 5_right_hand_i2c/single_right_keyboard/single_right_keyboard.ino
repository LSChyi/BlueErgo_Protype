#include <Wire.h>
#include <Keyboard.h>
#include "KeyBtn.h"

byte mcp23018 = 0x20;
byte row_codes[] = { 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
KeyBtn keys[7][6];
char key_def[84];

bool col_decoder(byte val, uint8_t col) {
    return !(0x01 & (val >> col));
}

void control_row(byte address, byte val) {
    Wire.beginTransmission(address);
    Wire.write(0x12);
    Wire.write(val);
    Wire.endTransmission();
}

void setup() {
    Serial.begin(9600);
    Keyboard.begin();

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

    Wire.begin();
    Wire.setClock(400000UL); // set I2C to 400khz

    // set group A pins
    Wire.beginTransmission(mcp23018);
    // select register IODIRA
    Wire.write(0x00);
    // set 0b00000000 means all pins are set as output
    Wire.write(0x00);
    Wire.endTransmission();

    // set group B pins
    Wire.beginTransmission(mcp23018);
    // select register IODIRB
    Wire.write(0x01);
    // set 0b11111111 means all pins are set as input
    Wire.write(0xFF);
    Wire.endTransmission();

    // wire pull up resisters to all B pins
    Wire.beginTransmission(mcp23018);
    // select register GPPUB
    Wire.write(0x0D);
    // set 0b11111111 means all resisters are connect to IODIRB
    Wire.write(0xFF);
    Wire.endTransmission();
}

void loop() {
    for (uint8_t row = 0; row < 7; ++row) {
        // select row by sending the corresponding code
        control_row(mcp23018, row_codes[row]);

        Wire.beginTransmission(mcp23018);
        Wire.write(0x13);
        Wire.endTransmission();
        Wire.requestFrom(mcp23018, (uint8_t)1);
        byte col_data = Wire.read();
        for (uint8_t col = 0; col < 6; ++col) {
            bool key_state = col_decoder(col_data, col);
            if (keys[row][col].state_changed(key_state)) {
                uint8_t matrix_idx = row + col * 14 + 7;
                if (key_state) {
                    Keyboard.press(key_def[matrix_idx]);
                } else {
                    Keyboard.release(key_def[matrix_idx]);
                }
                Serial.println(matrix_idx);
            }
        }
    }
}
