#include "TrackballLed.h"

TrackballLed::TrackballLed(int red_led_pin, int blue_led_pin, int green_led_pin, int white_led_pin) {
    led_last_change_time = 0;
    led_order[0] = red_led_pin;
    led_order[1] = blue_led_pin;
    led_order[2] = green_led_pin;
    led_order[3] = white_led_pin;
    brightness = 0;
    brightness_step = 6;
    current_led = led_order[0];
    current_led_order = 0;
    for (int i = 0; i < total_led; ++i) {
        pinMode(led_order[i], OUTPUT);
        digitalWrite(led_order[i], LOW);
    }
}

void TrackballLed::handleLight(unsigned long int last_action_time) {
    if(millis() - last_action_time < lit_stop_interval) {
        if(millis() - led_last_change_time > brightness_interval) {
            brightness += brightness_step;
            brightness = (brightness > 255) ? 255 : brightness;
            brightness = (brightness < 0) ? 0 : brightness;
            if (brightness == 0 || brightness == 255) {
                brightness_step *= -1;
                if (brightness == 0) {
                    analogWrite(current_led, 0);
                    current_led_order = (current_led_order + 1) % total_led;
                    current_led = led_order[current_led_order];
                }
            }
            analogWrite(current_led, brightness);
            led_last_change_time = millis();
        }
    } else {
        for(int i = 0; i < total_led; ++i) {
            analogWrite(led_order[i], 0);
        }
    }
}
