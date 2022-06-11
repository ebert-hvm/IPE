#include <Potentiometer.h>
int * reading;
Potentiometer * pot;

void setup()
{
    Serial.begin(9600);
    pot = new Potentiometer;
}

void loop()
{ 
    Serial.print(pot->running_average_reading(0));
    Serial.print(F(" "));
    Serial.print(pot->running_average_reading(1));
    Serial.print(F(" "));
    Serial.print(pot->running_average_reading(2));
    Serial.print(F("\n"));
    /*Serial.print(analogRead(A0));
    Serial.print(F(" "));
    Serial.print(analogRead(A1));
    Serial.print(F(" "));
    Serial.print(analogRead(A2));
    Serial.print(F("\n\n"));*/
    delay(15);
}
