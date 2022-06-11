#include "Potentiometer.h"

const int min_pot[servo_count] = {0, 0, 0},
          max_pot[servo_count] = {1023, 1023, 1023},
          pins_pot[servo_index::servo_count] = {PIN_POT_1, PIN_POT_2, PIN_POT_3};
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