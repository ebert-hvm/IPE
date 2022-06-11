//#include <Motors.h>
#include <Button.h>

int t_0, t_1, cur_state;
bool released;

//Motors * motors;
Button * button;

void execute_state()
{
    switch (cur_state)
    {
    case single_click:
        released = 1;
        Serial.print(F("Single click\n"));
  //      motors.save_position();
        break;
    case double_click:
        released = 0;
        Serial.print(F("Double click\n"));
//        motors.play();
        break;
    case hold:
        released = 0;
        Serial.print(F("Hold\n"));
        // delay(1000);
    //    motors.reset();
        break;
    case not_pressed:
        released = 1;
        //motors.update_position(t_1 - t_0);
        //Serial.println('4');
        break;
    case pressed:
        released = 0;
        //motors.update_position(t_1 - t_0);
        //Serial.println('4');
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    button = new Button;
    //motors = new Motors;

    released = 1;
    cur_state = not_pressed;
    t_0 = millis();
    t_1 = millis();
}

void loop()
{
    //Serial.println(button.is_pressed());
    
    cur_state = button->button_state(released);
    t_1 = millis();
    execute_state();
}
