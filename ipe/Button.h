#ifndef Button_h
#define Button_h

#include "Arduino.h"

#define PIN_BUTTON 8

typedef enum state
{
    single_click,
    double_click,
    hold,
    not_pressed,
    pressed
} state;

class Button
{
public:
    Button();
    bool is_pressed();
    int button_state(bool released);

private:
    int _double_click_interval,
        _hold_interval,
        _t_0,
        _t_1;

    bool _check_double_click();
    bool _check_hold();
};

#endif
