#include "KeyBtn.h"

int8_t row_pins[] = { A3, 2, 3, 5, 6, 7, 8 };
int8_t col_pins[] = { 11, 12, 13, A0, A1, A2 };

KeyBtn keys[7][6];

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    Serial.begin(9600);
    Serial.println("Test key with debounce key");
}

void loop() {
    for (int8_t row = 0; row < 7; ++row) {
        // select row
        digitalWrite(row_pins[row], LOW);
        
        for (int8_t col = 0; col < 6; ++col) {
            bool key_state = digitalRead(col_pins[col]);
            if (keys[row][col].state_changed(key_state)) {
                Serial.print(row);
                Serial.print(" ");
                Serial.print(col);
                if (key_state == HIGH) {
                    Serial.println(" is released");
                } else {
                    Serial.println(" is pressed");
                }
            }
        }

        // release row
        digitalWrite(row_pins[row], HIGH);
    }
}
