#include <Wire.h>

byte mcp23018 = 0x20;
byte row_codes[] = { 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

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
    while(!Serial);
    Serial.println("right keyboard matrix with mcp23018");
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
            if (key_state) {
                Serial.print(row);
                Serial.print(" ");
                Serial.print(col);
                Serial.println(" is closed");
            }
        }

        // release row by setting all pins high
        control_row(mcp23018, 0xFF);
    }
}
