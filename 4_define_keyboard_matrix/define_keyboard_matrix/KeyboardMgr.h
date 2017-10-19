#ifndef _KEYBOARD_MGR_H_
#define _KEYBOARD_MGR_H_
#define BAUD_RATE 57600

#include <SoftwareSerial.h>

typedef struct {
    bool pressed;
    uint8_t matrix_idx;
} ParsedData;

class KeyboardMgr {
public:
    KeyboardMgr(int Rx_pin, int Tx_pin);
    void begin();
    void exec();
private:
    bool read_from_left();
    SoftwareSerial *left_ble;
    ParsedData left_data;
    char key_def[84];
};

#endif
