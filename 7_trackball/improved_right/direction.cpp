#include "direction.h"

Direction::Direction(int pin1, int pin2) {
    pins[0] = pin1;
    pins[1] = pin2;
    pinMode(pins[0], INPUT);
    pinMode(pins[1], INPUT);
}

int Direction::read_action() {
    for(int i = 0; i < 2; ++i) {
      current_actions[i] = digitalRead(pins[i]);
      current_action_times[i] = millis();
      if (current_actions[i] != last_actions[i]) {
        last_actions[i] = current_actions[i];
        exponential = (exponential_bound - (current_action_times[i] - last_action_times[i]));
        exponential = (exponential > 0) ? exponential : 1;
        move_multiply = exponential_base;
        for (int i = 0; i < exponential; ++i) {
          move_multiply *= exponential_base;
        }
        last_action_times[i] = current_action_times[i];
        if (i == 0) {
          return (-1) * base_move_pixels * move_multiply;
        } else {
          return base_move_pixels * move_multiply;
        }
      }
    }

    return 0;
}
