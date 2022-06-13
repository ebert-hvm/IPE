#ifndef Potentiometer_h
#define Potentiometer_h

#include "Arduino.h"
#include "helpers.h"

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
    int running_average_reading(int index);

private:
    int _pot_index;
    int _running_average[servo_count][AVERAGE_SIZE], _average_value[servo_count], _average_index[servo_count];
};
#endif