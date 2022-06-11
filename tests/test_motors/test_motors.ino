#include <Motors.h>

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
