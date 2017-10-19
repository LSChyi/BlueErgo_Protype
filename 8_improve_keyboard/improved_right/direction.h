#define base_move_pixels      4
#define exponential_bound     15
#define exponential_base      1.18

#ifndef DIRECTION_H
#define DIRECTION_H

#include "Arduino.h"

class Direction {
public:
    Direction(int pin1, int pin2);
    int read_action();
private:
    int pins[2];
    int current_actions[2];
    int last_actions[2];
    int exponential;
    double move_multiply;
    unsigned long current_action_times[2];
    unsigned long last_action_times[2];
};
#endif
