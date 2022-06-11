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

extern const int min_pot[servo_index::servo_count],
    max_pot[servo_index::servo_count],
    pins_pot[servo_index::servo_count];

class Potentiometer
{
public:
    Potentiometer();
    float *running_average_reading();

private:
    int _pot_index, _average_index;
    float _running_average[servo_count][AVERAGE_SIZE], _average_value[servo_count];
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

extern const int pins_servo[servo_index::servo_count],
    min_pwm[servo_index::servo_count],
    max_pwm[servo_index::servo_count],
    home_position[servo_index::servo_count],
    movement_frame_time,
    transition_interval;

int mapf(float x, int in_min, int in_max, int out_min, int out_max);

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
    Servo _servos[servo_index::servo_count];
    int _index,
        _position_index;
    int _positions[servo_count][LIST_SIZE];
};

#endif
//#include "Potentiometer.h"

const int min_pot[servo_count] = {0, 0, 0},
          max_pot[servo_count] = {1023, 1023, 1023},
          pins_pot[servo_index::servo_count] = {PIN_POT_1, PIN_POT_2, PIN_POT_3};

float * Potentiometer::running_average_reading()
{
    for (_pot_index = 0; _pot_index != servo_index::servo_count; _pot_index++)
    {
        _average_value[_pot_index] -= _running_average[_pot_index][_average_index] / AVERAGE_SIZE;
        _running_average[_pot_index][_average_index] = analogRead(pins_pot[_pot_index]);
        _average_value[_pot_index] += _running_average[_pot_index][_average_index] / AVERAGE_SIZE;
    }
    _average_index = (_average_index + 1) % AVERAGE_SIZE;
    Serial.print(int(_average_value[0]));
    Serial.print(F(" "));
    Serial.print(int(_average_value[1]));
    Serial.print(F(" "));
    Serial.print(int(_average_value[2]));
    Serial.print(F("\n"));
    return _average_value;
}

Potentiometer::Potentiometer()
{
    _average_index = 0;
    for (_pot_index = 0; _pot_index != servo_index::servo_count; _pot_index++)
    {
        pinMode(pins_pot[_pot_index], INPUT);
    }
    for (_average_index = 0; _average_index != AVERAGE_SIZE; _average_index++)
    {
        Potentiometer::running_average_reading();
    }
    _average_index = 0;
}

//#include "Motors.h"

const int pins_servo[servo_index::servo_count] = {PIN_SERVO_1, PIN_SERVO_2, PIN_SERVO_3},
          min_pwm[servo_index::servo_count] = {600, 600, 600},
          max_pwm[servo_index::servo_count] = {2400, 2400, 2400},
          home_position[servo_index::servo_count] = {1500, 1500, 1500},
          movement_frame_time = 15,
          transition_interval = 500,
          update_position_frametime = 15;

int mapf(float x, int in_min, int in_max, int out_min, int out_max)
{
    return int(((x - 1.0 * in_min) * (out_max - out_min)) / (1.0 * (in_max - in_min)) + 1.0 * out_min);
}

Motors::Motors()
{
    _position_index = 0;

    for (_index = 0; _index != servo_index::servo_count; _index++)
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
        for (_index = 0; _index != servo_index::servo_count; _index++)
        {
            _servos[_index].writeMicroseconds(mapf(
                _pot.running_average_reading()[_index],
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
        _positions[_index][_position_index] = _servos[_index].readMicroseconds();
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

void Motors::execute()
{
    Serial.print(F("executing...\n"));
    float cur[servo_count],
          steps[servo_count];
    int i, j, max_steps;
    for (_index = 0; _index != servo_count; _index++)
    {
        _servos[_index].writeMicroseconds(home_position[_index]);
        cur[_index] = static_cast<float>(home_position[_index]);
        delay(100);
    }
    for (i = 0; i < _position_index; i++)
    {
        max_steps = max(_positions[base][i] - static_cast<int>(cur[base]),
                        max(_positions[whrist][i] - static_cast<int>(cur[whrist]),
                            _positions[grip][i] - static_cast<int>(cur[grip])));
        for (_index = 0; _index != servo_count; _index++)
        {
            steps[_index] = (_positions[_index][i] - static_cast<int>(cur[_index])) / max_steps;
        }
        for (j = 0; j < max_steps; j++)
        {
            for (_index = 0; _index != servo_count; _index++)
            {
                cur[_index] += steps[_index];
                _servos[_index].writeMicroseconds(static_cast<int>(cur[_index]));
            }
            delay(movement_frame_time);
        }
        delay(transition_interval);
    }
    Serial.print(F("done!\n"));
}

void Motors::reset()
{
    Serial.print("Positions reseted\n");
    _position_index = 0;
}

int t_0, t_1;
Motors * motors;

void setup()
{
    Serial.begin(9600);
  	motors = new Motors;
    t_0 = t_1 = millis();
}


void loop()
{
    if(motors->update_position(t_1 - t_0))
        t_0 = millis();
    t_1 = millis();
}
