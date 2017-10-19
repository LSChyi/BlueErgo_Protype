#include "Mouse.h"
#include "trackball.h"
#include "Arduino.h"

Trackball::Trackball(int left_pin, int right_pin, int up_pin, int down_pin, int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin) {
    last_action_time = 0;
    x_direction = new Direction(left_pin, right_pin);
    y_direction = new Direction(up_pin, down_pin);
    trackballLed = new TrackballLed(red_led_pin, blue_led_pin, green_led_pin, white_led_pin);
}

void Trackball::begin(bool _power_saving) {
    power_saving = _power_saving;
}

void Trackball::handleTrackball() {
    // handle mouse move
    x_move = x_direction->read_action();
    y_move = y_direction->read_action();
    if (x_move != 0 || y_move != 0) {
        Mouse.move(x_move, y_move, 0);
        last_action_time = millis();
    }

    if (!power_saving) {
        // handle led event
        trackballLed->handleLight(last_action_time);
    }
}
