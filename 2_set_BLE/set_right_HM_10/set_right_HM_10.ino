#define BAUD_RATE 9600
#include <SoftwareSerial.h>

SoftwareSerial BT(8, 7);

void setup() {
    delay(200);
    Serial.begin(BAUD_RATE);
    BT.begin(BAUD_RATE);
    Serial.println("Simple HM-10 test");
}

void loop() {
    if (BT.available()) {
        Serial.print(char(BT.read()));
    }

    if (Serial.available()) {
        BT.write(char(Serial.read()));
    }
}
