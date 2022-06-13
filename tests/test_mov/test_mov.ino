#include <Servo.h>

int i = 0;
Servo s;

void setup()
{
    Serial.begin(9600);
    s.attach(10, 600, 2400);
}

void loop()
{
    s.write(0);
    delay(2000);
    s.write(180);
    delay(2000);
}
