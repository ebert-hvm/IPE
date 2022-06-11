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

float * reading;
Potentiometer * pot;

void setup()
{
    Serial.begin(9600);
    pot = new Potentiometer;
}

void loop()
{
    reading = pot->running_average_reading();
    Serial.print(reading[0]);
    Serial.print(F(" "));
    Serial.print(reading[1]);
    Serial.print(F(" "));
    Serial.print(reading[2]);
    Serial.print(F("\n"));
    Serial.print(analogRead(A0));
    Serial.print(F(" "));
    Serial.print(analogRead(A1));
    Serial.print(F(" "));
    Serial.print(analogRead(A2));
    Serial.print(F("\n\n"));
    delay(15);
}
