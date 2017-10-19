int8_t row_pins[] = { A3, 2, 3, 5, 6, 7, 8 };
int8_t col_pins[] = { A2, A1, A0, 13, 12, 11 };

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    Serial.begin(9600);
    Serial.println("Test left circuits");
}

void loop() {
    for (int8_t row_pin : row_pins) {
        // select row
        digitalWrite(row_pin, LOW);

        for (int8_t col_pin : col_pins) {
            if (digitalRead(col_pin) == LOW) {
                Serial.print(row_pin);
                Serial.print(" ");
                Serial.print(col_pin);
                Serial.println(" is closed");
            }
        }
        
        // release row
        digitalWrite(row_pin, HIGH);
    }
}
