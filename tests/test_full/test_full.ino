#include <Button.h>
#include <Motors.h>

int t_0, t_1, cur_state;
bool released;

Motors * motors;
Button * button;

void execute_state()
{
    switch (cur_state)
    {
    case single_click:  
        released = 1;
        Serial.print(F("Single click\n"));
        motors->save_position();
        break;
    case double_click:
        //released = 0;
        Serial.print(F("Double click\n"));
        motors->execute();
        released = 1;
        t_0 = t_1 = 0;
        t_0 = t_1 = millis();
        break;
    case hold:
        released = 0;
        Serial.print(F("Hold\n"));
        motors->reset();
        break;
    case not_pressed:
        //Serial.println(t_1 - t_0);
        released = 1;
        if(motors->update_position(t_1 - t_0))
            t_0 = millis();
        break;
    case pressed:
        //Serial.println(t_1 - t_0);
        released = 0;
        if(motors->update_position(t_1 - t_0))
            t_0 = millis();
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    motors = new Motors;
    button = new Button;

    released = 1;
    cur_state = not_pressed;
    t_0 = millis();
    t_1 = millis();
}

void loop()
{
    //Serial.println(button->is_pressed());    
    cur_state = button->button_state(released);
    t_1 = millis();
    execute_state();
}
