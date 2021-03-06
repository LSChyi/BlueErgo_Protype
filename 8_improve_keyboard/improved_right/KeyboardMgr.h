#ifndef _KEYBOARD_MGR_H_
#define _KEYBOARD_MGR_H_
#define BAUD_RATE 9600

#include <SoftwareSerial.h>
#include "KeyBtn.h"
#include "trackball.h"

typedef struct {
    bool pressed;
    uint8_t matrix_idx;
} ParsedData;

class KeyboardMgr {
public:
    KeyboardMgr(int ble_Rx_pin, int ble_Tx_pin, int rn42_Rx_pin, int rn42_Tx_pin, byte _mcp23018_addr, int left_pin, int right_pin, int up_pin, int down_pin, int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin);
    void begin(bool is_usb);
    void exec();
private:
    bool read_from_left();
    bool col_decoder(byte val, uint8_t col);
    void control_row(byte val);
    void processKeyEvent(ParsedData data);
    SoftwareSerial *left_ble;
    SoftwareSerial *rn42;
    Trackball *trackball;
    ParsedData left_data;
    ParsedData right_data;
    char *current_layer;
    char layer1[84];
    char layer2[84];
    byte mcp23018_addr;
    KeyBtn keys[7][6];
    bool is_usb;
};

#endif
