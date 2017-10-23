#define BAUD_RATE 19200
#include "KeyBtn.h"
#include <CustomSoftwareSerial.h>

#define RX_PIN 4
#define TX_PIN 9

int8_t row_pins[] = { A3, 2, 3, 5, 6, 7, 8 };
int8_t col_pins[] = { 11, 12, 13, A0, A1, A2 };
CustomSoftwareSerial BT(TX_PIN, RX_PIN);

KeyBtn keys[7][6];

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    BT.begin(BAUD_RATE, CSERIAL_8E2);
}

void loop() {
    for (int8_t row = 0; row < 7; ++row) {
        // select row
        digitalWrite(row_pins[row], LOW);
        
        for (int8_t col = 0; col < 6; ++col) {
            bool key_state = digitalRead(col_pins[col]);
            if (keys[row][col].state_changed(key_state)) {
                uint8_t matrix_idx = row + col * 14;
                if (key_state == HIGH) {
                    matrix_idx |= 0b10000000;
                }
                BT.print((char)matrix_idx);
            }
        }

        // release row
        digitalWrite(row_pins[row], HIGH);
    }
}
