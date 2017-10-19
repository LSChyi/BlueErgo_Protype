#ifndef _KEY_BTN_H_
#define _KEY_BTN_H_
#define BOUNCE_TIME 1

class KeyBtn {
public:
    KeyBtn();
    bool state_changed(bool read_state, unsigned long row_start);
private:
    bool last_state;
    unsigned long last_act_time;
};

#endif
