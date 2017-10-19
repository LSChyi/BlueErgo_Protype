#define BAUD_RATE 9600
#include <SoftwareSerial.h>

#define RX_PIN 4
#define TX_PIN 9

int8_t row_pins[] = { A3, 2, 3, 5, 6, 7, 8 };
int8_t col_pins[] = { A2, A1, A0, 13, 12, 11 };
SoftwareSerial BT(TX_PIN, RX_PIN);

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    BT.begin(BAUD_RATE);
}

void loop() {
    for (int8_t row_pin : row_pins) {
        // select row
        digitalWrite(row_pin, LOW);

        for (int8_t col_pin : col_pins) {
            if (digitalRead(col_pin) == LOW) {
                BT.print(row_pin);
                BT.print(" ");
                BT.print(col_pin);
                BT.println(" is closed");
            }
        }
        
        // release row
        digitalWrite(row_pin, HIGH);
    }
}
