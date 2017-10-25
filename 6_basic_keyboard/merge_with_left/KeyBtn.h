#ifndef _KEY_BTN_H_
#define _KEY_BTN_H_
#define BOUNCE_TIME 2

class KeyBtn {
public:
    KeyBtn();
    bool state_changed(bool read_state);
private:
    bool last_state;
    unsigned long last_act_time;
};

#endif
