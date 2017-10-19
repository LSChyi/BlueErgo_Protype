#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "direction.h"
#include "trackballLed.h"

class Trackball {
public:
    Trackball(int left_pin, int right_pin, int up_pin, int down_pin, int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin);
    void begin(bool _power_saving);
    void handleTrackball();
private:
    Direction *x_direction;
    Direction *y_direction;
    TrackballLed *trackballLed;
    int x_move;
    int y_move;
    bool acted;
    bool power_saving;
    unsigned long int last_action_time;
};

#endif
