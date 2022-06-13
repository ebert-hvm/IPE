#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include "helpers.h"

#define PIN_BUTTON 8

class Button
{
public:
    int update_interval;
    Button();
    bool is_pressed();
    int update();

private:
    int _double_click_interval,
        _hold_interval,
        _t_0,
        _t_1;
    bool _released;

    bool _check_double_click();
    bool _check_hold();
};

#endif
