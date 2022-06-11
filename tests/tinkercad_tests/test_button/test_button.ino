#ifndef Button_h
#define Button_h

//#include "Arduino.h"

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

//#include "Arduino.h"
//#include "Button.h"

Button::Button()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
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
int Button::button_state(bool released)
{
    if (Button::is_pressed())
    {
        if (released)
        {
            if (Button::_check_hold())
            {
                return hold;
            }
            else if (Button::_check_double_click())
                return double_click;
            else
                return single_click;
        }
        else
            return pressed;
    }
    else
        // Serial.println(released);
        return not_pressed;
}

int t_0, t_1, cur_state;
bool released;

//Motors * motors;
Button * button;

void execute_state()
{
    switch (cur_state)
    {
    case single_click:
        released = 1;
        Serial.print(F("Single click\n"));
  //      motors.save_position();
        break;
    case double_click:
        released = 0;
        Serial.print(F("Double click\n"));
//        motors.play();
        break;
    case hold:
        released = 0;
        Serial.print(F("Hold\n"));
        // delay(1000);
    //    motors.reset();
        break;
    case not_pressed:
        released = 1;
        //motors.update_position(t_1 - t_0);
        //Serial.println('4');
        break;
    case pressed:
        released = 0;
        //motors.update_position(t_1 - t_0);
        //Serial.println('4');
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    button = new Button;
    //motors = new Motors;

    released = 1;
    cur_state = not_pressed;
    t_0 = millis();
    t_1 = millis();
}

void loop()
{
    //Serial.println(button.is_pressed());
    
    cur_state = button->button_state(released);
    t_1 = millis();
    execute_state();
}
