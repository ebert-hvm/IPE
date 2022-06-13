#include "Motors.h"

const int pins_servo[servo_count] = {PIN_SERVO_1, PIN_SERVO_2, PIN_SERVO_3},
          min_pwm[servo_count] = {600, 600, 600},
          max_pwm[servo_count] = {2400, 2400, 2400},
          movement_frame_time = 5,
          play_interval = 800,
          update_position_frametime = 15;
int home_position[servo_count] = {1500, 1500, 1500},
    signal[servo_count] = {1,1,1},
    velocity = 1;

Motors::Motors()
{
    update_position_interval = 15;
    position_index = 0;
    for (_index = 0; _index != servo_count; _index++)
    {
        _servos[_index].attach(pins_servo[_index]);
        _servos[_index].writeMicroseconds(home_position[_index]);
        delay(100);
    }
    for (_index = 0; _index != servo_count; _index++)
    {
        _cur[_index] = home_position[_index];
    }
}

void Motors::update_position()
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
}

void Motors::save_position()
{
    if(position_index == 0)
    {
        for (int cnt = 0; cnt < 50; cnt++)
        {
            for (_index = 0; _index != servo_count; _index++)
            {
                _positions[cnt][_index] = _servos[_index].readMicroseconds();
            }    
        }
    }
    else
    {
        for (_index = 0; _index != servo_count; _index++)
        {
            _positions[position_index][_index] = _servos[_index].readMicroseconds();
        }
    }
    Serial.print(F("Position "));
    Serial.print(position_index);
    Serial.print(F(": "));
    Serial.print(_servos[0].readMicroseconds());
    Serial.print(F(" "));
    Serial.print(_servos[1].readMicroseconds());
    Serial.print(F(" "));
    Serial.print(_servos[2].readMicroseconds());
    Serial.print(F(" Saved!\n"));
    position_index++;
}

void Motors::play(int cnt)
{
    if(cnt >0){
        Serial.print(F("From "));
        Serial.print(cnt-1);
        Serial.print(F(": "));
        Serial.print(_cur[0]);
        Serial.print(F(" "));
        Serial.print(_cur[1]);
        Serial.print(F(" "));
        Serial.print(_cur[2]);
        Serial.print(F("\nTo "));
        Serial.print(cnt);
        Serial.print(F(": "));
        Serial.print(_positions[cnt][0]);
        Serial.print(F(" "));
        Serial.print(_positions[cnt][1]);
        Serial.print(F(" "));
        Serial.print(_positions[cnt][2]);
    }
    _max_steps = 0;
    for (_index = 0; _index != servo_count; _index++)
    {
        if(cnt == 0) _cur[_index] = _servos[_index].readMicroseconds();
        _diff[_index] = _positions[cnt][_index] - _cur[_index];
        if(_diff[_index] < 0)
            signal[_index] = -1;
        else if(_diff[_index] == 0)
            signal[_index] = 0;
        else
            signal[_index] = 1;
        _diff[_index] = abs(_diff[_index]);
        _max_steps = max(_max_steps, _diff[_index]);
    }
    
    for(_cnt = 1; _cnt <= _max_steps; _cnt += velocity)
    {
        for (_index = 0; _index != servo_count; _index++)
        {
            if(_cnt <= abs(_diff[_index]))
            {
                _servos[_index].writeMicroseconds(_cur[_index] + signal[_index]*_cnt);
            }
            delayMicroseconds(2000);
        }
        if(_cnt > _max_steps)
        {
            _servos[_index].writeMicroseconds(_positions[cnt][_index]);
        }
    }
    for (_index = 0; _index != servo_count; _index++)
    {
        _cur[_index] = _servos[_index].readMicroseconds();
    }
}

void Motors::reset()
{
    position_index = 0;
}
