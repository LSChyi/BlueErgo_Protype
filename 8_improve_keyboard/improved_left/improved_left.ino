#define BAUD_RATE 9600
#define CYCLE 4
#include "KeyBtn.h"
#include <SoftwareSerial.h>
#include <QueueArray.h>

#define RX_PIN 4
#define TX_PIN 9

int8_t row_pins[] = { A3, 2, 3, 5, 6, 7, 8 };
int8_t col_pins[] = { 11, 12, 13, A0, A1, A2 };
SoftwareSerial BT(TX_PIN, RX_PIN);
QueueArray <uint8_t> queue;
uint8_t current_cycle = 0;
uint8_t send_buf[100];

KeyBtn keys[7][6];

void fasterWrite (int8_t pin, bool state) {
    if (pin == A3 || pin == 8) {
        if (pin == A3) {
            if (state) {
                PORTC |= _BV(PC3);
            } else {
                PORTC &= ~_BV(PC3);
            }
        } else {
            if (state) {
                PORTB |= _BV(PB0);
            } else {
                PORTB &= ~_BV(PB0);
            }
        }
    } else {
        switch (pin) {
            case 2:
                if (state) {
                    PORTD |= _BV(PD2);
                } else {
                    PORTD &= ~_BV(PD2);
                }
                break;
            case 3:
                if (state) {
                    PORTD |= _BV(PD3);
                } else {
                    PORTD &= ~_BV(PD3);
                }
                break;
            case 5:
                if (state) {
                    PORTD |= _BV(PD5);
                } else {
                    PORTD &= ~_BV(PD5);
                }
                break;
            case 6:
                if (state) {
                    PORTD |= _BV(PD6);
                } else {
                    PORTD &= ~_BV(PD6);
                }
                break;
            case 7:
                if (state) {
                    PORTD |= _BV(PD7);
                } else {
                    PORTD &= ~_BV(PD7);
                }
                break;
        }
    }
}

bool fasterRead(int8_t pin) {
    if (pin == 11 || pin == 12 || pin == 13) {
        switch (pin) {
            case 11:
                return (0x01 & (PINB >> 3));
            case 12:
                return (0x01 & (PINB >> 4));
            case 13:
                return (0x01 & (PINB >> 5));
        }
    } else {
        switch (pin) {
            case A0:
                return (0x01 & PINC);
            case A1:
                return (0x01 & PINC >> 1);
            case A2:
                return (0x01 & PINC >> 2);
        }
    }
}

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    BT.begin(BAUD_RATE);
    Serial.begin(9600);
}

void loop() {
    current_cycle = (current_cycle + 1) % CYCLE;
    for (int8_t row = 0; row < 7; ++row) {
        // select row
        //digitalWrite(row_pins[row], LOW);
        fasterWrite(row_pins[row], LOW);
        
        unsigned long now = millis();
        for (int8_t col = 0; col < 6; ++col) {
            //bool key_state = digitalRead(col_pins[col]);
            bool key_state = fasterRead(col_pins[col]);
            if (keys[row][col].state_changed(key_state, now)) {
                uint8_t matrix_idx = row + col * 14;
                if (key_state == HIGH) {
                    matrix_idx |= 0b10000000;
                }
                queue.push(matrix_idx);
            }
        }

        // release row
        //digitalWrite(row_pins[row], HIGH);
        fasterWrite(row_pins[row], HIGH);
    }
    if (current_cycle == 0) {
        uint8_t queue_size = queue.count();
        for (int i = 0; i < queue_size; ++i) {
            send_buf[i] = queue.pop();
        }
        BT.write(send_buf, queue_size);
    }
}
