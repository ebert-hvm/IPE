#ifndef helpers_h
#define helpers_h

typedef enum servo_index
{
    base,
    whrist,
    grip,
    servo_count
} servo_index;

#endif

#ifndef Potentiometer_h
#define Potentiometer_h

//#include "Arduino.h"
//#include "helpers.h"

#define PIN_POT_1 A0
#define PIN_POT_2 A1
#define PIN_POT_3 A2
#define AVERAGE_SIZE 10

extern const int min_pot[servo_count],
    max_pot[servo_count],
    pins_pot[servo_count];

class Potentiometer
{
public:
    Potentiometer();
    int running_average_reading(int index);

private:
    int _pot_index;
    int _running_average[servo_count][AVERAGE_SIZE], _average_value[servo_count], _average_index[servo_count];
};
#endif
#ifndef Motors_h
#define Motors_h

//#include <Potentiometer.h>
#include <Servo.h>

#define PIN_SERVO_1 9
#define PIN_SERVO_2 10
#define PIN_SERVO_3 11
#define LIST_SIZE 50

extern const int pins_servo[servo_count],
    min_pwm[servo_count],
    max_pwm[servo_count],
    movement_frame_time,
    transition_interval;
extern int home_position[servo_count];

class Motors
{
public:
    Motors();
    bool update_position(int interval);
    void save_position();
    void execute();
    void reset();

private:
    Potentiometer _pot;
    Servo _servos[servo_count];
    int _index, _pos_cnt, _position_index;
    int _positions[LIST_SIZE][servo_count];
    int _diff[servo_count],
        _max_steps,
        _cnt,
        _steps[servo_count],
        _rem[servo_count];
    int _cur[servo_count];
    void _move(int * pos);
};

#endif
//#include "Potentiometer.h"

const int min_pot[servo_count] = {0, 0, 0},
          max_pot[servo_count] = {1023, 1023, 1023},
          pins_pot[servo_count] = {PIN_POT_1, PIN_POT_2, PIN_POT_3};
int cnt, value;

int Potentiometer::running_average_reading(int index)
{
    _running_average[index][_average_index[index]] = analogRead(pins_pot[index]);
    value = 0;
    for (cnt = 0; cnt != AVERAGE_SIZE; cnt++)
    {
        value += _running_average[index][cnt];
    }
    value /= AVERAGE_SIZE;

    _average_index[index] = (_average_index[index] + 1) % AVERAGE_SIZE;
    return value;
}

Potentiometer::Potentiometer()
{
    for (_pot_index = 0; _pot_index != servo_count; _pot_index++)
    {
        pinMode(pins_pot[_pot_index], INPUT);
        for (_average_index[_pot_index] = 0; _average_index[_pot_index] != AVERAGE_SIZE; _average_index[_pot_index]++)
            _running_average[_pot_index][_average_index[_pot_index]] = analogRead(pins_pot[_pot_index]);
        _average_index[_pot_index] = 0;
        _average_value[_pot_index] = _running_average[_pot_index][_average_index[_pot_index]];
    }
}
//#include "Motors.h"

const int pins_servo[servo_count] = {PIN_SERVO_1, PIN_SERVO_2, PIN_SERVO_3},
          min_pwm[servo_count] = {600, 600, 600},
          max_pwm[servo_count] = {2400, 2400, 2400},
          movement_frame_time = 5,
          transition_interval = 500,
          update_position_frametime = 15;
int home_position[servo_count] = {1500, 1500, 1500};

Motors::Motors()
{
    _position_index = 0;

    for (_index = 0; _index != servo_count; _index++)
    {
        _servos[_index].attach(pins_servo[_index]);
        _servos[_index].writeMicroseconds(home_position[_index]);
        delay(100);
    }
}
bool Motors::update_position(int interval)
{
    if (interval > update_position_frametime)
    {
        for (_index = 0; _index != servo_count; _index++)
        {
            _servos[_index].writeMicroseconds(map(
                _pot.running_average_reading(_index),
             //   analogRead(pins_pot[_index]),
                min_pot[_index],
                max_pot[_index],
                min_pwm[_index],
                max_pwm[_index]));
        }
        return true;
    }
    return false;
}

/*bool Motors::update_position(int interval)
{
    if (interval > _update_position_frametime)
    {
        Serial.print(F("VTNCCCCCCCCCCC"));
        return true;
    }
    return false;
}*/
void Motors::save_position()
{
    for (_index = 0; _index != servo_index::servo_count; _index++)
    {
        _positions[_position_index][_index] = _servos[_index].readMicroseconds();
    }
    _position_index++;
    Serial.print(F("Position "));
    Serial.print(_position_index);
    Serial.print(F(": "));
    Serial.print(_servos[0].readMicroseconds());
    Serial.print(F(" "));
    Serial.print(_servos[1].readMicroseconds());
    Serial.print(F(" "));
    Serial.print(_servos[2].readMicroseconds());
    Serial.print(F(" Saved!\n"));
}

void Motors::_move(int * pos)
{
    int signal[servo_count] = {1,1,1};
    for (_index = 0; _index != servo_count; _index++)
    {
        _diff[_index] = pos[_index] - _cur[_index];
        if(_diff[_index] < 0)
            signal[_index] = -1;
        else if(_diff[_index] == 0)
            signal[_index] = 0;
        _diff[_index] = abs(_diff[_index]);
        _max_steps = max(_max_steps, _diff[_index]);
    }
    
    for(_cnt = 1; _cnt <= _max_steps; _cnt++)
    {
        for (_index = 0; _index != servo_count; _index++)
        {
            if(_cnt <= abs(_diff[_index]))
            {
                _servos[_index].writeMicroseconds(_cur[_index] + signal[_index]*_cnt);
            }
            delayMicroseconds(2000);
        }
    }
    for (_index = 0; _index != servo_count; _index++)
    {
        _cur[_index] = _servos[_index].readMicroseconds();
    }
    delay(500);
}

void Motors::execute()
{
    Serial.print(F("executing...\n"));
    
    for (_index = 0; _index != servo_count; _index++)
    {
        _cur[_index] = _servos[_index].readMicroseconds();
    }
    Motors::_move(home_position);
    
    for (_pos_cnt = 0; _pos_cnt < _position_index; _pos_cnt++)
    {
        Motors::_move(_positions[_pos_cnt]);
    }
    Motors::_move(home_position);
    Serial.print(F("done!\n"));
}

void Motors::reset()
{
    Serial.print("Positions reseted\n");
    _position_index = 0;
}
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

Motors * motors;
Button * button;

void execute_state()
{
    switch (cur_state)
    {
    case single_click:
        released = 1;
        Serial.print(F("Single click\n"));
        motors->save_position();
        break;
    case double_click:
        //released = 0;
        Serial.print(F("Double click\n"));
        motors->execute();
        released = 1;
        t_0 = t_1 = 0;
        t_0 = t_1 = millis();
        break;
    case hold:
        released = 0;
        Serial.print(F("Hold\n"));
        motors->reset();
        break;
    case not_pressed:
        //Serial.println(t_1 - t_0);
        released = 1;
        if(motors->update_position(t_1 - t_0))
            t_0 = millis();
        break;
    case pressed:
        //Serial.println(t_1 - t_0);
        released = 0;
        if(motors->update_position(t_1 - t_0))
            t_0 = millis();
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    motors = new Motors;
    button = new Button;

    released = 1;
    cur_state = not_pressed;
    t_0 = millis();
    t_1 = millis();
}

void loop()
{
    //Serial.println(button->is_pressed());    
    cur_state = button->button_state(released);
    t_1 = millis();
    execute_state();
}
