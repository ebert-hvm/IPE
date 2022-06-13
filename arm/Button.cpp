#include "Arduino.h"
#include "Button.h"

Button::Button()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    update_interval = 50,
    _double_click_interval = 500,
    _hold_interval = 2000,
    _t_0 = 0,
    _t_1 = 0;
}

bool Button::is_pressed()
{
    if (digitalRead(PIN_BUTTON) == LOW)
        return true;
    else
        return false;
}

bool Button::_check_double_click()
{
    _t_0 = millis();
    _t_1 = millis();
    while (_t_1 - _t_0 < _double_click_interval)
    { // esperando segundo clique
        if (Button::is_pressed())
        {                // houve segundo clique
            delay(500);
            return true; // houve 2 cliques
        }
        _t_1 = millis();
    }
    return false; // houve apenas 1 clique
}

bool Button::_check_hold()
{
    _t_0 = _t_1 = millis();

    while (_t_1 - _t_0 < _hold_interval)
    {
        if (Button::is_pressed())
        {
            _t_1 = millis();
            continue;
        }
        else
            return false;
    }
    return true;
}

int Button::update()
{
    if (Button::is_pressed()){
        if (Button::_check_hold())
        {
            return reset;
        }
        else if (Button::_check_double_click()){
            return play;
        }
        else{
            return save_position;
        }
    }
    else{
        // Serial.println(_released);
        return learn;
    }
}