#ifndef _KEYBOARD_MGR_H_
#define _KEYBOARD_MGR_H_
#define BAUD_RATE 9600

#include <SoftwareSerial.h>
#include "KeyBtn.h"

typedef struct {
    bool pressed;
    uint8_t matrix_idx;
} ParsedData;

class KeyboardMgr {
public:
    KeyboardMgr(int Rx_pin, int Tx_pin, byte _mcp23018_addr);
    void begin();
    void exec();
private:
    bool read_from_left();
    bool col_decoder(byte val, uint8_t col);
    void control_row(byte val);
    void processKeyEvent(ParsedData data);
    SoftwareSerial *left_ble;
    ParsedData left_data;
    ParsedData right_data;
    char key_def[84];
    byte mcp23018_addr;
    KeyBtn keys[7][6];
};

#endif
