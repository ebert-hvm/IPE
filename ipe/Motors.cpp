#include "Motors.h"

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