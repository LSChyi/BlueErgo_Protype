#ifndef TRACKBALL_LED_H
#define TRACKBALL_LED_H

#include "Arduino.h"
#define brightness_interval   30
#define lit_stop_interval     60000
#define total_led             4

class TrackballLed {
public:
    TrackballLed(int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin);
    void handleLight(unsigned long int last_action_time);
private:
    int led_order[total_led];
    unsigned long led_last_change_time;
    int brightness;
    int brightness_step;
    int current_led;
    int current_led_order;
};
#endif
